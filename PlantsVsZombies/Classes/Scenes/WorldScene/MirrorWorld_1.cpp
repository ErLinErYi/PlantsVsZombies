/**
 *Copyright (c) 2019 LZ.All Right Reserved
 *Author : LZ
 *Date: 2020.4.17
 *Email: 2117610943@qq.com
 */

#include "MirrorWorld_1.h"
#include "SelectWorldScene.h"
#include "Based/LevelData.h"
#include "../SelectPlantsScene/MirrorSelectPlantsScene.h"

MirrorWorld_1::MirrorWorld_1()
{
    _worldPosition = UserDefault::getInstance()->getIntegerForKey("WORLD_2_POSITION");
}

Scene* MirrorWorld_1::createScene()
{
    return MirrorWorld_1::create();
}

void MirrorWorld_1::addScrollView()
{
	/* 创建背景物品 */
	const string name[5] = { {"World1_32"},{"World1_31"},{"World1_36"},{"World1_30"},{"World1_27"} };
	const string name1[2] = { {"World1_22"},{"World1_40"} };

	for (int i = 0; i < 10; i++)
	{
		const auto world_32 = createSprite(name[i % 5], Vec2(500 + 1000 * i, i % 2 == 0 ? 850 : 350), rand() % 4 / 10.0f + 1.5f, 1, 0.2f, true);
		world_32->runAction(RepeatForever::create(RotateBy::create(10, 30)));
	}

	for (int i = 0; i < 8; i++)
	{
		const auto world_32 = createSprite(name1[i % 2], Vec2(500 + 2000 * i, 180), rand() % 3 / 10.0f + 1.5f, 3, 0.7f, true);
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
	for (int i = 0; i < _global->userInformation->getUserSelectWorldData().at(1)->levels; ++i)
	{
		draw->drawSegment(BeginPoint[i], EndPoint[i], 2, Color4F(0, 1, 1, 0.6f));
		draw->runAction(RepeatForever::create(Sequence::create(MoveBy::create(0.05f, Vec2(0.1f, 0.1f)), MoveBy::create(0.05f, Vec2(-0.1f, -0.1f)), nullptr)));
		auto LineAction = Sprite::createWithSpriteFrameName("LineAction.png");
		LineAction->setPosition(EndPoint[i]);
		draw->addChild(LineAction);
		LineAction->runAction(RepeatForever::create(Sequence::create(MoveTo::create(1.0f, EndPoint[i]),
			CallFunc::create([=]()
				{
					LineAction->setPosition(BeginPoint[i]);
				}), nullptr)));
	}
	_parallax->addChild(draw, 3, Vec2(0.5f, 0), Vec2(0, 0));

    showLevels();
}

void MirrorWorld_1::readWorldLevel()
{
    /* 读取该世界关卡数据 */
    if (!_global->userInformation->getUserSelectWorldData().at(1)->isReadWoldInformation)
    {
        OpenLevelData::getInstance()->openLevelsData(
            _global->userInformation->getTextPath().find(
                _global->userInformation->getGameDifficulty() ?
                "GAMEWORLD_1DATAS_DIF" : "GAMEWORLD_1DATAS")->second);
        _global->userInformation->getUserSelectWorldData().at(1)->isReadWoldInformation = true;
    }

    char worldFile[128];
    if (_global->userInformation->getGameDifficulty())
    {
        snprintf(worldFile, 128, _global->userInformation->getSystemDifCaveFileName(
            _global->userInformation->getUserCaveFileNumber()).c_str(), 2);
    }
    else
    {
        snprintf(worldFile, 128, _global->userInformation->getSystemCaveFileName(
            _global->userInformation->getUserCaveFileNumber()).c_str(), 2);
    }
    _global->userInformation->getUserSelectWorldData().at(1)->levels =
        UserDefault::getInstance()->getIntegerForKey(worldFile);


    if (_global->userInformation->getUserSelectWorldData().at(1)->levels == 0)
    {
        _global->userInformation->getUserSelectWorldData().at(1)->levels = 1;
    }
}

ui::Button* MirrorWorld_1::createButton(Node* node, const std::string& name, const Vec2& position)
{
    auto sprite4 = ui::Button::create(name + ".png", "", "", TextureResType::PLIST);
    sprite4->setPosition(position);
    sprite4->setGlobalZOrder(1);
    sprite4->setScale(2.0f);
    node->addChild(sprite4);

    auto text = ui::Text::create();
    text->setFontName(GAME_FONT_NAME_2);
    text->setFontSize(30);
    text->setString(to_string(++_level));
    text->setGlobalZOrder(1);
    text->setTextColor(Color4B::RED);
    text->setPosition(Vec2(40, 35));
    setLevelVisible(text);
    sprite4->addChild(text);

    if (_global->userInformation->getUserSelectWorldData().at(1)->levels < _level)
    {
        sprite4->setEnabled(false);
    }
    if (_global->userInformation->getUserSelectWorldData().at(1)->levels == _level)
    {
        auto wgfd = Sprite::createWithSpriteFrameName("wgfd.png");
        wgfd->setPosition(Vec2(40, 15));
        wgfd->setScale(0.8f);
        wgfd->setGlobalZOrder(1);
        sprite4->addChild(wgfd, -1);

        wgfd->runAction(RepeatForever::create(Sequence::create(FadeOut::create(0.2f), FadeIn::create(0.2f), nullptr)));
        sprite4->runAction(RepeatForever::create(Sequence::create(TintTo::create(0.1f, Color3B(100, 100, 100)), TintTo::create(0.1f, Color3B(255, 255, 255)), nullptr)));
    }
    if (_global->userInformation->getUserSelectWorldData().at(1)->levels > _level)
    {
        auto wgfd = Sprite::createWithSpriteFrameName("wgfd.png");
        wgfd->setPosition(Vec2(40, 15));
        wgfd->setScale(0.8f);
        wgfd->setGlobalZOrder(1);
        sprite4->addChild(wgfd, -1);

        sprite4->setColor(Color3B(0, 255, 255));
    }

    if (_global->userInformation->getUserSelectWorldData().at(1)->levels >= 52 &&
        !_global->userInformation->getUserSelectWorldData().at(1)->isBeginShowEggs)
    {
        _global->userInformation->getUserSelectWorldData().at(1)->isBeginShowEggs = true;
        UserDefault::getInstance()->setBoolForKey("ISBEGINSHOWEGGS", true);
    }

    createButtonListener(sprite4, _level);

    return sprite4;
}

void MirrorWorld_1::createButtonListener(ui::Button* button, const int& ID)
{
    char LevelName[20] = {};
    snprintf(LevelName, 20, "Level_%d", ID);

    button->addTouchEventListener([=](Ref* sender, ui::Widget::TouchEventType type)
        {
            switch (type)
            {
            case ui::Widget::TouchEventType::BEGAN:
                AudioEngine::setVolume(AudioEngine::play2d(
                    _global->userInformation->getMusicPath().find("tap")->second), 
                    _global->userInformation->getSoundEffectVolume());
                break;
            case ui::Widget::TouchEventType::ENDED:

                UserDefault::getInstance()->setFloatForKey("WORLD_2_POSITION", _scrollView->getScrolledPercentHorizontal()); /* 记录位置 */

                //读取关卡信息
                OpenLevelData::getInstance()->createLevelData(ID, LevelName);
                OpenLevelData::getInstance()->setLevelNumber(ID);

                _global->userInformation->setCurrentPlayLevels(ID);
                _global->userInformation->setCurrentPlayWorldTag(1);
                _global->userInformation->setCurrentPlayWorldName(" - 镜像世界 - ");

                Director::getInstance()->replaceScene(TransitionFade::create(1.0f, MirrorSelectPlantsScene::createScene()));

                break;
            }
        });
}

void MirrorWorld_1::setLevelVisible(Node* node)
{
    if (_global->userInformation->getUserSelectWorldData().at(1)->levels < _level - 1)
    {
        node->setVisible(false);
    }
    if (_global->userInformation->getUserSelectWorldData().at(1)->levels == _level)
    {
        node->setOpacity(100);
        node->setCascadeOpacityEnabled(true);
    }
    if (_global->userInformation->getUserSelectWorldData().at(1)->levels == _level - 1)
    {
        node->setOpacity(30);
        node->setCascadeOpacityEnabled(true);
    }
}

void MirrorWorld_1::createGoBack()
{
    auto back = ui::Button::create("back.png", "back1.png", "", TextureResType::PLIST);
    back->setScale(0.7f);
    back->setAnchorPoint(Vec2(0, 1));
    back->setPosition(Vec2(0, 1080));
    this->addChild(back);

    back->addTouchEventListener([=](Ref* sender, ui::Widget::TouchEventType type)
        {
            switch (type)
            {
            case ui::Widget::TouchEventType::BEGAN:
                AudioEngine::setVolume(AudioEngine::play2d(
                    _global->userInformation->getMusicPath().find("gravebutton")->second),
                    _global->userInformation->getSoundEffectVolume());
                break;
            case ui::Widget::TouchEventType::ENDED:

                UserDefault::getInstance()->setFloatForKey("WORLD_2_POSITION", _scrollView->getScrolledPercentHorizontal()); /* 记录位置 */

                _global->userInformation->setMainToWorld(false);
                Director::getInstance()->replaceScene(TransitionFade::create(0.5f, SelectWorldScene::createScene()));
                break;
            }
        });
}
