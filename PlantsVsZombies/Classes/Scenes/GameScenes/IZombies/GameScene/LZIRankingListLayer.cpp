/**
 *Copyright (c) 2022 LZ.All Right Reserved
 *Author : LZ
 *Date: 2022.01.14
 *Email: 2117610943@qq.com
 */

#include "LZIRankingListLayer.h"
#include "LZIControlLayer.h"
#include "Based/LZPlayMusic.h"
#include "Based/LZCsvFile.h"
#include "Based/LZMouseEventControl.h"
#include "Based/LZUserData.h"
#include "Based/LZGetMomentTime.h"
#include "Based/LZDefine.h"
#include "Scenes/GameScenes/TestingGround/GameScene/LZTRankingListLayer.h"
#include "Scenes/GameScenes/HammerZombies/GameScene/LZHRankingListLayer.h"

bool IRankingListLayer::notUploadData = false;

IRankingListLayer::IRankingListLayer():
	_rankingListScrollView(nullptr),
	_draw(nullptr),
	_loadingText(nullptr),
	_uploadButton(nullptr),
    _nowNettime(nullptr),
	_csvFile(nullptr),
	_isRecordName("ISIZERECORD"),
	_mostLevelName("IRANKINGLISTDATAUPLOAD"),
	_selectClos(1),
	_mostLevel(0),
	_sURLList("https://raw.githubusercontent.com/ErLinErYi/PVZDownloader/master/ranking.csv")
{
	_downloader.reset(new network::Downloader());
	onSetMaxLevel(max(UserData::getInstance()->openIntUserData(const_cast<char*>("IZOMBIES_MOST_LEVEL")), 1));
}

IRankingListLayer::~IRankingListLayer()
{
	if (_nowNettime)delete _nowNettime;
	delete _csvFile;

	IRankingListLayer::notUploadData = false;
}

IRankingListLayer* IRankingListLayer::createLayer(bool notUploadData)
{
	IRankingListLayer::notUploadData = notUploadData;
	return IRankingListLayer::create();
}

bool IRankingListLayer::init()
{
	if (!LayerColor::initWithColor(Color4B(0, 0, 0, 180)))return false;

	createShieldLayer(this);
	onCreateScrollView();
	onShowTitle();
	onShowLoadingText();
	//onCheckUploadButtonEnable();
	onDownloadRankingList();
	onShowUploadDataRequirement();
	onShowRankingListUpdateInformation();
	onShowBackButton();

	return true;
}

void IRankingListLayer::onShowTitle()
{
	_draw = DrawNode::create();
	_draw->drawSolidRect(Vec2(20, 20), Vec2(1300, 1060), Color4F(0.2f, 0.2f, 0.2f, 0.7f));
	_draw->drawSolidRect(Vec2(1320, 20), Vec2(1900, 530), Color4F(0.2f, 0.2f, 0.2f, 0.7f));
	_draw->drawSolidRect(Vec2(1320, 550), Vec2(1900, 1060), Color4F(0.2f, 0.2f, 0.2f, 0.7f));
	_draw->drawLine(Vec2(100, 900), Vec2(1220, 900), Color4F(0.5f, 0.5f, 0.5f, 0.7f));
	_draw->drawLine(Vec2(100, 800), Vec2(1220, 800), Color4F(0.5f, 0.5f, 0.5f, 0.7f));

	_draw->drawLine(Vec2(100, 900), Vec2(100, 100), Color4F(0.5f, 0.5f, 0.5f, 0.7f));
	_draw->drawLine(Vec2(300, 900), Vec2(300, 100), Color4F(0.5f, 0.5f, 0.5f, 0.7f));
	_draw->drawLine(Vec2(900, 900), Vec2(900, 100), Color4F(0.5f, 0.5f, 0.5f, 0.7f));
	_draw->drawLine(Vec2(1220, 900), Vec2(1220, 100), Color4F(0.5f, 0.5f, 0.5f, 0.7f));
	this->addChild(_draw);

	
	string str[3] = { "排名","玩家名称","最高记录" };
	Vec2 pos[3] = { Vec2(200, 850),Vec2(600, 850),Vec2(1060, 850) };
	for (int i = 0; i < 3; ++i)
	{
		auto ranking = cocos2d::ui::Text::create();
		ranking->setFontName(GAME_FONT_NAME_1);
		ranking->setFontSize(GAME_TEXT_SIZE(str[i]));
		ranking->setString(GAME_TEXT(str[i]));
		ranking->setPosition(pos[i]);
		ranking->setColor(Color3B(0, 255, 255));
		this->addChild(ranking);
	}

	onShowDifferentTitle();
}

