/**
 *Copyright (c) 2021 LZ.All Right Reserved
 *Author : LZ
 *Date: 2021.2.3
 *Emal: 2117610943@qq.com
 */

#include "LZSunFlowerTwin.h"
#include "Scenes/GameScenes/Adventure/SelectPlantsScene/LZASPSSpriteLayer.h"
#include "Scenes/GameScenes/Adventure/GameScene/LZAGSData.h"
#include "Based/LZPlayMusic.h"

SunFlowerTwin::SunFlowerTwin(Node* node)
{
	_node = node;
	_plantImage = nullptr;
	_plantsType = PlantsType::SunFlowerTwin;
	_healthPoint = 300;
}

SunFlowerTwin::~SunFlowerTwin()
{
}

SunFlowerTwin* SunFlowerTwin::create(Node* node)
{
	SunFlowerTwin* sunFlowerTwin = new (std::nothrow) SunFlowerTwin(node);
	if (sunFlowerTwin && sunFlowerTwin->init())
	{
		sunFlowerTwin->autorelease();
		return sunFlowerTwin;
	}
	CC_SAFE_DELETE(sunFlowerTwin);
	return nullptr;
}

Sprite* SunFlowerTwin::createPlantImage()
{
	imageInit("SunFlowerTwin", INIT);
	_plantImage->setScale(1.5f);
	_plantImage->setAnchorPoint(Vec2(0.5f, 0.55f));
	return _plantImage;
}

void SunFlowerTwin::createPlantAnimation()
{
	_plantAnimation = plantInit("SunFlowerTwin", "SunFlowerTwin_Normal");
	_plantAnimation->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	_plantAnimation->setScale(1.2f);
	_node->addChild(_plantAnimation);

	// 影子
	setPlantShadow(1.5f);
	_plantAnimation->getChildByName("plantshadow")->setPosition(Vec2(0, 5));

	// 泥土飞溅动画
	setPlantSoilSplashAnimation(1.0f);

	// 创建监听
	createListener();
}

void SunFlowerTwin::createListener()
{
	calculateSunShowTime();

	playAnimation();

	_plantAnimation->setEventListener([=](spTrackEntry* entry, spEvent* event)
		{
			if (strcmp(event->data->name, "skill") == 0)
			{
				!_isCreateSun ? createSuns() : nullptr;
			}
		});
}

void SunFlowerTwin::playAnimation()
{
	auto delaytime = DelayTime::create(_sunShowTime.x);
	auto delaytime1 = DelayTime::create(_sunShowTime.y);
	auto callfunc = CallFunc::create([=]() 
		{ 
			_plantAnimation->addAnimation(0, "SunFlowerTwin_Skill", false); 
			_plantAnimation->addAnimation(0, "SunFlowerTwin_Normal", true);
		});
	auto callfunc2 = CallFunc::create([=](){	_isCreateSun = false;});

	_plantAnimation->runAction(RepeatForever::create(Sequence::create(delaytime, callfunc, delaytime1, callfunc2, nullptr)));
}

void SunFlowerTwin::createSuns()
{
	PlayMusic::playMusic("sunFlowerTwinSkill");
	PlayMusic::playMusic(rand() % 2 == 0 ? "throw" : "throw2");
	
	Vec2 pos[2]{ Vec2(-30,30),Vec2(30,30) };
	for (int i = 0; i < 2; ++i)
	{
		_sun = new Sun(goodsLayerInformation);
		_sun->setSunTag(++_sunTag);
		_sun->setPosition(_plantAnimation->getPosition() + pos[i]);
		_sun->createSuns(1.f);

		SunsGroup.push_back(_sun);
	}

	_isCreateSun = true;
}

SkeletonAnimation* SunFlowerTwin::showPlantAnimationAndText()
{
	auto& lta = _global->userInformation->getGameText();
	SPSSpriteLayer::plantCardTextScrollView->setInnerContainerSize(Size(lta.find("SUNFLOWERTWIN_1")->second->position));

	_isLoop = true;
	_plantAnimation = plantInit("SunFlowerTwin", "SunFlowerTwin_Normal");
	_plantAnimation->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	_plantAnimation->setScale(2.2f);
	_plantAnimation->setPosition(Vec2(200, 610));

	SPSSpriteLayer::createPlantsText(0, lta.find("SUNFLOWERTWIN_1")->second->text, Vec2(190, 910), lta.find("SUNFLOWERTWIN_1")->second->fontsize);
	SPSSpriteLayer::createPlantsText(2, lta.find("SUNFLOWERTWIN_2")->second->text, Vec2(360, 1000), lta.find("SUNFLOWERTWIN_2")->second->fontsize, Color3B::YELLOW, false);
	SPSSpriteLayer::createPlantsText(3, lta.find("SUNFLOWERTWIN_3")->second->text, Vec2(440, 1000), lta.find("SUNFLOWERTWIN_3")->second->fontsize, Color3B::RED, false);
	SPSSpriteLayer::createPlantsText(1, SPSSpriteLayer::selectRequirementText(lta, PlantsType::SunFlowerTwin, "SUNFLOWERTWIN_4", "SUNFLOWERTWIN_5"), Vec2(360, 870),
		lta.find("SUNFLOWERTWIN_5")->second->fontsize, SPSSpriteLayer::isPlantIsCanSelect[static_cast<unsigned int>(PlantsType::SunFlowerTwin)] ? Color3B::ORANGE : Color3B(255, 70, 0), false);

	return _plantAnimation;
}
