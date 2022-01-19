/**
 *Copyright (c) 2022 LZ.All Right Reserved
 *Author : LZ
 *Date: 2022.01.19
 *Email: 2117610943@qq.com
 */
#include "LZHRankingListLayer.h"
#include "Based/LZGetMomentTime.h"
#include "Based/LZUserData.h"

HRankingListLayer::HRankingListLayer()
{
	_csvFile = nullptr;
	_mostLevel = max(1, UserData::getInstance()->openIntUserData(const_cast<char*>("MOST_HAMMERZOMBIES_LEVEL_NUMBER")));
}

HRankingListLayer::~HRankingListLayer()
{
}

HRankingListLayer* HRankingListLayer::createLayer(bool notUploadData)
{
	IRankingListLayer::notUploadData = notUploadData;
	return HRankingListLayer::create();
}

void HRankingListLayer::onShowDifferentTitle()
{
	if (!notUploadData)
	{
		auto title = cocos2d::ui::Text::create();
		title->setFontName(GAME_FONT_NAME_1);
		title->setFontSize(GAME_TEXT_SIZE("“我僵尸模式” 玩家闯关记录排行榜"));
		title->setString("“锤僵尸模式” 玩家闯关记录排行榜");
		title->setPosition(Vec2(660, 980));
		this->addChild(title);
	}
	else
	{
		onShowTitleButton(2);
	}
}

void HRankingListLayer::onDownloadRankingList()
{
	const string sURLList = "https://gitee.com/GITLZ/PVZDownLoader/raw/master/hz.csv";
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

void HRankingListLayer::onUploadData()
{
	_uploadButton->setEnabled(false);

	Application::getInstance()->sendLeveData(_global->userInformation->getUserName(), _mostLevel, 2);
	UserData::getInstance()->caveUserData(const_cast<char*>("HRANKINGLISTDATAUPLOAD"), static_cast<int>(_mostLevel));
	UserDefault::getInstance()->setIntegerForKey("RECORDDAY", _nowNettime->getNetDay());
	UserDefault::getInstance()->setIntegerForKey("RECORDMON", _nowNettime->getNetMon());
	UserDefault::getInstance()->setBoolForKey("ISHZRECORD", true);

	onSuccessfulFeedback();
}

void HRankingListLayer::onCheckUploadButtonEnable()
{
	if (!notUploadData)
	{
		_nowNettime = new MomentTime();
		_nowNettime->requestNetTime([this]()
			{
				auto recordMon = UserDefault::getInstance()->getIntegerForKey("RECORDMON");
				if (_nowNettime->getNetMon() != recordMon)
				{
					UserData::getInstance()->caveUserData(const_cast<char*>("HRANKINGLISTDATAUPLOAD"), 0);
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

				auto mostUpload = UserData::getInstance()->openIntUserData(const_cast<char*>("HRANKINGLISTDATAUPLOAD"));
				auto isTgRecord = UserDefault::getInstance()->getBoolForKey("ISHZRECORD");
				if (!isTgRecord && mostUpload < static_cast<int>(_mostLevel)) // 没有上传过并且上传的最高记录小于当前记录
				{
					_uploadButton->setEnabled(true);
				}
			}, true);
	}
}

void HRankingListLayer::onSelectCsvFile(int id)
{
	if (id == 0)
	{
		_director->getRunningScene()->addChild(IRankingListLayer::createLayer(), 10, "IRankingListLayer");
		deleteDialog();
	}
	else if (id == 1)
	{
		_director->getRunningScene()->addChild(TRankingListLayer::createLayer(), 10, "TRankingListLayer");
		deleteDialog();
	}
	else if (id == 2)
	{
		onParseCsvData();
	}
}
