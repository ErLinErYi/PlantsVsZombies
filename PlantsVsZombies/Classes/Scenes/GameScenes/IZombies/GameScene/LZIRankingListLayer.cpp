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

IRankingListLayer::IRankingListLayer():
	_rankingListScrollView(nullptr),
	_draw(nullptr),
	_loadingText(nullptr)
{
	_downloader.reset(new network::Downloader());
}

IRankingListLayer::~IRankingListLayer()
{
}

bool IRankingListLayer::init()
{
	if (!LayerColor::initWithColor(Color4B(0, 0, 0, 180)))return false;

	createShieldLayer(this);
	onCreateScrollView();
	onShowTitle();
	onShowLoadingText();
	onDownloadRankingList();
	onShowBackButton();

	return true;
}

void IRankingListLayer::onShowTitle()
{
	_draw = DrawNode::create();
	_draw->drawSolidRect(Vec2(20, 20), Vec2(1900, 1060), Color4F(0.2f, 0.2f, 0.2f, 0.7f));
	_draw->drawLine(Vec2(400, 900), Vec2(1520, 900), Color4F(0.5f, 0.5f, 0.5f, 0.7f));
	_draw->drawLine(Vec2(400, 800), Vec2(1520, 800), Color4F(0.5f, 0.5f, 0.5f, 0.7f));

	_draw->drawLine(Vec2(400, 900), Vec2(400, 100), Color4F(0.5f, 0.5f, 0.5f, 0.7f));
	_draw->drawLine(Vec2(600, 900), Vec2(600, 100), Color4F(0.5f, 0.5f, 0.5f, 0.7f));
	_draw->drawLine(Vec2(1200, 900), Vec2(1200, 100), Color4F(0.5f, 0.5f, 0.5f, 0.7f));
	_draw->drawLine(Vec2(1520, 900), Vec2(1520, 100), Color4F(0.5f, 0.5f, 0.5f, 0.7f));
	this->addChild(_draw);

	auto title = ui::Text::create();
	title->setFontName(GAME_FONT_NAME_1);
	title->setFontSize(50);
	title->setString("“我是僵尸模式” 玩家闯关记录排行榜");
	title->setPosition(Vec2(960, 980));
	this->addChild(title);

	string str[3] = { "排名","玩家名称","最高记录" };
	Vec2 pos[3] = { Vec2(500, 850),Vec2(900, 850),Vec2(1360, 850) };
	for (int i = 0; i < 3; ++i)
	{
		auto ranking = ui::Text::create();
		ranking->setFontName(GAME_FONT_NAME_1);
		ranking->setFontSize(35);
		ranking->setString(str[i]);
		ranking->setPosition(pos[i]);
		ranking->setColor(Color3B(0, 255, 255));
		this->addChild(ranking);
	}
}

void IRankingListLayer::onShowBackButton()
{
	auto button = Button::create("SeedChooser_Button2.png", "SeedChooser_Button2_Glow.png", "", cocos2d::ui::Widget::TextureResType::PLIST);
	button->setTitleText(GAME_TEXT("退出"));
	button->setTitleColor(Color3B(0, 255, 255));
	button->setTitleFontSize(GAME_TEXT_SIZE("退出"));
	button->setScale(2.0f);
	button->setPosition(Vec2(1785, 50));
	this->addChild(button);
	button->addTouchEventListener([&](Ref* sender, Widget::TouchEventType type)
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
	_rankingListScrollView->setPosition(Vec2(960.f, 450.f));
	_rankingListScrollView->setBounceEnabled(true);
	_rankingListScrollView->setScrollBarPositionFromCorner(Vec2(-10, 0));
	_rankingListScrollView->setScrollBarWidth(10);
	_rankingListScrollView->setScrollBarColor(Color3B(200,200,200));
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
		_loadingText->setString("排行榜加载失败！");
		_loadingText->setColor(Color3B::RED);
	};
}

void IRankingListLayer::onParseCsvData()
{
	auto csv = new CSVFile();
	csv->openFile(_strRankingList);

	vector<string> data;
	data.push_back(_global->userInformation->getUserName()+"--( 我 )");
	data.push_back(to_string(IControlLayer::mostLevelNumber));
	csv->addNewData(data);
	csv->sortData(1);

	auto row = csv->getRows();
	auto draw = DrawNode::create();

	_rankingListScrollView->setInnerContainerSize(Size(1120.f, 100 * row));

	auto h = 100 * row;
	for (int i = 0; i <= row; ++i)
	{
		draw->drawLine(Vec2(0, h - 100 * i), Vec2(1120, h - 100 * i), Color4F(0.5f, 0.5f, 0.5f, 0.7f));

		auto num = ui::Text::create();
		num->setFontName(GAME_FONT_NAME_1);
		num->setFontSize(35);
		num->setString(to_string(i + 1));
		num->setPosition(Vec2(100, h - 50 - 100 * i));
		_rankingListScrollView->addChild(num);
	}

	_rankingListScrollView->addChild(draw);

	for (int i = 0; i < row; ++i)
	{
		string nameStr = csv->getData(i, 0);
		auto name = ui::Text::create();
		name->setFontName(GAME_FONT_NAME_1);
		name->setFontSize(35);
		name->setString(nameStr);
		name->setPosition(Vec2(500, h - 50 - 100 * i));
		_rankingListScrollView->addChild(name);

		auto level = ui::Text::create();
		level->setFontName(GAME_FONT_NAME_1);
		level->setFontSize(35);
		level->setString(csv->getData(i, 1));
		level->setPosition(Vec2(960, h - 50 - 100 * i));
		_rankingListScrollView->addChild(level);

		if (nameStr.rfind("--( 我 )") != nameStr.npos)
		{
			name->setColor(Color3B::ORANGE);
			level->setColor(Color3B::ORANGE);
		}
	}

	delete csv;

	onMouseEvent();
}

void IRankingListLayer::onMouseEvent()
{
	/* 鼠标滑动监听 */
	auto mouse = EventListenerMouse::create();
	mouse->onMouseScroll = [=](Event* event)
	{
		const auto mouseEvent = static_cast<EventMouse*>(event);
		const float movex = mouseEvent->getScrollY() * (1400.f / _rankingListScrollView->getContentSize().height);

		MouseEventControl::mouseScrollControlListener(_rankingListScrollView, movex, ui::ScrollView::Direction::VERTICAL);
	};
	Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(mouse, _rankingListScrollView);
}

void IRankingListLayer::onShowLoadingText()
{
	_loadingText = ui::Text::create();
	_loadingText->setFontName(GAME_FONT_NAME_1);
	_loadingText->setFontSize(35);
	_loadingText->setString("排行榜加载中！");
	_loadingText->setPosition(Vec2(960, 540));
	_loadingText->setColor(Color3B(0, 255, 255));
	this->addChild(_loadingText);
}
