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

bool IRankingListLayer::notUploadData = false;

IRankingListLayer::IRankingListLayer():
	_rankingListScrollView(nullptr),
	_draw(nullptr),
	_loadingText(nullptr),
	_uploadButton(nullptr),
    _nowNettime(nullptr),
	_csvFile(nullptr)
{
	_downloader.reset(new network::Downloader());
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
	onCheckUploadButtonEnable();
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

	auto title = ui::Text::create();
	title->setFontName(GAME_FONT_NAME_1);
	title->setFontSize(GAME_TEXT_SIZE("“我是僵尸模式” 玩家闯关记录排行榜"));
	title->setString(GAME_TEXT("“我是僵尸模式” 玩家闯关记录排行榜"));
	title->setPosition(Vec2(660, 980));
	this->addChild(title);

	string str[3] = { "排名","玩家名称","最高记录" };
	Vec2 pos[3] = { Vec2(200, 850),Vec2(600, 850),Vec2(1060, 850) };
	for (int i = 0; i < 3; ++i)
	{
		auto ranking = ui::Text::create();
		ranking->setFontName(GAME_FONT_NAME_1);
		ranking->setFontSize(GAME_TEXT_SIZE(str[i]));
		ranking->setString(GAME_TEXT(str[i]));
		ranking->setPosition(pos[i]);
		ranking->setColor(Color3B(0, 255, 255));
		this->addChild(ranking);
	}
}

void IRankingListLayer::onShowBackButton()
{
	_uploadButton = Button::create("SeedChooser_Button2.png", "SeedChooser_Button2_Glow.png", "", cocos2d::ui::Widget::TextureResType::PLIST);
	_uploadButton->setTitleText(GAME_TEXT("数据上传"));
	_uploadButton->setTitleColor(Color3B(0, 255, 255));
	_uploadButton->setTitleFontSize(GAME_TEXT_SIZE("退出"));
	_uploadButton->setScale(2.0f);
	_uploadButton->setPosition(Vec2(1440, 50));
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
	button2->setPosition(Vec2(1785, 50));
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
	_rankingListScrollView = ui::ScrollView::create();
	_rankingListScrollView->setDirection(ui::ScrollView::Direction::VERTICAL);
	_rankingListScrollView->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	_rankingListScrollView->setContentSize(Size(1120.f, 700.f));
	_rankingListScrollView->setPosition(Vec2(660.f, 450.f));
	_rankingListScrollView->setBounceEnabled(true);
	_rankingListScrollView->setScrollBarPositionFromCorner(Vec2(2, 0));
	_rankingListScrollView->setScrollBarWidth(10);
	_rankingListScrollView->setScrollBarColor(Color3B(200, 200, 200));
	this->addChild(_rankingListScrollView, 1);
}

