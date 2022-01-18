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

TRankingListLayer::TRankingListLayer():
	_csvFile(nullptr),
	_mostLevel(max(UserData::getInstance()->openIntUserData(const_cast<char*>("TESTINGGROUND")), 1))
{
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
		Vec2 pos[3] = { Vec2(100, 950) ,Vec2(480, 950) ,Vec2(860, 950) };
		string str[3] = { "我是僵尸模式排行榜","植物试炼场排行榜","锤僵尸模式排行榜" };
		for (int i = 0; i < 3; ++i)
		{
			auto layerColor = LayerColor::create(Color4B(i == 1 ? 0 : 255, 255, 255, 51));
			layerColor->setContentSize(Size(360, 50));
			layerColor->setPosition(pos[i]);
			this->addChild(layerColor);

			auto draw = DrawNode::create();
			draw->drawRect(Vec2(0, 0), Vec2(360, 50), Color4F(i == 1 ? 0 : 1, 1, 1, 0.5f));
			layerColor->addChild(draw);

			auto button = Button::create();
			button->ignoreContentAdaptWithSize(false);
			button->setContentSize(Size(360, 50));
			button->setTitleText(str[i]);
			button->setTitleColor(Color3B(i == 1 ? 0 : 255, 255, 255));
			button->setTitleFontSize(30);
			button->setTitleFontName(GAME_FONT_NAME_1);
			button->setPosition(Vec2(180, 25));
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

void TRankingListLayer::onParseCsvData()
{
	if (!_csvFile)
	{
		_csvFile = new CSVFile();
		_csvFile->openFile(_strRankingList);

		vector<string> data;
		data.push_back(GAME_TEXT("本地 我：") + _global->userInformation->getUserName());
		data.push_back(to_string(_mostLevel));
		_csvFile->addNewData(data);
		_csvFile->sortData(1);
	}

	_rankingListScrollView->removeAllChildren();

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
						level->setString(_csvFile->getData(i, 1));
						level->setPosition(Vec2(960, h - 50 - 100 * i));
						_rankingListScrollView->addChild(level);

						if (nameStr.find(GAME_TEXT("本地 我：")) != nameStr.npos)
						{
							num->setColor(Color3B(0, 255, 255));
							name->setColor(Color3B(0, 255, 255));
							name->runAction(RepeatForever::create(Sequence::create(ScaleTo::create(0.5f, 1.08f), ScaleTo::create(0.5f, 0.92f), nullptr)));
							level->setColor(Color3B(0, 255, 255));
							level->runAction(RepeatForever::create(Sequence::create(ScaleTo::create(0.5f, 1.08f), ScaleTo::create(0.5f, 0.92f), nullptr)));

							if (!this->getChildByName("myList"))
							{
								auto own = cocos2d::ui::Text::create();
								own->setFontName(GAME_FONT_NAME_1);
								own->setFontSize(GAME_TEXT_SIZE("我的排名名称记录"));
								own->setString(StringUtils::format(GAME_TEXT("我的排名名称记录").c_str(), i + 1, _global->userInformation->getUserName().c_str(), _mostLevel));
								own->setPosition(Vec2(660, 57));
								own->setColor(Color3B(0, 255, 255));
								own->setName("myList");
								this->addChild(own);

								_draw->drawRect(Vec2(100, 22), Vec2(1220, 98), Color4F(0, 1, 1, 0.5f));
								_draw->drawSolidRect(Vec2(100, 22), Vec2(1220, 98), Color4F(0, 1, 1, 0.2f));
							}
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
				}

				auto recordDay = UserDefault::getInstance()->getIntegerForKey("RECORDDAY");
				if (_nowNettime->getNetDay() != recordDay)
				{
					UserDefault::getInstance()->setBoolForKey("ISIZERECORD", false);
					UserDefault::getInstance()->setBoolForKey("ISTGRECORD", false);
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
}
