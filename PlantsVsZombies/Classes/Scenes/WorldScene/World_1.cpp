/**
 *Copyright (c) 2019 LZ.All Right Reserved
 *Author : LZ
 *Date: 2019.8.18
 *Modify: 2020.1.28
 *Email: 2117610943@qq.com
 */

#include "World_1.h"
#include "Scenes/SelectPlantsScene/SPSBackGroundLayer.h"
#include "SelectWorldScene.h"
#include "Scenes/SelectPlantsScene/SelectPlantsScene.h"

World_1::World_1():
	_global(Global::getInstance()),
	_director(Director::getInstance()),
	_backgroundSize(_director->getWinSize()),
	_parallax(nullptr),
	_scrollView(nullptr),
	_level(0),
	_worldPosition(UserDefault::getInstance()->getIntegerForKey("WORLD_1_POSITION")),
	_worldRollingDistance(_worldPosition)
{
	srand(time(nullptr));

	/* 播放音乐 */
	_global->changeBgMusic("mainmusic2", true);
}

Scene* World_1::createScene()
{
	return World_1::create();
}

bool World_1::init()
{
	if (!Scene::init())
	{
		return false;
	}

	readWorldLevel();

	createBackground();

	createScrollView();

    createGoBack();

	createMouseListener();

	return true;
}

void  World_1::createBackground()
{
	auto background = Sprite::create(_global->userInformation->getImagePath().find("WorldBackground")->second);
	background->setPosition(_backgroundSize / 2.0f);
	this->addChild(background);

	auto rotate = Sprite::create(_global->userInformation->getImagePath().find("RunBackground")->second);
	rotate->setScale(3.0f);
	rotate->setPosition(_backgroundSize / 2.0f);
	this->addChild(rotate);
	rotate->runAction(RepeatForever::create(RotateBy::create(1.0f, -10)));

	auto World1_34 = Sprite::create(_global->userInformation->getImagePath().find("World1_34")->second);
	World1_34->setScale(0.6f);
	World1_34->setPosition(_backgroundSize / 2.0f);
	this->addChild(World1_34);

	auto World1_33 = Sprite::create(_global->userInformation->getImagePath().find("World1_33")->second);
	World1_33->setPosition(Vec2(800, 600));
	World1_34->addChild(World1_33);

	_parallax = ParallaxNode::create();

	auto WorldBackgroundBlack = Sprite::create(_global->userInformation->getImagePath().find("WorldBackgroundBlack")->second);
	auto WorldBackgroundBlack1 = Sprite::create(_global->userInformation->getImagePath().find("WorldBackgroundBlack")->second);
	WorldBackgroundBlack->setAnchorPoint(Vec2(0, 1));
	WorldBackgroundBlack1->setAnchorPoint(Vec2(0, 0));
	WorldBackgroundBlack1->setFlippedY(true);
	_parallax->addChild(WorldBackgroundBlack, 2, Vec2(0, 0), Vec2(0, 1140));
	_parallax->addChild(WorldBackgroundBlack1, 2, Vec2(0, 0), Vec2(0, -60));

	for (int i = 0; i < 8; i++)
	{
		switch (rand() % 2)
		{
		case 0:
		{
			auto World1_28 = Sprite::create(_global->userInformation->getImagePath().find("World1_28")->second);
			_parallax->addChild(World1_28, 1, Vec2(0.1f, 0), Vec2(700 * i, rand() % 400 + 300));
		}
		break;
		case 1:
		{
			auto World1_29 = Sprite::create(_global->userInformation->getImagePath().find("World1_29")->second);
			_parallax->addChild(World1_29, 1, Vec2(0.1f, 0), Vec2(700 * i, rand() % 400 + 300));
		}
		break;
		}
	}
}

void World_1::createScrollView()
{
	_scrollView = ui::ScrollView::create();
	_scrollView->setBounceEnabled(true);
	_scrollView->setDirection(ui::ScrollView::Direction::HORIZONTAL);
	_scrollView->setContentSize(Director::getInstance()->getWinSize());
	_scrollView->setInnerContainerSize(Size(33100, 1080));
	_scrollView->setPosition(Vec2(0, 0));
	_scrollView->jumpToPercentHorizontal(_worldPosition);
	this->addChild(_scrollView);

	_scrollView->addChild(_parallax);

	this->addScrollView();
}

