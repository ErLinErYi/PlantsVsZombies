/**
 *Copyright (c) 2021 LZ.All Right Reserved
 *Author : LZ
 *Date: 2021.6.17
 *Email: 2117610943@qq.com
 */

#include "LZPumpkin.h"
#include "Zombies/LZZombies.h"
#include "Scenes/GameScenes/Adventure/SelectPlantsScene/LZASPSSpriteLayer.h"
#include "Scenes/GameScenes/Adventure/GameScene/LZAGSData.h"
#include "Scenes/GameScenes/Adventure/GameScene/LZAGSControlLayer.h"

int Pumpkin::tagAddition = 10000;

Pumpkin::Pumpkin(Node* node) :
	_skinId(1),
	_setZombiesNoteat(false),
	_plantAnimationBack(nullptr)
{
	_node = node;
	_plantImage = nullptr;

	_healthPoint = 4000;
	_totalHealthPoint = 4000;
	_plantsType = PlantsType::Pumpkin;
}

Pumpkin::~Pumpkin()
{
}

Pumpkin* Pumpkin::create(Node* node)
{
	Pumpkin* pumpkin = new (std::nothrow) Pumpkin(node);
	if (pumpkin && pumpkin->init())
	{
		pumpkin->autorelease();
		return pumpkin;
	}
	CC_SAFE_DELETE(pumpkin);
	return nullptr;
}
Sprite* Pumpkin::createPlantImage()
{
	imageInit("Pumpkin", INIT);
	_plantImage->setScale(1.2f);
	_plantImage->setAnchorPoint(Vec2(0.5f, 0.95f));
	return _plantImage;
}

void Pumpkin::createPlantAnimation()
{
	_plantAnimationBack = plantInit("Pumpkin", "animation");
	_plantAnimationBack->setAttachment("skin", "Pumpkin_front_allstar2");
	_plantAnimationBack->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	_plantAnimationBack->setScale(1.2f);
	_plantAnimationBack->setTimeScale(1.f);
	_plantAnimationBack->setTag(0);
	_plantAnimationBack->setLocalZOrder(_plantAnimationBack->getLocalZOrder() - 10);
	_plantAnimationBack->setPosition(_plantAnimationBack->getPosition() - Vec2(0, 10));
	_node->addChild(_plantAnimationBack);

	_plantAnimation = plantInit("Pumpkin", "animation");
	_plantAnimation->setSkin("skin");
	_plantAnimation->setAttachment("bone", "Pumpkin_back2");
	_plantAnimation->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	_plantAnimation->setScale(1.2f);
	_plantAnimation->setTimeScale(1.f);
	_plantAnimation->setLocalZOrder(_plantAnimation->getLocalZOrder() + 1);
	_plantAnimation->setTag(_plantAnimation->getTag() + tagAddition);
	_plantAnimation->setPosition(_plantAnimation->getPosition() - Vec2(0, 10));
	_node->addChild(_plantAnimation);

	// 影子
	setPlantShadow(1.8f);

	// 泥土飞溅动画
	setPlantSoilSplashAnimation(1.0f);
}

void Pumpkin::zombieEatPlant(Zombies* zombie)
{
	if (getPlantIsSurvive() && Plants::getZombieIsSameLineWithPlant(zombie) &&       /* 植物存活 && 植物与僵尸在同一行 */
		getZombieIsEncounterPlant(zombie) && zombie->getZombieIsEatPlants())         /* 僵尸遇到植物 && 僵尸是吃植物的僵尸 */
	{
		setZombieEatPlantControl(zombie);
	}
}

bool Pumpkin::getZombieIsEncounterPlant(Zombies* zombie)
{
	return fabs(zombie->getZombiePositionX() - _plantAnimation->getPositionX()) <= 100;
}

void Pumpkin::setZombieEatPlantControl(Zombies* zombie)
{
	if (not _setZombiesNoteat && zombie->getZombieIsEat())
	{
		_setZombiesNoteat = true;
		zombie->setZombieIsEat(false);
		if (!zombie->getZombieIsPlayDieAnimation()) /* 僵尸没有播放死亡动画 */
		{
			zombie->getZombieAnimation()->setTrackEventListener(zombie->getZombieAnimation()->getCurrent(), nullptr);
		}
	}

	Plants::setZombieEatPlantControl(zombie);
}

void Pumpkin::checkPlantHealthPoint()
{
	if (_healthPoint <= _totalHealthPoint * 2.f / 3.f && _skinId == 1)
	{
		_plantAnimation->setSkin("skin2");
		_skinId = 2;
	}
	if (_healthPoint <= _totalHealthPoint / 3.f && _skinId == 2)
	{
		_plantAnimation->setSkin("skin3");
		_skinId = 3;
	}
	if (_healthPoint <= 0 && _skinId == 3)
	{
		_plantAnimationBack->removeFromParent();
		_skinId = 4;
	}
}

SkeletonAnimation* Pumpkin::showPlantAnimationAndText()
{
	auto& lta = _global->userInformation->getGameText();
	SPSSpriteLayer::plantCardTextScrollView->setInnerContainerSize(Size(lta.find("PUMPKIN_1")->second->position));

	_isLoop = true;
	_plantAnimation = plantInit("Pumpkin", "animation");
	_plantAnimation->setSkin("skin");
	_plantAnimation->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	_plantAnimation->setScale(2.f);
	_plantAnimation->setPosition(Vec2(200, 610));
	_plantAnimation->setTimeScale(1.f);

	SPSSpriteLayer::createPlantsText(0, lta.find("PUMPKIN_1")->second->text, Vec2(190, 910), lta.find("PUMPKIN_1")->second->fontsize);
	SPSSpriteLayer::createPlantsText(2, lta.find("PUMPKIN_2")->second->text, Vec2(360, 1000), lta.find("PUMPKIN_2")->second->fontsize, Color3B::YELLOW, false);
	SPSSpriteLayer::createPlantsText(3, lta.find("PUMPKIN_3")->second->text, Vec2(440, 1000), lta.find("PUMPKIN_3")->second->fontsize, Color3B::RED, false);
	SPSSpriteLayer::createPlantsText(1, SPSSpriteLayer::selectRequirementText(lta, PlantsType::Pumpkin, "PUMPKIN_4", "PUMPKIN_5"), Vec2(360, 870),
		lta.find("PUMPKIN_5")->second->fontsize, SPSSpriteLayer::isPlantIsCanSelect[static_cast<unsigned int>(PlantsType::Pumpkin)] ? Color3B::ORANGE : Color3B(255, 70, 0), false);

	return _plantAnimation;
}

void Pumpkin::readPlantInforamtion(rapidjson::Document* levelDataDocument, char* key, int i)
{
	controlLayerInformation->gameMapInformation->plantPumpkin[static_cast<int>(_rowAndColumn.y)][static_cast<int>(_rowAndColumn.x)] = true;
}

void Pumpkin::setPlantOtherInformation(rapidjson::Document* levelDataDocument, char* key, int i)
{
	_plantAnimationBack->setAnimation(0, "animation", true);
	_plantAnimationBack->setPosition(_plantAnimationBack->getPosition() + Vec2(0, 10));
    _plantAnimationBack->setTimeScale(1.f);

	_plantAnimation->setAnimation(0, "animation", true);
	_plantAnimation->setPosition(_plantAnimation->getPosition() + Vec2(0, 10));
	_plantAnimation->setTimeScale(1.f);
}