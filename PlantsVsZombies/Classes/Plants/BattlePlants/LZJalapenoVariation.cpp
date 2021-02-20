/**
 *Copyright (c) 2021 LZ.All Right Reserved
 *Author : LZ
 *Date: 2021.1.24
 *Email: 2117610943@qq.com
 */

#include "LZJalapenoVariation.h"
#include "Based/LZPlayMusic.h"
#include "Scenes/GameScenes/Adventure/GameScene/LZAGSBackgroundLayer.h"
#include "Scenes/GameScenes/Adventure/GameScene/LZAGSDefine.h"
#include "Zombies/LZZombies.h"

JalapenoVariation::JalapenoVariation(Node* node)
{
	_node = node;
	_plantImage = nullptr;

	_healthPoint = 300;
	_isLoop = false;
	_combatEffecttiveness = 1800;
	_plantsType = PlantsType::JalapenoVariation;
}

JalapenoVariation::~JalapenoVariation()
{
}

JalapenoVariation* JalapenoVariation::create(Node* node)
{
	JalapenoVariation* jalapenoVariation = new (std::nothrow) JalapenoVariation(node);
	if (jalapenoVariation && jalapenoVariation->init())
	{
		jalapenoVariation->autorelease();
		return jalapenoVariation;
	}
	CC_SAFE_DELETE(jalapenoVariation);
	return nullptr;
}

Sprite* JalapenoVariation::createPlantImage()
{
	imageInit("JalapenoVariation", INIT);
	_plantImage->setScale(1.4f);
	_plantImage->setFlippedX(true);
	return _plantImage;
}

void JalapenoVariation::createPlantAnimation()
{
	_plantAnimation = plantInit("JalapenoVariation", "Jalapeno_Explode");
	_plantAnimation->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	_plantAnimation->setScaleX(-_plantAnimation->getScaleX());
	_plantAnimation->setTimeScale(1.5f);
	_node->addChild(_plantAnimation);

	// 影子
	setPlantShadow(1.8f);

	// 泥土飞溅动画
	setPlantSoilSplashAnimation(0.8f);

	// 事件监听
	createListener();
}

void JalapenoVariation::showExplodeAnimation()
{
	PlayMusic::playMusic("cherrybomb");

	GSBackgroundLayer::backgroundRunAction();

	for (int i = 4; i >= 0; --i)
	{
		auto jalapenoFire = SkeletonAnimation::createWithData(_global->userInformation->getAnimationData().find("Jalapeno_Fire")->second);
		jalapenoFire->setPosition(Vec2(_plantAnimation->getPositionX() + 5, GRASS_POSITION_BOTTOM + 138 * (i + 1) - 140));
		jalapenoFire->setAnimation(0, "animation", false);
		jalapenoFire->setLocalZOrder((50 - i) * 100 + 50 - i + 100);
		jalapenoFire->setScale(0.85f, 3.0f);
		jalapenoFire->runAction(Sequence::create(DelayTime::create(2),
			CallFunc::create([jalapenoFire]()
				{
					jalapenoFire->removeFromParent();
				}), nullptr));
		_node->addChild(jalapenoFire);
	}
}

bool JalapenoVariation::getZombieIsInExplodeRange(Zombies* zombie)
{
	return getZombieIsSameColumnWithPlant(zombie) ? true : false;
}

SkeletonAnimation* JalapenoVariation::showPlantAnimationAndText()
{
	auto& lta = _global->userInformation->getGameText();
	SPSSpriteLayer::plantCardTextScrollView->setInnerContainerSize(Size(lta.find("JALAPENO_6")->second->position));

	_isLoop = true;
	_plantAnimation = plantInit("JalapenoVariation", "Jalapeno_Normal");
	_plantAnimation->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	_plantAnimation->setScale(1.5f);
	_plantAnimation->setScaleX(-_plantAnimation->getScaleX());
	_plantAnimation->setPosition(Vec2(200, 610));

	SPSSpriteLayer::createPlantsText(0, lta.find("JALAPENO_6")->second->text, Vec2(190, 910), lta.find("JALAPENO_6")->second->fontsize);
	SPSSpriteLayer::createPlantsText(2, lta.find("JALAPENO_2")->second->text, Vec2(360, 1000), lta.find("JALAPENO_2")->second->fontsize, Color3B::YELLOW, false);
	SPSSpriteLayer::createPlantsText(3, lta.find("JALAPENO_7")->second->text, Vec2(440, 1000), lta.find("JALAPENO_7")->second->fontsize, Color3B::RED, false);
	SPSSpriteLayer::createPlantsText(1, SPSSpriteLayer::selectRequirementText(lta, PlantsType::JalapenoVariation, "JALAPENO_8", "JALAPENO_5"), Vec2(360, 870),
		lta.find("JALAPENO_8")->second->fontsize, SPSSpriteLayer::isPlantIsCanSelect[static_cast<unsigned int>(PlantsType::JalapenoVariation)] ? Color3B::ORANGE : Color3B(255, 70, 0), false);

	return _plantAnimation;
}