void World_1::addScrollView()
{
	/* 创建背景物品 */
	const string name[5] = { {"World1_32"},{"World1_31"},{"World1_36"},{"World1_30"},{"World1_27"} };
	const string name1[2] = { {"World1_39"},{"World1_40"}};

	for (int i = 0; i < 10; i++)
	{
		const auto world_32 = createSprite(name[i % 5], Vec2(500 + 1000 * i, i % 2 == 0 ? 850 : 350), rand() % 4/ 10.0f + 0.3f, 1, 0.2f, true);
		world_32->runAction(RepeatForever::create(RotateBy::create(10, 30)));
	}

	for (int i = 0; i < 8; i++)
	{
		const auto world_32 = createSprite(name1[i % 2], Vec2(500 + 2000 * i, 180), rand() % 3 / 10.0f + 0.5f, 3, 0.7f, true);
		world_32->setAnchorPoint(Vec2(0.5f, 0.5f));
		if (i % 2 != 0)
		{
			world_32->runAction(RepeatForever::create(RotateBy::create(25, 30)));
		}
	}

	/* 创建线条 */
	auto draw = DrawNode::create();
	Vec2 BeginPoint[53] =
	{
		Vec2(575, 450) ,Vec2(1050, 580) ,Vec2(1370, 730) ,Vec2(1650, 510) ,Vec2(2050, 440) ,Vec2(2300, 370) ,Vec2(2650, 520) ,Vec2(2900,740),
		Vec2(3230, 560) ,Vec2(3550, 520) ,Vec2(3920, 580) ,Vec2(4250, 750) ,Vec2(4620, 660) ,Vec2(4960, 590) ,Vec2(5250, 460) ,Vec2(5620,620),
		Vec2(5950, 665) ,Vec2(6270, 770) ,Vec2(6600, 590) ,Vec2(6900, 490) ,Vec2(7200, 420) ,Vec2(7500, 520) ,Vec2(7810, 680) ,Vec2(8100, 790),
		Vec2(8400, 725) ,Vec2(8700, 600) ,Vec2(9000, 510) ,Vec2(9300, 450) ,Vec2(9580, 540) ,Vec2(9950, 670) ,Vec2(10300, 750) ,Vec2(10590, 690),
		Vec2(10900, 620) ,Vec2(11200, 510) ,Vec2(11500, 400) ,Vec2(11900, 500) ,Vec2(12100, 410) ,Vec2(12300, 500) ,Vec2(12700, 430) ,Vec2(13000, 520),
		Vec2(13300, 620) ,Vec2(13600, 730) ,Vec2(13900, 660) ,Vec2(14200, 570) ,Vec2(14500, 470) ,Vec2(14800, 370) ,Vec2(15130, 540) ,Vec2(15500, 680),
		Vec2(15800, 730) ,Vec2(16200, 680) ,Vec2(16520, 480) ,Vec2(16700, 390),Vec2(16900,480)
	};
	Vec2 EndPoint[53] =
	{
		Vec2(1000, 560) ,Vec2(1300, 700) ,Vec2(1600, 520) ,Vec2(2000, 440) ,Vec2(2250, 380) ,Vec2(2600, 510) ,Vec2(2900, 720) ,Vec2(3190,560),
		Vec2(3530, 520) ,Vec2(3900, 580) ,Vec2(4250, 760) ,Vec2(4600, 650) ,Vec2(4960, 590) ,Vec2(5250, 460) ,Vec2(5600, 620) ,Vec2(5950,670),
		Vec2(6270, 750) ,Vec2(6600, 590) ,Vec2(6900, 490) ,Vec2(7200, 440) ,Vec2(7500, 520) ,Vec2(7810, 680) ,Vec2(8070, 795) ,Vec2(8400, 720),
		Vec2(8700, 600) ,Vec2(9000, 510) ,Vec2(9300, 450) ,Vec2(9580, 540) ,Vec2(9950, 670) ,Vec2(10300, 750) ,Vec2(10590, 690) ,Vec2(10900, 620),
		Vec2(11200, 510) ,Vec2(11500, 400) ,Vec2(11900, 500) ,Vec2(12100, 410) ,Vec2(12300, 500) ,Vec2(12700, 430) ,Vec2(13000, 520) ,Vec2(13300, 620),
		Vec2(13600, 730) ,Vec2(13900, 660) ,Vec2(14200, 570) ,Vec2(14500, 470) ,Vec2(14800, 370) ,Vec2(15130, 540) ,Vec2(15500, 680) ,Vec2(15800, 730),
		Vec2(16200, 680) ,Vec2(16520, 480) ,Vec2(16700, 400) ,Vec2(16900, 480),Vec2(16950,490)
	};
	for (int i = 0; i < _global->userInformation->getUserSelectWorldData().at(0)->levels; i++)
	{
		draw->drawSegment(BeginPoint[i], EndPoint[i], 2, Color4F(0, 1, 1, 0.6f));
		draw->runAction(RepeatForever::create(Sequence::create(MoveBy::create(0.05f, Vec2(0.1f, 0.1f)), MoveBy::create(0.05f, Vec2(-0.1f, -0.1f)), nullptr)));
		auto LineAction = Sprite::create(_global->userInformation->getImagePath().find("LineAction")->second);
		LineAction->setPosition(EndPoint[i]);
		draw->addChild(LineAction);
		LineAction->runAction(RepeatForever::create(Sequence::create(MoveTo::create(1.0f, EndPoint[i]),
			CallFunc::create([=]()
				{
					LineAction->setPosition(BeginPoint[i]);
				}), nullptr)));
	}
	_parallax->addChild(draw, 3, Vec2(0.5f, 0), Vec2(0, 0));

	/* 创建关卡 */
	auto World1_38 = createSprite("World1_38", Vec2(10, _backgroundSize.height / 2.0f), 0.8f, 2, 0.5f, false);
	auto sprite = createSprite(World1_38, "begingame", Vec2(550, 450), 1.0f, 0);

	auto wgfd = Sprite::create(_global->userInformation->getImagePath().find("wgfd")->second);
	wgfd->setPosition(Vec2(80, 30));
	wgfd->setScale(0.7f);
	sprite->addChild(wgfd, -1);

	auto Level_1 = createSprite("World1_26", Vec2(900, 500), 0.6f, 2, 0.5f, true);
	createSprite(Level_1, "World1_4", Vec2(0, 280), 0.8f, 1, true);
	createSprite(Level_1, "World1_6", Vec2(100, 380), 0.8f, 0, true);
	createButton(Level_1, "begingame", Vec2(200, 300));

	auto Level_2 = createSprite("World1_26", Vec2(1200, 600), 0.8f, 2, 0.5f, true);
	createSprite(Level_2, "World1_10", Vec2(20, 280), 0.8f, 0, true);
	createSprite(Level_2, "World1_4", Vec2(0, 320), 0.8f, 1, true);
	createSprite(Level_2, "World1_16", Vec2(90, 500), 0.7f, 2, true);
	createButton(Level_2, "begingame", Vec2(180, 320));

	auto Level_3 = createSprite("World1_26", Vec2(1500, 450), 0.7f, 2, 0.5f, true);
	createSprite(Level_3, "World1_13", Vec2(20, 280), 0.9f, 0, true);
	createSprite(Level_3, "World1_8", Vec2(170, 360), 0.7f, 1, true);
	createButton(Level_3, "begingame", Vec2(180, 270));

	auto Level_4 = createSprite("World1_9", Vec2(1900, 350), 0.8f, 2, 0.5f, true);
	createSprite(Level_4, "World1_7", Vec2(0, 230), 0.8f, 0, true);
	createSprite(Level_4, "World1_4", Vec2(-50, 290), 0.8f, 1, true);
	createSprite(Level_4, "World1_18", Vec2(-50, 520), 0.7f, 2, true);
	createButton(Level_4, "begingame", Vec2(150, 300));

	auto Level_5 = createSprite("World1_9", Vec2(2200, 300), 0.6f, 2, 0.5f, true);
	createSprite(Level_5, "World1_10", Vec2(0, 250), 0.8f, 0, true);
	createSprite(Level_5, "World1_4", Vec2(0, 280), 0.7f, 1, true);
	createButton(Level_5, "begingame", Vec2(150, 300));

	auto Level_6 = createSprite("World1_26", Vec2(2500, 450), 0.7f, 2, 0.5f, true);
	createSprite(Level_6, "World1_13", Vec2(20, 280), 0.9f, 0, true);
	createSprite(Level_6, "World1_8", Vec2(170, 360), 0.7f, 1, true);
	createButton(Level_6, "begingame", Vec2(180, 280));

	auto Level_7 = createSprite("World1_9", Vec2(2800, 600), 0.8f, 2, 0.5f, true);
	createSprite(Level_7, "World1_10", Vec2(0, 280), 0.8f, 1, true);
	createSprite(Level_7, "World1_13", Vec2(-20, 320), 0.8f, 2, true);
	createSprite(Level_7, "World1_11", Vec2(40, 400), 0.7f, 0, true);
	createButton(Level_7, "begingame", Vec2(150, 330));

	auto Level_8 = createSprite("World1_26", Vec2(3100, 500), 0.6f, 2, 0.5f, true);
	createSprite(Level_8, "World1_13", Vec2(10, 280), 1.0f, 1, true);
	createSprite(Level_8, "World1_12", Vec2(40, 270), 0.9f, 2, true);
	createButton(Level_8, "begingame", Vec2(170, 300));

	auto Level_9 = createSprite("World1_26", Vec2(3400, 400), 0.8f, 2, 0.5f, true);
	createSprite(Level_9, "World1_10", Vec2(20, 270), 0.9f, 1, true);
	createSprite(Level_9, "World1_7", Vec2(20, 290), 0.9f, 2, true);
	createSprite(Level_9, "World1_13", Vec2(-10, 320), 1.1f, 3, true);
	createSprite(Level_9, "World1_23", Vec2(40, 340), 0.6f, 4, true);
	createButton(Level_9, "begingame", Vec2(200, 330));

	auto Level_10 = createSprite("World1_9", Vec2(3800, 500), 0.5f, 2, 0.5f, true);
	createSprite(Level_10, "World1_10", Vec2(-10, 270), 0.9f, 1, true);
	createSprite(Level_10, "World1_7", Vec2(0, 290), 0.9f, 2, true);
	createSprite(Level_10, "World1_4", Vec2(-30, 310), 0.9f, 3, true);
	createSprite(Level_10, "World1_8", Vec2(0, 370), 0.8f, 4, true);
	createButton(Level_10, "begingame", Vec2(200, 330));

	auto Level_11 = createSprite("World1_26", Vec2(4100, 650), 0.7f, 2, 0.5f, true);
	createSprite(Level_11, "World1_4", Vec2(-20, 290), 0.9f, 2, true);
	createSprite(Level_11, "World1_6", Vec2(70, 380), 0.8f, 1, true);
	createButton(Level_11, "begingame", Vec2(200, 320));

	auto Level_12 = createSprite("World1_9", Vec2(4500, 550), 0.65f, 2, 0.5f, true);
	createSprite(Level_12, "World1_10", Vec2(-20, 260), 0.9f, 1, true);
	createSprite(Level_12, "World1_13", Vec2(-60, 330), 1.2f, 2, true);
	createSprite(Level_12, "World1_12", Vec2(50, 300), 0.8f, 3, true);
	createButton(Level_12, "begingame", Vec2(180, 330));

	auto Level_13 = createSprite("World1_9", Vec2(4900, 500), 0.5f, 2, 0.5f, true);
	createSprite(Level_13, "World1_10", Vec2(-20, 280), 0.9f, 1, true);
	createSprite(Level_13, "World1_4", Vec2(-50, 310), 0.9f, 2, true);
	createButton(Level_13, "begingame", Vec2(180, 330));

	auto Level_14 = createSprite("World1_26", Vec2(5100, 350), 0.8f, 2, 0.5f, true);
	createSprite(Level_14, "World1_10", Vec2(70, 280), 0.9f, 1, true);
	createSprite(Level_14, "World1_13", Vec2(-30, 340), 1.2f, 2, true);
	createSprite(Level_14, "World1_18", Vec2(-30, 550), 0.7f, 3, true);
	createButton(Level_14, "begingame", Vec2(200, 330));

	auto Level_15 = createSprite("World1_26", Vec2(5500, 500), 0.7f, 2, 0.5f, true);
	createSprite(Level_15, "World1_10", Vec2(10, 280), 0.9f, 1, true);
	createSprite(Level_15, "World1_7", Vec2(-10, 330), 1.2f, 2, true);
	createSprite(Level_15, "World1_4", Vec2(-40, 360), 1.0f, 2, true);
	createButton(Level_15, "begingame", Vec2(200, 360));

	auto Level_16 = createSprite("World1_9", Vec2(5900, 600), 0.6f, 2, 0.5f, true);
	createSprite(Level_16, "World1_6", Vec2(20, 340), 0.9f, 1, true);
	createSprite(Level_16, "World1_4", Vec2(-40, 260), 0.8f, 2, true);
	createButton(Level_16, "begingame", Vec2(160, 290));

	auto Level_17 = createSprite("World1_26", Vec2(6200, 700), 0.5f, 2, 0.5f, true);
	createSprite(Level_17, "World1_23", Vec2(60, 320), 0.6f, 2, true);
	createSprite(Level_17, "World1_4", Vec2(0, 260), 0.8f, 1, true);
	createButton(Level_17, "begingame", Vec2(180, 290));

	auto Level_18 = createSprite("World1_9", Vec2(6500, 500), 0.55f, 2, 0.5f, true);
	createSprite(Level_18, "World1_10", Vec2(-20, 270), 0.9f, 1, true);
	createSprite(Level_18, "World1_13", Vec2(-30, 330), 1.0f, 2, true);
	createSprite(Level_18, "World1_15", Vec2(140, 430), 1.0f, 3, true);
	createButton(Level_18, "begingame", Vec2(170, 330));

	auto Level_19 = createSprite("World1_14", Vec2(6800, 400), 0.6f, 2, 0.5f, true);
	createSprite(Level_19, "World1_5", Vec2(-50, 250), 1.2f, 1, true);
	createSprite(Level_19, "World1_24", Vec2(0, 360), 1.0f, 2, true);
	createButton(Level_19, "begingame", Vec2(230, 300));

	auto Level_20 = createSprite("World1_25", Vec2(7100, 350), 0.65f, 2, 0.5f, true);
	createSprite(Level_20, "World1_3", Vec2(-10, 280), 1.0f, 1, true);
	createButton(Level_20, "begingame", Vec2(210, 310));

	auto Level_21 = createSprite("World1_20", Vec2(7400, 450), 0.6f, 2, 0.5f, true);
	createSprite(Level_21, "World1_5", Vec2(-70, 210), 1.0f, 1, true);
	createSprite(Level_21, "World1_35", Vec2(30, 410), 1.0f, 2, true);
	createButton(Level_21, "begingame", Vec2(160, 260));

	auto Level_22 = createSprite("World1_14", Vec2(7700, 600), 0.5f, 2, 0.5f, true);
	createSprite(Level_22, "World1_3", Vec2(-10, 280), 1.0f, 1, true);
	createButton(Level_22, "begingame", Vec2(180, 280));

	auto Level_23 = createSprite("World1_14", Vec2(8000, 750), 0.4f, 2, 0.5f, true);
	createSprite(Level_23, "World1_19", Vec2(-10, 230), 1.0f, 1, true);
	createSprite(Level_23, "World1_37", Vec2(100, 380), 1.0f, 2, true);
	createButton(Level_23, "begingame", Vec2(190, 260));

	auto Level_24 = createSprite("World1_25", Vec2(8300, 650), 0.6f, 2, 0.5f, true);
	createSprite(Level_24, "World1_5", Vec2(-70, 250), 1.2f, 1, true);
	createButton(Level_24, "begingame", Vec2(210, 300));

	auto Level_25 = createSprite("World1_14", Vec2(8600, 550), 0.5f, 2, 0.5f, true);
	createSprite(Level_25, "World1_19", Vec2(-10, 230), 1.0f, 1, true);
	createButton(Level_25, "begingame", Vec2(190, 260));

	auto Level_26 = createSprite("World1_25", Vec2(8900, 450), 0.5f, 2, 0.5f, true);
	createSprite(Level_26, "World1_5", Vec2(-50, 250), 1.2f, 1, true);
	createSprite(Level_26, "World1_15", Vec2(120, 470), 1.0f, 3, true);
	createButton(Level_26, "begingame", Vec2(190, 300));

	auto Level_27 = createSprite("World1_20", Vec2(9200, 400), 0.6f, 2, 0.5f, true);
	createSprite(Level_27, "World1_3", Vec2(-70, 210), 1.0f, 1, true);
	createButton(Level_27, "begingame", Vec2(160, 250));

	auto Level_28 = createSprite("World1_14", Vec2(9500, 500), 0.4f, 2, 0.5f, true);
	createSprite(Level_28, "World1_19", Vec2(-10, 230), 1.0f, 1, true);
	createSprite(Level_28, "World1_37", Vec2(100, 380), 1.0f, 2, true);
	createButton(Level_28, "begingame", Vec2(200, 260));

	auto Level_29 = createSprite("World1_25", Vec2(9800, 600), 0.6f, 2, 0.5f, true);
	createSprite(Level_29, "World1_5", Vec2(-70, 250), 1.2f, 1, true);
	createSprite(Level_29, "World1_24", Vec2(0, 360), 1.0f, 2, true);
	createButton(Level_29, "begingame", Vec2(230, 300));

	auto Level_30 = createSprite("World1_14", Vec2(10200, 700), 0.5f, 2, 0.5f, true);
	createSprite(Level_30, "World1_3", Vec2(-30, 250), 1.0f, 1, true);
	createButton(Level_30, "begingame", Vec2(190, 260));

	auto Level_31 = createSprite("World1_14", Vec2(10500, 650), 0.4f, 2, 0.5f, true);
	createSprite(Level_31, "World1_19", Vec2(-10, 230), 1.0f, 1, true);
	createSprite(Level_31, "World1_17", Vec2(100, 380), 1.0f, 2, false);
	createButton(Level_31, "begingame", Vec2(200, 260));

	auto Level_32 = createSprite("World1_20", Vec2(10800, 550), 0.6f, 2, 0.5f, true);
	createSprite(Level_32, "World1_3", Vec2(-70, 210), 1.0f, 1, true);
	createSprite(Level_32, "World1_37", Vec2(100, 380), 1.0f, 2, true);
	createButton(Level_32, "begingame", Vec2(160, 250));

	auto Level_33 = createSprite("World1_25", Vec2(11100, 450), 0.5f, 2, 0.5f, true);
	createSprite(Level_33, "World1_5", Vec2(-70, 250), 1.2f, 1, true);
	createSprite(Level_33, "World1_21", Vec2(0, 300), 1.0f, 2, true);
	createSprite(Level_33, "World1_24", Vec2(170, 390), 1.0f, 3, true);
	createButton(Level_33, "begingame", Vec2(190, 300));

	auto Level_34 = createSprite("World1_14", Vec2(11400, 350), 0.4f, 2, 0.5f, true);
	createSprite(Level_34, "World1_5", Vec2(-70, 250), 1.2f, 1, true);
	createSprite(Level_34, "World1_35", Vec2(30, 430), 1.0f, 2, true);
	createButton(Level_34, "begingame", Vec2(200, 300));

	auto Level_35 = createSprite("World1_2", Vec2(11800, 500), 0.9f, 2, 0.5f, true);
	createButton(Level_35, "begingame", Vec2(124, 350));
	createButton(Level_35, "begingame", Vec2(345, 260));
	createButton(Level_35, "begingame", Vec2(574, 350));

	auto Level_38 = createSprite("World1_26", Vec2(12600, 350), 0.6f, 2, 0.5f, true);
	createSprite(Level_38, "World1_5", Vec2(-20, 260), 1.0f, 1, true);
	createButton(Level_38, "begingame", Vec2(190, 300));

	auto Level_39 = createSprite("World1_26", Vec2(12900, 450), 0.6f, 2, 0.5f, true);
	createSprite(Level_39, "World1_3", Vec2(0, 280), 1.0f, 1, true);
	createButton(Level_39, "begingame", Vec2(180, 290));

	auto Level_40 = createSprite("World1_9", Vec2(13200, 550), 0.6f, 2, 0.5f, true);
	createSprite(Level_40, "World1_5", Vec2(-70, 250), 1.0f, 1, false);
	createSprite(Level_40, "World1_21", Vec2(50, 300), 1.0f, 2, true);
	createButton(Level_40, "begingame", Vec2(170, 290));

	auto Level_41 = createSprite("World1_9", Vec2(13500, 650), 0.6f, 2, 0.5f, true);
	createSprite(Level_41, "World1_19", Vec2(-50, 250), 1.0f, 1, true);
	createButton(Level_41, "begingame", Vec2(170, 290));

	auto Level_42 = createSprite("World1_25", Vec2(13800, 600), 0.6f, 2, 0.5f, true);
	createSprite(Level_42, "World1_4", Vec2(-30, 260), 0.9f, 1, false);
	createSprite(Level_42, "World1_8", Vec2(0, 370), 0.8f, 4, true);
	createButton(Level_42, "begingame", Vec2(170, 290));

	auto Level_43 = createSprite("World1_25", Vec2(14100, 500), 0.6f, 2, 0.5f, true);
	createSprite(Level_43, "World1_13", Vec2(-50, 270), 1.2f, 1, true);
	createButton(Level_43, "begingame", Vec2(170, 280));

	auto Level_44 = createSprite("World1_14", Vec2(14400, 400), 0.6f, 2, 0.5f, true);
	createSprite(Level_44, "World1_13", Vec2(-50, 270), 1.2f, 1, true);
	createButton(Level_44, "begingame", Vec2(170, 280));

	auto Level_45 = createSprite("World1_20", Vec2(14700, 300), 0.6f, 2, 0.5f, true);
	createSprite(Level_45, "World1_13", Vec2(-50, 270), 1.2f, 1, true);
	createSprite(Level_45, "World1_12", Vec2(50, 300), 0.8f, 3, true);
	createButton(Level_45, "begingame", Vec2(170, 280));

	auto Level_46 = createSprite("World1_26", Vec2(15000, 450), 0.7f, 2, 0.5f, true);
	createSprite(Level_46, "World1_4", Vec2(-20, 290), 0.9f, 2, true);
	createSprite(Level_46, "World1_6", Vec2(70, 380), 0.8f, 1, true);
	createButton(Level_46, "begingame", Vec2(200, 320));

	auto Level_47 = createSprite("World1_26", Vec2(15300, 550), 0.8f, 2, 0.5f, true);
	createSprite(Level_47, "World1_10", Vec2(20, 270), 0.9f, 1, true);
	createSprite(Level_47, "World1_7", Vec2(20, 290), 0.9f, 2, true);
	createSprite(Level_47, "World1_13", Vec2(-10, 320), 1.1f, 3, true);
	createSprite(Level_47, "World1_23", Vec2(40, 340), 0.6f, 4, true);
	createButton(Level_47, "begingame", Vec2(200, 330));

	auto Level_48 = createSprite("World1_26", Vec2(15700, 650), 0.6f, 2, 0.5f, true);
	createSprite(Level_48, "World1_4", Vec2(0, 280), 0.8f, 1, true);
	createSprite(Level_48, "World1_6", Vec2(100, 380), 0.8f, 0, true);
	createButton(Level_48, "begingame", Vec2(200, 300));

	auto Level_49 = createSprite("World1_25", Vec2(16050, 600), 0.6f, 2, 0.5f, true);
	createSprite(Level_49, "World1_5", Vec2(-70, 250), 1.2f, 1, true);
	createSprite(Level_49, "World1_24", Vec2(0, 360), 1.0f, 2, true);
	createButton(Level_49, "begingame", Vec2(230, 300));

	auto Level_50 = createSprite("World1_2", Vec2(16400, 500), 0.9f, 2, 0.5f, true);
	createButton(Level_50, "begingame", Vec2(124, 350));
	createButton(Level_50, "begingame", Vec2(345, 260));
	createButton(Level_50, "begingame", Vec2(573, 350));
}

