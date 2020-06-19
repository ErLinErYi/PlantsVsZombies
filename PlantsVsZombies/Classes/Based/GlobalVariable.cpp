#include "Based/GlobalVariable.h"
#include "AudioEngine.h"

using namespace cocos2d::experimental;

Global* Global::_instance = nullptr;

Global* Global::getInstance()
{
	if (_instance == nullptr)
	{
		_instance = new (std::nothrow)Global;
	}
	return _instance;
}

Global::Global() :
  prohibitId(-1)
, _sumAnimationInterval(0)
, _frequence(0)
, userInformation(new UserInformation())
{
}

Global::~Global()
{
	if(userInformation)delete userInformation;
}

void Global::resumeProhibit()
{
	AudioEngine::setFinishCallback(prohibitId, [=](int i, string name)
		{
			prohibitId = -1;
		});
}

void Global::checkAnimationInterval()
{
	++_frequence;
	if (_sumAnimationInterval <= 0)
	{
		_sumAnimationInterval = 1.f / Director::getInstance()->getSecondsPerFrame();
	}
	else
	{
		_sumAnimationInterval = (_sumAnimationInterval + 1.f / Director::getInstance()->getSecondsPerFrame()) / 2;
	}

	if (_sumAnimationInterval <= 20 && _frequence > 60)
	{
		AudioEngine::stopAll();
		wstring str = L"        很抱歉！检测到程序运行的平均帧数太低对游戏的可玩性有极大的影响。\
所以请点击确定按钮关闭此游戏！关闭不使用的其他程序或提升电脑硬件配置等操作后再次尝试运行此游戏。";

		if (MessageBoxW(Director::getInstance()->getOpenGLView()->getWin32Window(), str.c_str(), L"游戏平均帧数太低(平均FPS <= 20FPS)", MB_ICONERROR | MB_OK) == MB_OKCANCEL)
		{
			Director::getInstance()->end();
		}
	}
}

