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
	_isRecordName = "ISHZRECORD";
	_mostLevelName = "HRANKINGLISTDATAUPLOAD";
	_sURLList = "https://gitee.com/GITLZ/PVZDownLoader/raw/master/hz.csv";
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
		title->setFontSize(GAME_TEXT_SIZE("“锤僵尸模式” 玩家闯关记录排行榜"));
		title->setString(GAME_TEXT("“锤僵尸模式” 玩家闯关记录排行榜"));
		title->setPosition(Vec2(660, 980));
		this->addChild(title);
	}
	else
	{
		onShowTitleButton(2);
	}
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