Sprite* World_1::createSprite(const std::string& name, const Vec2& position, const float& scale, const int& zorder, const float& speed, bool IsFlipped)
{
	auto sprite = Sprite::create(_global->userInformation->getImagePath().find(name)->second);
	sprite->setAnchorPoint(Vec2(0, 0.5f));
	sprite->setScale(scale);
	sprite->setPosition(position);
	IsFlipped == true ? sprite->setFlippedX(rand() % 2) : IsFlipped;
	_parallax->addChild(sprite, zorder, Vec2(speed, 0), position);
	return sprite;
}

Sprite* World_1::createSprite(Node* node, const std::string& name, const Vec2& position, const float& scale, const int& zorder, bool IsFlipped)
{
	auto sprite = Sprite::create(_global->userInformation->getImagePath().find(name)->second);
	sprite->setAnchorPoint(Vec2(0, 0.5f));
	sprite->setScale(scale);
	sprite->setPosition(position);
	IsFlipped == true ? sprite->setFlippedX(rand() % 2) : IsFlipped;
	node->addChild(sprite, zorder);
	return sprite;
}

void World_1::createMouseListener()
{
	auto mouse = EventListenerMouse::create();
	mouse->onMouseScroll = [&](Event* event)
	{
		auto mouseEvent = static_cast<EventMouse*>(event);
		float movex = mouseEvent->getScrollY() * 3;

		auto minOffset = 0.f;
		auto maxOffset = 100.f;

		auto offset = _scrollView->getScrolledPercentHorizontal();
		offset += movex;

		if (offset < minOffset)
		{
			offset = minOffset;
		}
		else if (offset > maxOffset)
		{
			offset = maxOffset;
		}
		_scrollView->scrollToPercentHorizontal(offset, 0.5f, true);
	};
	Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(mouse, _scrollView);
}

