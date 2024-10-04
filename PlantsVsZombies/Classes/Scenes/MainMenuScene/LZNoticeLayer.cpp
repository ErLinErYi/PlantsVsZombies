/**
 *Copyright (c) 2024 LZ.All Right Reserved
 *Author : LZ
 *Date: 2024.01.26
 *Email: 2117610943@qq.com
 */

#include "LZNoticeLayer.h"
#include <Based/LZDefine.h>
#include <Based/LZPlayMusic.h>

NoticeLayer::NoticeLayer()
{
	_downloader.reset(new network::Downloader());
}

NoticeLayer::~NoticeLayer()
{
}

bool NoticeLayer::init()
{
	if (!LayerColor::initWithColor(Color4B(0, 0, 0, 180)))return false;

	createShieldLayer(this);
	setOpacity(0);
	setScale(0.8f);
	runAction(Spawn::create(FadeTo::create(0.2f, 180), ScaleTo::create(0.2f, 1.f), nullptr));

	_drawAction = DrawNode::create();
	this->addChild(_drawAction, 1);
	_drawNode = DrawNode::create();
	_drawNode->setLineWidth(1);
	_drawNode->drawSolidRect(Vec2(150, 150), Vec2(1770, 930), Color4F(0.2f, 0.2f, 0.2f, 0.7f));
	_drawNode->drawRect(Vec2(149, 149), Vec2(1771, 931), Color4F(0.f, 0.9f, 0.9f, 0.5f));
	
	this->addChild(_drawNode);

	onShowBackButton();
	onShowNoticeText();
	downloadNoticeText();
	return true;
}

void NoticeLayer::onShowBackButton()
{
	auto button = Button::create("SeedChooser_Button2.png", "SeedChooser_Button2_Glow.png", "", cocos2d::ui::Widget::TextureResType::PLIST);
	button->setTitleText(GAME_TEXT("退出"));
	button->setTitleColor(Color3B(0, 255, 255));
	button->setTitleFontSize(GAME_TEXT_SIZE("退出"));
	button->setScale(2.0f);
	button->setPosition(Vec2(_director->getInstance()->getWinSize().width / 2.f, 100));
	this->addChild(button);
	button->addTouchEventListener([&](Ref* sender, Widget::TouchEventType type)
		{
			switch (type)
			{
			case Widget::TouchEventType::BEGAN:
				PlayMusic::playMusic("tap2");
				break;
			case Widget::TouchEventType::ENDED:
				setMouseListenerEnable();
				deleteDialog();
				break;
			}
		});
}

void NoticeLayer::onShowNoticeText()
{
	auto notice = Label::createWithTTF("公告", GAME_FONT_NAME_1, 50);
	notice->setPosition(Vec2(_director->getInstance()->getWinSize().width / 2.f, 870));
	this->addChild(notice);

	TTFConfig ttfConfig(GAME_FONT_NAME_1, 30, GlyphCollection::DYNAMIC);
	_noticeText = Label::createWithTTF(ttfConfig, "文本加载中!");
	_noticeText->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	_noticeText->setMaxLineWidth(1500);
	_noticeText->setPosition(Vec2(_director->getInstance()->getWinSize().width / 2.f, _director->getInstance()->getWinSize().height / 2.f - 40));
	_noticeText->setLineBreakWithoutSpace(true);
	this->addChild(_noticeText);
}

void NoticeLayer::downloadNoticeText()
{
	_downloader->createDownloadDataTask("http://www.lzgd.xyz/notice.txt");
	_downloader->onDataTaskSuccess = [this](const cocos2d::network::DownloadTask& task,
		std::vector<unsigned char>& data)
		{
			string noticeNetWork;
			for (auto p : data)
			{
				noticeNetWork += p;
			}
			_noticeText->setString(noticeNetWork);
		};
	_downloader->onTaskError = [this](const cocos2d::network::DownloadTask& task,
		int errorCode,
		int errorCodeInternal,
		const std::string& errorStr)
		{
			_noticeText->setString("暂无公告！！！");
		};
}
