/**
 *Copyright (c) 2020 LZ.All Right Reserved
 *Author : LZ
 *Date: 2020.4.2
 *Email: 2117610943@qq.com
 */

#include "UpdateClient.h"
#include "spine/spine-cocos2dx.h"

using namespace spine;

UpdateClient::UpdateClient():
    _dialog(nullptr)
{
    _downloader.reset(new network::Downloader());
}

bool UpdateClient::init()
{
    if (!LayerColor::initWithColor(Color4B(0, 0, 0, 180)))return false;
    createShieldLayer(this);

    createDiglog();
    return true;
}

void UpdateClient::createDiglog()
{
    _dialog = Sprite::createWithSpriteFrameName("LevelObjiectivesBg.png");
    _dialog->setPosition(_director->getWinSize() / 2);
    _dialog->setScale(0.9f);
    this->addChild(_dialog);

    auto PauseAnimation = SkeletonAnimation::createWithData(_global->userInformation->getAnimationData().find("PauseAnimation")->second);
    PauseAnimation->setAnimation(0, "animation", true);
    PauseAnimation->setPosition(Vec2(530, 650));
    _dialog->addChild(PauseAnimation);

    showText();

    createButton("百度网盘下载", Vec2(210, 100), Update_Button::百度网盘下载);
	createButton("腾讯微云下载", Vec2(520, 100), Update_Button::腾讯微云下载);
    createButton("直接下载", Vec2(830, 100), Update_Button::直接下载);
	createButton("确定", Vec2(520, 100), Update_Button::确定);

    /* 创建触摸监听 */
    createTouchtListener(_dialog);
}

void UpdateClient::createButton(const std::string& name, Vec2& vec2, Update_Button buttonType)
{
	/* 创建返回主菜单按钮 */
	auto button = ui::Button::create("ButtonNew2.png", "ButtonNew.png", "", TextureResType::PLIST);
	auto label = Label::createWithTTF(name, GAME_FONT_NAME_1, 35);
	label->enableShadow(Color4B(0, 0, 0, 200));//设置阴影
	label->setScale(2.0f);
	button->setTitleLabel(label);
	button->setTitleColor(Color3B::WHITE);
	button->setPosition(vec2);
	button->setScale(0.5f);
	button->setName(to_string(static_cast<int>(buttonType)));
	_dialog->addChild(button);

	if (buttonType == Update_Button::确定)
	{
		button->setVisible(false);
	}

	button->addTouchEventListener([=](Ref* sender, ui::Widget::TouchEventType type)
		{
			switch (type)
			{
			case ui::Widget::TouchEventType::BEGAN:
				AudioEngine::setVolume(AudioEngine::play2d(_global->userInformation->getMusicPath().find("gravebutton")->second), _global->userInformation->getSoundEffectVolume());
				break;
			case ui::Widget::TouchEventType::ENDED:
				switch (buttonType)
				{
				case Update_Button::百度网盘下载:
					Application::getInstance()->openURL("https://pan.baidu.com/s/1NlpRcQTv1FrN51UgAzGiHw&shfl=sharepset#/");
					break;
				case Update_Button::腾讯微云下载:
					Application::getInstance()->openURL("https://share.weiyun.com/5TewoDc");
					break;
				case Update_Button::直接下载:
					_remindText->setString("正在连接中，请稍等！");
					_remindText->setPosition(Vec2(_dialog->getContentSize().width / 2.f, _dialog->getContentSize().height / 2.f + 200));
					button->setEnabled(false);
                    downloadData();
					break;
				case Update_Button::确定:
					Director::getInstance()->end();
					break;
				}
			}
		});
}

void UpdateClient::showText()
{
	_remindText = Label::createWithTTF("检测到有新版本，请选择更新方式！", GAME_FONT_NAME_1, 50);
	_remindText->setPosition(Vec2(_dialog->getContentSize().width / 2.f, _dialog->getContentSize().height / 2.f));
	_remindText->setColor(Color3B::BLACK);
	_remindText->setMaxLineWidth(900);
	_remindText->setName("Update");
    _dialog->addChild(_remindText);
}