void World_1::createButton(Node* node, const std::string& name, const Vec2& position)
{
	auto sprite4 = ui::Button::create(_global->userInformation->getImagePath().find(name)->second);
	sprite4->setPosition(position);
	sprite4->setGlobalZOrder(1);
	node->addChild(sprite4);

	string buff = std::to_string(++_level);
	auto Text = ui::Text::create();
	Text->setFontName("resources/fonts/arial.ttf");
	Text->setFontSize(60);
	Text->setString(buff);
	Text->setGlobalZOrder(1);
	Text->setTextColor(Color4B::RED);
	Text->setPosition(Vec2(80, 70));
	sprite4->addChild(Text);

	if (_global->userInformation->getUserSelectWorldData().at(0)->levels < _level)
	{
		sprite4->setEnabled(false);
	}
	if (_global->userInformation->getUserSelectWorldData().at(0)->levels == _level)
	{
		auto wgfd = Sprite::create(_global->userInformation->getImagePath().find("wgfd")->second);
		wgfd->setPosition(Vec2(80, 30));
		wgfd->setScale(0.7f);
		wgfd->setGlobalZOrder(1);
		sprite4->addChild(wgfd, -1);

		wgfd->runAction(RepeatForever::create(Sequence::create(FadeOut::create(0.2f), FadeIn::create(0.2f), nullptr)));
		sprite4->runAction(RepeatForever::create(Sequence::create(TintTo::create(0.1f, Color3B(100, 100, 100)), TintTo::create(0.1f, Color3B(255, 255, 255)), nullptr)));
	}
	if (_global->userInformation->getUserSelectWorldData().at(0)->levels > _level)
	{
		auto wgfd = Sprite::create(_global->userInformation->getImagePath().find("wgfd")->second);
		wgfd->setPosition(Vec2(80, 30));
		wgfd->setScale(0.7f);
		wgfd->setGlobalZOrder(1);
		sprite4->addChild(wgfd, -1);

		sprite4->setColor(Color3B(0, 255, 255));
	}

	if (_global->userInformation->getUserSelectWorldData().at(0)->levels >= 52 && !_global->userInformation->getUserSelectWorldData().at(0)->isBeginShowEggs)
	{
		_global->userInformation->getUserSelectWorldData().at(0)->isBeginShowEggs = true;
		UserDefault::getInstance()->setBoolForKey("ISBEGINSHOWEGGS", true);
		UserDefault::getInstance()->flush();
	}

	createButtonListener(sprite4, _level);
}