void IRankingListLayer::onShowDifferentTitle()
{
	if (!notUploadData)
	{
		auto title = cocos2d::ui::Text::create();
		title->setFontName(GAME_FONT_NAME_1);
		title->setFontSize(GAME_TEXT_SIZE("“我是僵尸模式” 玩家闯关记录排行榜"));
		title->setString(GAME_TEXT("“我是僵尸模式” 玩家闯关记录排行榜"));
		title->setPosition(Vec2(660, 980));
		this->addChild(title);
	}
	else
	{
		onShowTitleButton(0);
	}
}

void IRankingListLayer::onShowTitleButton(int id)
{
	Vec2 pos[3] = { Vec2(100, 940) ,Vec2(480, 940) ,Vec2(860, 940) };
	string str[3] = { "我是僵尸模式排行榜","植物试炼场排行榜","锤僵尸模式排行榜" };
	for (int i = 0; i < 3; ++i)
	{
		auto layerColor = LayerColor::create(Color4B(i == id ? 0 : 255, 255, 255, 51));
		layerColor->setContentSize(Size(360, 80));
		layerColor->setPosition(pos[i]);
		this->addChild(layerColor);

		auto draw = DrawNode::create();
		draw->drawRect(Vec2(0, 0), Vec2(360, 80), Color4F(i == id ? 0 : 1, 1, 1, 0.5f));
		layerColor->addChild(draw);

		auto button = Button::create();
		button->ignoreContentAdaptWithSize(false);
		button->setContentSize(Size(360, 80));
		button->setTitleText(GAME_TEXT(str[i]));
		button->setTitleColor(Color3B(i == id ? 0 : 255, 255, 255));
		button->setTitleFontSize(GAME_TEXT_SIZE(str[i]));
		button->setTitleFontName(GAME_FONT_NAME_1);
		button->setPosition(Vec2(180, 40));
		button->setEnabled(!(i == id));
		button->addTouchEventListener([=](Ref* sender, Widget::TouchEventType type)
			{
				switch (type)
				{
				case Widget::TouchEventType::BEGAN:
					PlayMusic::playMusic("tap2");
					break;
				case Widget::TouchEventType::ENDED:
					onSelectCsvFile(i);
					break;
				}
			});
		layerColor->addChild(button);
	}
}

void IRankingListLayer::onShowBackButton()
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
				deleteDialog();
				break;
			}
		});
}

void IRankingListLayer::onCreateScrollView()
{
	_rankingListScrollView = cocos2d::ui::ScrollView::create();
	_rankingListScrollView->setDirection(cocos2d::ui::ScrollView::Direction::VERTICAL);
	_rankingListScrollView->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	_rankingListScrollView->setContentSize(Size(1120.f, 700.f));
	_rankingListScrollView->setPosition(Vec2(660.f, 450.f));
	_rankingListScrollView->setBounceEnabled(true);
	_rankingListScrollView->setScrollBarPositionFromCorner(Vec2(3, 0));
	_rankingListScrollView->setScrollBarWidth(5);
	_rankingListScrollView->setScrollBarColor(Color3B(200, 200, 200));
	this->addChild(_rankingListScrollView, 1);
}

