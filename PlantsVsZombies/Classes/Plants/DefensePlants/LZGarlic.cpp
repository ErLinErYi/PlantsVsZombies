/**
 *Copyright (c) 2020 LZ.All Right Reserved
 *Author : LZ
 *Date: 2020.2.6
 *Email: 2117610943@qq.com
 */

#include "LZGarlic.h"

#include "Zombies/LZZombies.h"
#include "Scenes/GameScenes/Adventure/GameScene/LZAGSData.h"
#include "Scenes/GameScenes/Adventure/SelectPlantsScene/LZASPSSpriteLayer.h"
#include "Based/LZPlayMusic.h"

Garlic::Garlic(Node* node):
    _animationId(0)
{
	_node = node;
	_plantImage = nullptr;

	_healthPoint = 2000;
	_totalHealthPoint = 2000;
	_plantsType = PlantsType::Garlic;

	srand(time(nullptr));
}

Garlic::~Garlic()
{
}

Garlic* Garlic::create(Node* node)
{
	Garlic* garlic = new (std::nothrow) Garlic(node);
	if (garlic && garlic->init())
	{
		garlic->autorelease();
		return garlic;
	}
	CC_SAFE_DELETE(garlic);
	return nullptr;
}
Sprite* Garlic::createPlantImage()
{
	imageInit("Garlic", INIT);
	_plantImage->setScale(1.2f);
	_plantImage->setAnchorPoint(Vec2(0.4f, 0.45f));
	return _plantImage;
}

void Garlic::createPlantAnimation()
{
	_plantAnimation = plantInit("Garlic", "Garlic_Normal");
	_plantAnimation->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	_plantAnimation->setScale(0.8f);
	_node->addChild(_plantAnimation);

	// 影子
	setPlantShadow(3.0f);

	// 泥土飞溅动画
	setPlantSoilSplashAnimation(1.0f);
}

void Garlic::determineRelativePositionPlantsAndZombies()
{
	for (auto zombie : ZombiesGroup)
	{
		zombieEatPlant(zombie);      /* 僵尸吃植物 */
	}
}

void Garlic::zombieEatPlant(Zombies* zombie)
{
	if (getPlantIsSurvive() && getZombieIsSameLineWithPlant(zombie) && Plants::getZombieIsEncounterPlant(zombie)) /* 植物存活 && 植物与僵尸在同一行 && 僵尸遇到植物 */
	{
		if (zombie->getZombieIsSurvive() && !zombie->getZombieIsEat() && zombie->getZombieIsFrozen() != 2)
		{
			const string eateffect[3] = { "chomp","chomp2","chompsoft" };
			PlayMusic::playMusic(eateffect[rand() % 3]);

			zombie->setZombieEatPlantNumber(_plantNumber);
			zombie->setZombieStop();
			zombie->setZombieIsEat(true);
			zombie->setZombieIsEatGarlic(true);
			zombie->getZombieAnimation()->setAnimation(0, "Zombies_Eat", true);
			zombie->getZombieAnimation()->runAction(Sequence::create(DelayTime::create(1.0f),
				CallFunc::create([=]()
					{
						effectZombies(zombie);
					}), nullptr));
		}
	}
}

void Garlic::effectZombies(Zombies* zombie)
{
	if (zombie)
	{
		PlayMusic::playMusic(rand() % 2 ? "yuck" : "yuck2");
		reducePlantHealthPoint(100 - 30 * zombie->getZombieCurrentBloodProportionBloodPrecent());

		if (zombie->getZombieCurrentBloodVolume() > 0)
		{
			changeZombiePositionY(zombie);
			zombieRecoveryMove(zombie);
			zombie->getZombieAnimation()->setColor(Color3B(181, 230, 29));
			zombie->getZombieAnimation()->runAction(Sequence::create(DelayTime::create(0.5f),
				CallFunc::create([=]()
					{
						zombie->getZombieAnimation()->setColor(Color3B::WHITE);
					}), nullptr));
		}
	}
}

