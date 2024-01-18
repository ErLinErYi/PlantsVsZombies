#include "LZAppDelegate.h"
#include "Scenes/LoadingScene/LZLoadingScene.h"
#include "Based/LZUserInformation.h"

#define USE_AUDIO_ENGINE 1
#define USE_SIMPLE_AUDIO_ENGINE 0

#ifdef WIN32
#pragma execution_character_set("utf-8")
#endif

#if USE_AUDIO_ENGINE && USE_SIMPLE_AUDIO_ENGINE
#error "Don't use AudioEngine and SimpleAudioEngine at the same time. Please just select one in your game!"
#endif

#if USE_AUDIO_ENGINE
#include "audio/include/AudioEngine.h"
using namespace cocos2d::experimental;
#elif USE_SIMPLE_AUDIO_ENGINE
#include "audio/include/SimpleAudioEngine.h"
using namespace CocosDenshion;
#endif

USING_NS_CC;

static cocos2d::Size designResolutionSize = cocos2d::Size(1920, 1080);
GUID scheme_default;

AppDelegate::AppDelegate()
{
}

AppDelegate::~AppDelegate()
{
#if USE_AUDIO_ENGINE
	AudioEngine::end();
#elif USE_SIMPLE_AUDIO_ENGINE
	SimpleAudioEngine::end();
#endif
}

void AppDelegate::initGLContextAttrs()
{
	// set OpenGL context attributes: red,green,blue,alpha,depth,stencil,multisamplesCount
	GLContextAttrs glContextAttrs = { 8, 8, 8, 8, 24, 8, 0 };

	GLView::setGLContextAttrs(glContextAttrs);
}

static int register_all_packages()
{
	SpriteFrameCache::getInstance()->addSpriteFramesWithFile(
		"resources/Images/LoadingScene/LoadingScene.plist",
		"resources/Images/LoadingScene/LoadingScene.pvr.ccz");
	return 0;
}

bool AppDelegate::applicationDidFinishLaunching()
{
	auto director = Director::getInstance();

	auto glview = director->getOpenGLView();

	if (!glview)
	{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32) || (CC_TARGET_PLATFORM == CC_PLATFORM_MAC) || (CC_TARGET_PLATFORM == CC_PLATFORM_LINUX)
		glview = GLViewImpl::createWithRect(LZPVZNAME, cocos2d::Rect(0, 0, designResolutionSize.width, designResolutionSize.height), 1.0f);
#else
		glview = GLViewImpl::create(LZPVZNAME);
#endif
		director->setOpenGLView(glview);
	}

	RemoveMenu(GetSystemMenu(director->getOpenGLView()->getWin32Window(), false), SC_CLOSE, MF_BYCOMMAND);

	register_all_packages();

	// turn on display FPS
	director->setDisplayStats(true);

	// set FPS. the default value is 1.0/60 if you don't call this
	director->setAnimationInterval(1.0f / UserInformation::getScreenDisplayFrequency());

	// Set the design resolution
	glview->setDesignResolutionSize(designResolutionSize.width, designResolutionSize.height, ResolutionPolicy::EXACT_FIT);

	// create a scene. it's an autorelease object
	auto scene = LoadingScene::create();

	// run
	director->runWithScene(scene);

	return true;
}

void AppDelegate::applicationDidEnterBackground()
{
	Director::getInstance()->stopAnimation();

#if USE_AUDIO_ENGINE
	AudioEngine::pauseAll();
#elif USE_SIMPLE_AUDIO_ENGINE
	SimpleAudioEngine::getInstance()->pauseBackgroundMusic();
	SimpleAudioEngine::getInstance()->pauseAllEffects();
#endif
}

void AppDelegate::applicationWillEnterForeground()
{
	Director::getInstance()->startAnimation();

#if USE_AUDIO_ENGINE
	AudioEngine::resumeAll();
#elif USE_SIMPLE_AUDIO_ENGINE
	SimpleAudioEngine::getInstance()->resumeBackgroundMusic();
	SimpleAudioEngine::getInstance()->resumeAllEffects();
#endif
}

void AppDelegate::applicationDidFocusCallBack()
{
#if USE_AUDIO_ENGINE
	const auto scene = Director::getInstance()->getRunningScene();
	if (scene && scene->getName() != "GameScene")
	{
		AudioEngine::resumeAll();
	}
#elif USE_SIMPLE_AUDIO_ENGINE
	SimpleAudioEngine::getInstance()->resumeBackgroundMusic();
	SimpleAudioEngine::getInstance()->resumeAllEffects();
#endif
	}

void AppDelegate::applicationDidUnFocusCallBack()
{
#if USE_AUDIO_ENGINE
	const auto scene = Director::getInstance()->getRunningScene();
	if (scene && scene->getName() != "GameScene")
	{
		AudioEngine::pauseAll();
	}
#elif USE_SIMPLE_AUDIO_ENGINE
	SimpleAudioEngine::getInstance()->pauseBackgroundMusic();
	SimpleAudioEngine::getInstance()->pauseAllEffects();
#endif
	}