void IRankingListLayer::onDownloadRankingList()
{
	_downloader->createDownloadDataTask(_sURLList);
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

void IRankingListLayer::onAddLocalData()
{
	vector<string> data;
	data.push_back(GAME_TEXT("本地 我：") + _global->userInformation->getUserName());
	data.push_back(to_string(onGetMaxLevel()));
	data.push_back("0");
	data.push_back("0");
	_csvFile->addNewData(data);
	_csvFile->deleteSuffix("\r", 1);
	_csvFile->sortData(1);
}

void IRankingListLayer::onSetMaxLevel(int level)
{
	_mostLevel = level;

	_encryptKey = rand();
	_mostLevel ^= _encryptKey;
}

int IRankingListLayer::onGetMaxLevel()
{
	return _mostLevel ^ _encryptKey;
}

void IRankingListLayer::onParseCsvData()
{
	if (!_csvFile)
	{
		_csvFile = new CSVFile();
		_csvFile->openFile(_strRankingList);
		_csvFile->sortData(0);
		_csvFile->removeDuplicationReservedMaximum(0);
		_csvFile->strMatchingAndInsertStr(0, Application::getInstance()->getUserName(), "  ", GAME_TEXT("网络 我："));
		_csvFile->deletePrefix("  ", 0);

		onAddLocalData();
	}
	
	_rankingListScrollView->removeAllChildren();

	auto row = _csvFile->getRows(_selectClos, "0");
	auto draw = DrawNode::create();
	
	_rankingListScrollView->setInnerContainerSize(Size(1120.f, max(700, 100 * row)));

	auto h = max(700, 100 * row);
	for (int i = 0; i <= row; ++i)
	{
		runAction(Sequence::create(DelayTime::create(i * 0.02f),
			CallFunc::create([=]()
				{
					draw->drawLine(Vec2(0, h - 100 * i), Vec2(1120, h - 100 * i), Color4F(0.5f, 0.5f, 0.5f, 0.7f));

					if (i != row)
					{
						auto num = cocos2d::ui::Text::create();
						num->setFontName(GAME_FONT_NAME_2);
						num->setFontSize(35);
						num->setString(to_string(i + 1));
						num->setPosition(Vec2(100, h - 50 - 100 * i));
						_rankingListScrollView->addChild(num);

						string nameStr = _csvFile->getData(i, 0);
						auto name = cocos2d::ui::Text::create();
						name->setFontName(GAME_FONT_NAME_1);
						name->setFontSize(35);
						name->setString(nameStr);
						name->setPosition(Vec2(500, h - 50 - 100 * i));
						_rankingListScrollView->addChild(name);

						auto level = cocos2d::ui::Text::create();
						level->setFontName(GAME_FONT_NAME_2);
						level->setFontSize(35);
						level->setString(_csvFile->getData(i, _selectClos));
						level->setPosition(Vec2(960, h - 50 - 100 * i));
						_rankingListScrollView->addChild(level);

						if (nameStr.find(GAME_TEXT("本地 我：")) != nameStr.npos)
						{
							num->setColor(Color3B(0, 255, 255));
							name->setColor(Color3B(0, 255, 255));
							name->runAction(RepeatForever::create(Sequence::create(ScaleTo::create(0.5f, 1.08f), ScaleTo::create(0.5f, 0.92f), nullptr)));
							level->setColor(Color3B(0, 255, 255));
							level->runAction(RepeatForever::create(Sequence::create(ScaleTo::create(0.5f, 1.08f), ScaleTo::create(0.5f, 0.92f), nullptr)));

							auto own = cocos2d::ui::Text::create();
							own->setFontName(GAME_FONT_NAME_1);
							own->setFontSize(GAME_TEXT_SIZE("我的排名名称记录"));
							if (i + 1 >= row)
							{
								own->setString(StringUtils::format(GAME_TEXT("我的排名名未上榜").c_str(), _global->userInformation->getUserName().c_str(), onGetMaxLevel()));
							}
							else
							{
								own->setString(StringUtils::format(GAME_TEXT("我的排名名称记录").c_str(), i + 1, _global->userInformation->getUserName().c_str(), onGetMaxLevel()));
							}
							own->setPosition(Vec2(660, 57));
							own->setColor(Color3B(0, 255, 255));
							this->addChild(own);

							_draw->drawRect(Vec2(100, 22), Vec2(1220, 98), Color4F(0, 1, 1, 0.5f));
							_draw->drawSolidRect(Vec2(100, 22), Vec2(1220, 98), Color4F(0, 1, 1, 0.2f));
							draw->drawSolidRect(Vec2(0, h - 100 * i - 1), Vec2(1120, h - 100 * (i + 1) + 1), Color4F(0, 1, 1, 0.2f));
						}
						if (nameStr.find("网络 我：") != nameStr.npos)
						{
							num->setColor(Color3B(0, 255, 255));
							name->setColor(Color3B(0, 255, 255));
							name->runAction(RepeatForever::create(Sequence::create(ScaleTo::create(0.5f, 1.08f), ScaleTo::create(0.5f, 0.92f), nullptr)));
							level->setColor(Color3B(0, 255, 255));
							level->runAction(RepeatForever::create(Sequence::create(ScaleTo::create(0.5f, 1.08f), ScaleTo::create(0.5f, 0.92f), nullptr)));
							draw->drawSolidRect(Vec2(0, h - 100 * i - 1), Vec2(1120, h - 100 * (i + 1) + 1), Color4F(0, 1, 1, 0.2f));
						}
						if (i == 0)
						{
							num->setColor(Color3B::YELLOW);
							name->setColor(Color3B::YELLOW);
							name->enableGlow(Color4B(0, 255, 255, 255));
							level->setColor(Color3B::YELLOW);
							level->enableGlow(Color4B(0, 255, 255, 255));
							draw->drawSolidRect(Vec2(0, h - 100 * i - 1), Vec2(1120, h - 100 * (i + 1) + 1), Color4F(1, 1, 0, 0.3f));
						}
						else if (i == 1)
						{
							num->setColor(Color3B(192, 192, 192));
							num->enableGlow(Color4B::WHITE);
							name->setColor(Color3B(192, 192, 192));
							name->enableGlow(Color4B::WHITE);
							level->setColor(Color3B(192, 192, 192));
							level->enableGlow(Color4B::WHITE);
						}
						else if (i == 2)
						{
							num->setColor(Color3B(117, 60, 43));
							num->enableGlow(Color4B(187, 124, 104, 255));
							name->setColor(Color3B(117, 60, 43));
							name->enableGlow(Color4B(187, 124, 104, 255));
							level->setColor(Color3B(117, 60, 43));
							level->enableGlow(Color4B(187, 124, 104, 255));
						}
					}
				}), nullptr));
	}

	_rankingListScrollView->addChild(draw);

	onMouseEvent();
}

void IRankingListLayer::onMouseEvent()
{
	/* 鼠标滑动监听 */
	auto mouse = EventListenerMouse::create();
	mouse->onMouseScroll = [=](Event* event)
	{
		const auto mouseEvent = static_cast<EventMouse*>(event);
		const float movex = mouseEvent->getScrollY() * (300 / _rankingListScrollView->getInnerContainerSize().height) * 100;

		MouseEventControl::mouseScrollControlListener(_rankingListScrollView, movex, cocos2d::ui::ScrollView::Direction::VERTICAL);
	};
	Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(mouse, _rankingListScrollView);
}

void IRankingListLayer::onShowLoadingText()
{
	_loadingText = cocos2d::ui::Text::create();
	_loadingText->setFontName(GAME_FONT_NAME_1);
	_loadingText->setFontSize(GAME_TEXT_SIZE("排行榜加载中！"));
	_loadingText->setString(GAME_TEXT("排行榜加载中！"));
	_loadingText->setPosition(Vec2(660, 542));
	_loadingText->setColor(Color3B(0, 255, 255));
	this->addChild(_loadingText);
}

void IRankingListLayer::onShowUploadDataRequirement()
{
	auto draw = DrawNode::create();
	draw->drawRect(Vec2(1350, 1030), Vec2(1550, 980), Color4F(0, 1, 1, 1));
	this->addChild(draw);

	auto uploadTitle = cocos2d::ui::Text::create();
	uploadTitle->setFontName(GAME_FONT_NAME_1);
	uploadTitle->setFontSize(GAME_TEXT_SIZE("上传说明"));
	uploadTitle->setString(GAME_TEXT("上传说明"));
	uploadTitle->setPosition(Vec2(1450, 1002));
	uploadTitle->setColor(Color3B(0,255,255));
	this->addChild(uploadTitle);

	auto uploadReq = cocos2d::ui::Text::create();
	uploadReq->setFontName(GAME_FONT_NAME_1);
	uploadReq->setFontSize(GAME_TEXT_SIZE("上传说明详细"));
	uploadReq->setString(GAME_TEXT("上传说明详细"));
	uploadReq->setPosition(Vec2(1615,765));
	uploadReq->setTextAreaSize(Size(540, 410));
	uploadReq->setTextHorizontalAlignment(cocos2d::TextHAlignment::LEFT);
	uploadReq->setTextVerticalAlignment(cocos2d::TextVAlignment::CENTER);
	uploadReq->setColor(Color3B::RED);
	this->addChild(uploadReq);
}

void IRankingListLayer::onShowRankingListUpdateInformation()
{
	auto draw = DrawNode::create();
	draw->drawRect(Vec2(1350, 500), Vec2(1550, 450), Color4F(0, 1, 1, 1));
	this->addChild(draw);

	auto uploadTitle = cocos2d::ui::Text::create();
	uploadTitle->setFontName(GAME_FONT_NAME_1);
	uploadTitle->setFontSize(GAME_TEXT_SIZE("其他说明"));
	uploadTitle->setString(GAME_TEXT("其他说明"));
	uploadTitle->setPosition(Vec2(1450, 470));
	uploadTitle->setColor(Color3B(0, 255, 255));
	this->addChild(uploadTitle);

	auto uploadReq = cocos2d::ui::Text::create();
	uploadReq->setFontName(GAME_FONT_NAME_1);
	uploadReq->setFontSize(GAME_TEXT_SIZE("其他说明详细"));
	uploadReq->setString(GAME_TEXT("其他说明详细"));
	uploadReq->setPosition(Vec2(1615, 260));
	uploadReq->setTextAreaSize(Size(540, 410));
	uploadReq->setTextHorizontalAlignment(cocos2d::TextHAlignment::LEFT);
	uploadReq->setTextVerticalAlignment(cocos2d::TextVAlignment::CENTER);
	uploadReq->setColor(Color3B::WHITE);
	this->addChild(uploadReq);
}

void IRankingListLayer::onCheckUploadButtonEnable()
{
	if (!notUploadData)
	{
		_nowNettime = new MomentTime();
		_nowNettime->requestNetTime([this]()
			{
				if (_nowNettime->getNetDay() > 0 && _nowNettime->getNetMon() > 0 && _nowNettime->getNetYear() > 0)
				{
					auto recordMon = UserDefault::getInstance()->getIntegerForKey("RECORDMON");
					if (_nowNettime->getNetMon() != recordMon)
					{
						UserData::getInstance()->caveUserData(const_cast<char*>(_mostLevelName.c_str()), 0);
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

					auto mostUpload = UserData::getInstance()->openIntUserData(const_cast<char*>(_mostLevelName.c_str()));
					auto isIzeRecord = UserDefault::getInstance()->getBoolForKey(_isRecordName.c_str());
					if (!isIzeRecord && mostUpload < static_cast<int>(onGetMaxLevel())) // 没有上传过并且上传的最高记录小于当前记录
					{
						_uploadButton->setEnabled(true);
					}
				}
			}, true);
	}
}

void IRankingListLayer::onUploadData()
{
	_uploadButton->setEnabled(false);

	Application::getInstance()->sendLeveData(_global->userInformation->getUserName(), onGetMaxLevel(), 0);
	UserData::getInstance()->caveUserData(const_cast<char*>("IRANKINGLISTDATAUPLOAD"), static_cast<int>(onGetMaxLevel()));
	UserDefault::getInstance()->setIntegerForKey("RECORDDAY", _nowNettime->getNetDay());
	UserDefault::getInstance()->setIntegerForKey("RECORDMON", _nowNettime->getNetMon());
	UserDefault::getInstance()->setBoolForKey("ISIZERECORD", true);

	onSuccessfulFeedback();
}

void IRankingListLayer::onSuccessfulFeedback()
{
	auto layer = LayerColor::create(Color4B(40, 40, 40, 255));
	layer->setContentSize(Size(350, 100));
	layer->setPosition(Vec2(810, 465));
	layer->runAction(Sequence::create(DelayTime::create(3.f), CallFunc::create([layer]() {layer->removeFromParent(); }), nullptr));
	this->addChild(layer, 5);

	auto draw = DrawNode::create();
	draw->drawRect(Vec2(0, 0), Vec2(350, 100), Color4F(0, 1, 1, 1));
	layer->addChild(draw);

	auto text = cocos2d::ui::Text::create();
	text->setString(GAME_TEXT("数据上传成功！"));
	text->setFontName(GAME_FONT_NAME_1);
	text->setFontSize(GAME_TEXT_SIZE("数据上传成功！"));
	text->setPosition(Vec2(175, 50));
	layer->addChild(text);
}

void IRankingListLayer::onSelectCsvFile(int id)
{
	if (id == 0)
	{
		onParseCsvData();
	}
	else if (id == 1)
	{
		_director->getRunningScene()->addChild(TRankingListLayer::createLayer(), 10, "TRankingListLayer");
		deleteDialog();
	}
	else if (id == 2)
	{
		_director->getRunningScene()->addChild(HRankingListLayer::createLayer(), 10, "HRankingListLayer");
		deleteDialog();
	}
}