void IRankingListLayer::onDownloadRankingList()
{
	const string sURLList = "https://gitee.com/GITLZ/PVZDownLoader/raw/master/csv.csv";
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

void IRankingListLayer::onParseCsvData()
{
	_csvFile = new CSVFile();
	_csvFile->openFile(_strRankingList);

	vector<string> data;
	data.push_back(GAME_TEXT("(我)--> ") + _global->userInformation->getUserName());
	data.push_back(to_string(IControlLayer::mostLevelNumber));
	_csvFile->addNewData(data);
	_csvFile->sortData(1);

	auto row = _csvFile->getRows();
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
						auto num = ui::Text::create();
						num->setFontName(GAME_FONT_NAME_1);
						num->setFontSize(35);
						num->setString(to_string(i + 1));
						num->setPosition(Vec2(100, h - 50 - 100 * i));
						_rankingListScrollView->addChild(num);

						string nameStr = _csvFile->getData(i, 0);
						auto name = ui::Text::create();
						name->setFontName(GAME_FONT_NAME_1);
						name->setFontSize(35);
						name->setString(nameStr);
						name->setPosition(Vec2(500, h - 50 - 100 * i));
						_rankingListScrollView->addChild(name);

						auto level = ui::Text::create();
						level->setFontName(GAME_FONT_NAME_1);
						level->setFontSize(35);
						level->setString(_csvFile->getData(i, 1));
						level->setPosition(Vec2(960, h - 50 - 100 * i));
						_rankingListScrollView->addChild(level);

						if (nameStr.find(GAME_TEXT("(我)--> ")) != nameStr.npos)
						{
							name->setColor(Color3B(0, 255, 255));
							name->runAction(RepeatForever::create(Sequence::create(ScaleTo::create(0.5f, 1.08f), ScaleTo::create(0.5f, 0.92f), nullptr)));
							level->setColor(Color3B(0, 255, 255));
							level->runAction(RepeatForever::create(Sequence::create(ScaleTo::create(0.5f, 1.08f), ScaleTo::create(0.5f, 0.92f), nullptr)));

							auto own = ui::Text::create();
							own->setFontName(GAME_FONT_NAME_1);
							own->setFontSize(GAME_TEXT_SIZE("我的当前排名在排行榜的第"));
							own->setString(StringUtils::format(GAME_TEXT("我的当前排名在排行榜的第").c_str(), i + 1));
							own->setPosition(Vec2(660, 57));
							own->setColor(Color3B(0, 255, 255));
							this->addChild(own);
						}
						if (i == 0)
						{
							name->setColor(Color3B::ORANGE);
							name->enableGlow(Color4B(0, 255, 255, 255));
							level->setColor(Color3B::ORANGE);
							level->enableGlow(Color4B(0, 255, 255, 255));
						}
						else if (i == 1)
						{
							name->setColor(Color3B(128, 0, 64));
							name->enableOutline(Color4B::BLUE);
							level->setColor(Color3B(128, 0, 64));
							level->enableOutline(Color4B::BLUE);
						}
						else if (i == 2)
						{
							name->setColor(Color3B::GREEN);
							level->setColor(Color3B::GREEN);
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

		MouseEventControl::mouseScrollControlListener(_rankingListScrollView, movex, ui::ScrollView::Direction::VERTICAL);
	};
	Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(mouse, _rankingListScrollView);
}

void IRankingListLayer::onShowLoadingText()
{
	_loadingText = ui::Text::create();
	_loadingText->setFontName(GAME_FONT_NAME_1);
	_loadingText->setFontSize(GAME_TEXT_SIZE("排行榜加载中！"));
	_loadingText->setString(GAME_TEXT("排行榜加载中！"));
	_loadingText->setPosition(Vec2(660, 542));
	_loadingText->setColor(Color3B(0, 255, 255));
	this->addChild(_loadingText);
}

void IRankingListLayer::onUploadData()
{
	_uploadButton->setEnabled(false);

	Application::getInstance()->sendLeveData(_global->userInformation->getUserName(), IControlLayer::mostLevelNumber);
	UserData::getInstance()->caveUserData(const_cast<char*>("IRANKINGLISTDATAUPLOAD"), static_cast<int>(IControlLayer::mostLevelNumber));
	UserData::getInstance()->caveUserData(const_cast<char*>("RECORDDAY"),_nowNettime->getNetDay());
	UserData::getInstance()->caveUserData(const_cast<char*>("RECORDMON"), _nowNettime->getNetMon());
}

void IRankingListLayer::onShowUploadDataRequirement()
{
	auto draw = DrawNode::create();
	draw->drawRect(Vec2(1350, 1030), Vec2(1550, 980), Color4F(0, 1, 1, 1));
	this->addChild(draw);

	auto uploadTitle = ui::Text::create();
	uploadTitle->setFontName(GAME_FONT_NAME_1);
	uploadTitle->setFontSize(GAME_TEXT_SIZE("上传说明"));
	uploadTitle->setString(GAME_TEXT("上传说明"));
	uploadTitle->setPosition(Vec2(1450, 1002));
	uploadTitle->setColor(Color3B(0,255,255));
	this->addChild(uploadTitle);

	auto uploadReq = ui::Text::create();
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

	auto uploadTitle = ui::Text::create();
	uploadTitle->setFontName(GAME_FONT_NAME_1);
	uploadTitle->setFontSize(GAME_TEXT_SIZE("其他说明"));
	uploadTitle->setString(GAME_TEXT("其他说明"));
	uploadTitle->setPosition(Vec2(1450, 470));
	uploadTitle->setColor(Color3B(0, 255, 255));
	this->addChild(uploadTitle);

	auto uploadReq = ui::Text::create();
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
				auto recordMon = UserData::getInstance()->openIntUserData(const_cast<char*>("RECORDMON"));
				if (_nowNettime->getNetMon() != recordMon)
				{
					UserData::getInstance()->caveUserData(const_cast<char*>("IRANKINGLISTDATAUPLOAD"), 0);
					UserData::getInstance()->caveUserData(const_cast<char*>("RECORDDAY"), 0);
					UserData::getInstance()->caveUserData(const_cast<char*>("RECORDMON"), 0);
				}

				auto mostUpload = UserData::getInstance()->openIntUserData(const_cast<char*>("IRANKINGLISTDATAUPLOAD"));
				auto recordDay = UserData::getInstance()->openIntUserData(const_cast<char*>("RECORDDAY"));
				if (_nowNettime->getNetDay() != recordDay && mostUpload < static_cast<int>(IControlLayer::mostLevelNumber)) // 不在同一天并且上传的最高记录小于当前记录
				{
					_uploadButton->setEnabled(true);
				}
			}, true);
	}
}