void World_1::createButtonListener(ui::Button* button, const int& ID)
{
	char LevelName[15] = {};
	sprintf(LevelName, "Level_%d", ID);

	button->addTouchEventListener([=](Ref* sender, ui::Widget::TouchEventType type)
		{
			switch (type)
			{
			case ui::Widget::TouchEventType::BEGAN:
				AudioEngine::setVolume(AudioEngine::play2d(_global->userInformation->getMusicPath().find("tap")->second), _global->userInformation->getSoundEffectVolume());
				break;
			case ui::Widget::TouchEventType::ENDED:
				
				UserDefault::getInstance()->setFloatForKey("WORLD_1_POSITION", _scrollView->getScrolledPercentHorizontal()); /* 记录位置 */
				UserDefault::getInstance()->flush();

				//读取关卡信息
				OpenLevelData::getInstance()->createLevelData(ID, LevelName);
				OpenLevelData::getInstance()->setLevelNumber(ID);
				
				_global->userInformation->setCurrentPlayLevels(ID);
				_global->userInformation->setCurrentPlayWorldTag(0);
				_global->userInformation->setCurrentPlayWorldName(" - 现代世界 - ");

				Director::getInstance()->replaceScene(TransitionFade::create(1.0f, SelectPlantsScene::createScene()));

				break;
			}
		});
}

