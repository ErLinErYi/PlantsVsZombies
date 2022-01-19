/**
 *Copyright (c) 2022 LZ.All Right Reserved
 *Author : LZ
 *Date: 2022.01.18
 *Email: 2117610943@qq.com
 */
#include "LZTRankingListLayer.h"
#include "Based/LZPlayMusic.h"
#include "Based/LZUserData.h"
#include "Based/LZCsvFile.h"
#include "Based/LZGetMomentTime.h"
#include "Scenes/GameScenes/Adventure/GameScene/LZAGSPauseQuitLayer.h"
#include "Scenes/GameScenes/HammerZombies/GameScene/LZHRankingListLayer.h"

TRankingListLayer::TRankingListLayer()
{
	_csvFile = nullptr;
	_mostLevel = max(UserData::getInstance()->openIntUserData(const_cast<char*>("TESTINGGROUND")), 1);
}

TRankingListLayer::~TRankingListLayer()
{
}

TRankingListLayer* TRankingListLayer::createLayer(bool notUploadData)
{
	IRankingListLayer::notUploadData = notUploadData;
	return TRankingListLayer::create();
}

void TRankingListLayer::onShowDifferentTitle()
{
	if (!notUploadData)
	{
		auto title = cocos2d::ui::Text::create();
		title->setFontName(GAME_FONT_NAME_1);
		title->setFontSize(GAME_TEXT_SIZE("“我是僵尸模式” 玩家闯关记录排行榜"));
		title->setString("“植物试炼场模式” 玩家闯关记录排行榜");
		title->setPosition(Vec2(660, 980));
		this->addChild(title);
	}
	else
	{
		onShowTitleButton(1);
	}
}

void TRankingListLayer::onDownloadRankingList()
{
	const string sURLList = "https://gitee.com/GITLZ/PVZDownLoader/raw/master/tg.csv";
	_downloader->createDownloadDataTask(sURLList);
	_downloader->onDataTaskSuccess = [this](const cocos2d::network::DownloadTask& task,
		std::vector<unsigned char>& data)
	{
		_loadingText->setVisible(false);

		for (auto s : data)
		{
			_strRankingList += s;
		}

		onParseCsvData();
	};
	_downloader->onTaskError = [this](const cocos2d::network::DownloadTask& task,
		int errorCode,
		int errorCodeInternal,
		const std::string& errorStr)
	{
		_loadingText->setString(GAME_TEXT("排行榜加载失败！"));
		_loadingText->setColor(Color3B::RED);
	};
}

void TRankingListLayer::onShowBackButton()
{
	_uploadButton = Button::create("SeedChooser_Button2.png", "SeedChooser_Button2_Glow.png", "", cocos2d::ui::Widget::TextureResType::PLIST);
	_uploadButton->setTitleText(GAME_TEXT("数据上传"));
	_uploadButton->setTitleColor(Color3B(0, 255, 255));
	_uploadButton->setTitleFontSize(GAME_TEXT_SIZE("退出"));
	_uploadButton->setScale(2.0f);
	_uploadButton->setPosition(Vec2(1440, 70));
	_uploadButton->setEnabled(false);
	this->addChild(_uploadButton);
	_uploadButton->addTouchEventListener([&](Ref* sender, Widget::TouchEventType type)
		{
			switch (type)
			{
			case Widget::TouchEventType::BEGAN:
				PlayMusic::playMusic("tap2");
				break;
			case Widget::TouchEventType::ENDED:
				onUploadData();
				break;
			}
		});

	auto button2 = Button::create("SeedChooser_Button2.png", "SeedChooser_Button2_Glow.png", "", cocos2d::ui::Widget::TextureResType::PLIST);
	button2->setTitleText(GAME_TEXT("退出"));
	button2->setTitleColor(Color3B(0, 255, 255));
	button2->setTitleFontSize(GAME_TEXT_SIZE("退出"));
	button2->setScale(2.0f);
	button2->setPosition(Vec2(1785, 70));
	this->addChild(button2);
	button2->addTouchEventListener([&](Ref* sender, Widget::TouchEventType type)
		{
			switch (type)
			{
			case Widget::TouchEventType::BEGAN:
				PlayMusic::playMusic("tap2");
				break;
			case Widget::TouchEventType::ENDED:
				GSPauseQuitLayer::resumeLayer();
				deleteDialog();
				break;
			}
		});
}

void TRankingListLayer::onCheckUploadButtonEnable()
{
	if (!notUploadData)
	{
		_nowNettime = new MomentTime();
		_nowNettime->requestNetTime([this]()
			{
				auto recordMon = UserDefault::getInstance()->getIntegerForKey("RECORDMON");
				if (_nowNettime->getNetMon() != recordMon)
				{
					UserData::getInstance()->caveUserData(const_cast<char*>("TRANKINGLISTDATAUPLOAD"), 0);
					UserDefault::getInstance()->setIntegerForKey("RECORDDAY", 0);
					UserDefault::getInstance()->setIntegerForKey("RECORDMON", 0);
					UserDefault::getInstance()->setBoolForKey("ISIZERECORD", false);
					UserDefault::getInstance()->setBoolForKey("ISTGRECORD", false);
					UserDefault::getInstance()->setBoolForKey("ISHZRECORD", false);
				}

				auto recordDay = UserDefault::getInstance()->getIntegerForKey("RECORDDAY");
				if (_nowNettime->getNetDay() != recordDay)
				{
					UserDefault::getInstance()->setBoolForKey("ISIZERECORD", false);
					UserDefault::getInstance()->setBoolForKey("ISTGRECORD", false);
					UserDefault::getInstance()->setBoolForKey("ISHZRECORD", false);
				}

				auto mostUpload = UserData::getInstance()->openIntUserData(const_cast<char*>("TRANKINGLISTDATAUPLOAD"));
				auto isTgRecord = UserDefault::getInstance()->getBoolForKey("ISTGRECORD");
				if (!isTgRecord && mostUpload < static_cast<int>(_mostLevel)) // 没有上传过并且上传的最高记录小于当前记录
				{
					_uploadButton->setEnabled(true);
				}
			}, true);
	}
}

void TRankingListLayer::onUploadData()
{
	_uploadButton->setEnabled(false);

	Application::getInstance()->sendLeveData(_global->userInformation->getUserName(), _mostLevel, 1);
	UserData::getInstance()->caveUserData(const_cast<char*>("TRANKINGLISTDATAUPLOAD"), static_cast<int>(_mostLevel));
	UserDefault::getInstance()->setIntegerForKey("RECORDDAY", _nowNettime->getNetDay());
	UserDefault::getInstance()->setIntegerForKey("RECORDMON", _nowNettime->getNetMon());
	UserDefault::getInstance()->setBoolForKey("ISTGRECORD", true);

	onSuccessfulFeedback();
}

void TRankingListLayer::onSelectCsvFile(int id)
{
	if (id == 0)
	{
		_director->getRunningScene()->addChild(IRankingListLayer::createLayer(), 10, "IRankingListLayer");
		deleteDialog();
	}
	else if (id == 1)
	{
		onParseCsvData();
	}
	else if (id == 2)
	{
		_director->getRunningScene()->addChild(HRankingListLayer::createLayer(), 10, "HRankingListLayer");
		deleteDialog();
	}
}