void Garlic::zombieRecoveryMove(Zombies* zombie)
{
	if (zombie->getZombieIsEat() && zombie->getZombieIsStop()) /* 僵尸正在吃植物 && 僵尸正在停止移动 */
	{
		zombie->setZombieIsEat(false);
		zombie->setZombieIsEatGarlic(false);
		if (!zombie->getZombieIsPlayDieAnimation()) /* 僵尸没有播放死亡动画 */
		{
			zombie->getZombieAnimation()->setMix("Zombies_Eat", Zombies::getZombieAniamtionName(zombie->getZombieType()), 0.5f);
			zombie->getZombieAnimation()->addAnimation(0, Zombies::getZombieAniamtionName(zombie->getZombieType()), true);
			zombie->setZombieCurrentSpeed(zombie->getZombieSpeed());
		}
	}
}

void Garlic::checkPlantHealthPoint()
{
	if (_healthPoint <= static_cast<int>(_totalHealthPoint * 2.f / 3.f) && _animationId == 0)
	{
		_animationId = 1;
		_plantAnimation->setAnimation(0, "Garlic_Damage", true);
	}
	if (_healthPoint <= static_cast<int>(_totalHealthPoint * 1.f / 3.f) && _animationId == 1)
	{
		_animationId = 2;
		_plantAnimation->setAnimation(0, "Garlic_Damage2", true);
	}
	if (_healthPoint <= 0)
	{
		_plantAnimation->setVisible(false);
	}
}

bool Garlic::getPlantIsSurvive() const
{
	return _healthPoint > 0 ? true : false;
}

void Garlic::changeZombiePositionY(Zombies* zombie)
{
	if (zombie->getZombieInRow() == 0)
	{
		zombie->getZombieAnimation()->runAction(MoveBy::create(0.5f, Vec2(-20, 138)));
		zombie->getZombieAnimation()->setLocalZOrder(zombie->getZombieAnimation()->getLocalZOrder() - 100);
		zombie->setZombieInRow(1);
	}
	else if (zombie->getZombieInRow() == 4)
	{
		zombie->getZombieAnimation()->runAction(MoveBy::create(0.5f, Vec2(-20, -138)));
		zombie->getZombieAnimation()->setLocalZOrder(zombie->getZombieAnimation()->getLocalZOrder() + 100);
		zombie->setZombieInRow(3);
	}
	else
	{
		auto number = rand() % 2;
		zombie->getZombieAnimation()->runAction(MoveBy::create(0.5f, Vec2(-20, number ? -138 : 138)));
		zombie->getZombieAnimation()->setLocalZOrder(zombie->getZombieAnimation()->getLocalZOrder() + (number ? 100 : -100));
		zombie->setZombieInRow(number ? zombie->getZombieInRow() - 1 : zombie->getZombieInRow() + 1);
	}
}

SkeletonAnimation* Garlic::showPlantAnimationAndText()
{
	auto& lta = _global->userInformation->getGameText();
	SPSSpriteLayer::plantCardTextScrollView->setInnerContainerSize(Size(lta.find("GARLIC_1")->second->position));

	_isLoop = true;
	_plantAnimation = plantInit("Garlic", "Garlic_Normal");
	_plantAnimation->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	_plantAnimation->setScale(1.5f);
	_plantAnimation->setPosition(Vec2(200, 610));

	SPSSpriteLayer::createPlantsText(0, lta.find("GARLIC_1")->second->text, Vec2(190, 910), lta.find("GARLIC_1")->second->fontsize);
	SPSSpriteLayer::createPlantsText(2, lta.find("GARLIC_2")->second->text, Vec2(360, 1000), lta.find("GARLIC_2")->second->fontsize, Color3B::YELLOW, false);
	SPSSpriteLayer::createPlantsText(3, lta.find("GARLIC_3")->second->text, Vec2(440, 1000), lta.find("GARLIC_3")->second->fontsize, Color3B::RED, false);
	SPSSpriteLayer::createPlantsText(1, lta.find("GARLIC_4")->second->text, Vec2(360, 870), lta.find("GARLIC_4")->second->fontsize, Color3B::ORANGE, false);
	
	return _plantAnimation;
}