void UpdateClient::downloadData()
{
	_progressText = Label::createWithTTF("", GAME_FONT_NAME_1, 30);
	_progressText->setMaxLineWidth(900);
	_progressText->setPosition(Vec2(_dialog->getContentSize().width / 2.f, _dialog->getContentSize().height / 2.f));
	_dialog->addChild(_progressText);

	auto loadBarBackground = Sprite::createWithSpriteFrameName("bgFile.png");
	loadBarBackground->setPosition(Vec2(_dialog->getContentSize().width / 2.f, _dialog->getContentSize().height / 2.f - 100));
	_dialog->addChild(loadBarBackground);

	_explanText = Label::createWithTTF("使用“直接下载”可能下载速度很慢或直接连接失败（由于没钱只能白嫖国外服务器，你可以选择 CLIMB OVER THE WALL）", GAME_FONT_NAME_1, 30);
	_explanText->setPosition(Vec2(_dialog->getContentSize().width / 2.f, _dialog->getContentSize().height / 2.f + 100));
	_explanText->setColor(Color3B::BLACK);
	_explanText->setMaxLineWidth(900);
	_dialog->addChild(_explanText);

	auto bar = ui::LoadingBar::create();
	bar->loadTexture("progressFile.png", TextureResType::PLIST);
	bar->setDirection(LoadingBar::Direction::LEFT); /* 设置加载方向 */
	bar->setPercent(0);
	bar->setPosition(Vec2(_dialog->getContentSize().width / 2.f, _dialog->getContentSize().height / 2.f - 100));
	_dialog->addChild(bar);

	static string sNameList = "PlantsVsZombies_NEW.rar";
	static string path = "C:/Users/SoftWare/Desktop/" + sNameList;

	_downloader->createDownloadFileTask("https://github.com/ErLinErYi/PVZDownloader/raw/master/PlantsVsZombies_LZ_New.rar", path, sNameList);
    _downloader->onTaskProgress = [=](const network::DownloadTask& task,
        int64_t bytesReceived,
        int64_t totalBytesReceived,
        int64_t totalBytesExpected)
    {
        float percent = float(totalBytesReceived * 100) / totalBytesExpected;
        bar->setPercent(percent);

		char buf[32];
		snprintf(buf, 32, "%.3f %%[共 %d KB]", percent, int(totalBytesExpected / 1024));
		_progressText->setString(buf);

		_remindText->setString("文件正在下载中！请稍等！");
    };

	_downloader->onFileTaskSuccess = [this](const cocos2d::network::DownloadTask& task)
	{
		char buf[128];
		snprintf(buf, 128, "下载 [%s] 成功。已存放到桌面", task.identifier.c_str());
		_progressText->setString(buf);
		_remindText->setString("点击确定退出游戏！");

		((Button*)_dialog->getChildByName("0"))->setVisible(false);
		((Button*)_dialog->getChildByName("1"))->setVisible(false);
		((Button*)_dialog->getChildByName("2"))->setVisible(false);
		((Button*)_dialog->getChildByName("3"))->setVisible(true);
	};

	_downloader->onTaskError = [this](const cocos2d::network::DownloadTask& task,
		int errorCode,
		int errorCodeInternal,
		const std::string& errorStr)
	{
		_remindText->setString("文件下载失败！请重试！");
		_remindText->setPosition(Vec2(_dialog->getContentSize().width / 2.f, _dialog->getContentSize().height / 2.f + 200));
		((Button*)_dialog->getChildByName("2"))->setEnabled(true);

		log("Failed to download : %s, identifier(%s) error code(%d), internal error code(%d) desc(%s)"
			, task.requestURL.c_str()
			, task.identifier.c_str()
			, errorCode
			, errorCodeInternal
			, errorStr.c_str());
	};
}