void World_1::readWorldLevel()
{
	/* 读取该世界关卡数据 */
	if (!_global->userInformation->getUserSelectWorldData().at(0)->isReadWoldInformation)
	{
		OpenLevelData::getInstance()->openLevelsData(_global->userInformation->getTextPath().find(_global->userInformation->getGameDifficulty() ? "GAMEWORLD_1DATAS_DIF" : "GAMEWORLD_1DATAS")->second);
		_global->userInformation->getUserSelectWorldData().at(0)->isReadWoldInformation = true;
	}

	_global->userInformation->getUserSelectWorldData().at(0)->levels =
		UserDefault::getInstance()->getIntegerForKey(
			_global->userInformation->getGameDifficulty() ?
			_global->userInformation->getSystemDifCaveFileName(_global->userInformation->getUserCaveFileNumber()).c_str() :
			_global->userInformation->getSystemCaveFileName(_global->userInformation->getUserCaveFileNumber()).c_str());
	
	if (_global->userInformation->getUserSelectWorldData().at(0)->levels == 0)
	{
		_global->userInformation->getUserSelectWorldData().at(0)->levels = 1;
	}
}

void World_1::createGoBack()
{
	auto back = ui::Button::create(_global->userInformation->getImagePath().find("back")->second, _global->userInformation->getImagePath().find("back1")->second);
	back->setScale(0.7f);
	back->setAnchorPoint(Vec2(0, 1));
	back->setPosition(Vec2(0, 1080));
	this->addChild(back);

	back->addTouchEventListener([=](Ref* sender, ui::Widget::TouchEventType type)
		{
			switch (type)
			{
			case ui::Widget::TouchEventType::BEGAN:
				AudioEngine::setVolume(AudioEngine::play2d(_global->userInformation->getMusicPath().find("gravebutton")->second), _global->userInformation->getSoundEffectVolume());
				break;
			case ui::Widget::TouchEventType::ENDED:

				UserDefault::getInstance()->setFloatForKey("WORLD_1_POSITION", _scrollView->getScrolledPercentHorizontal()); /* 记录位置 */
				UserDefault::getInstance()->flush();

				_global->userInformation->setMainToWorld(false);
				Director::getInstance()->replaceScene(TransitionFade::create(0.5f, SelectWorldScene::createScene()));
				break;
			}
		});
}