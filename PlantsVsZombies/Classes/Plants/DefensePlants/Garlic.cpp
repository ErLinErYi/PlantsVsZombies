/**
 *Copyright (c) 2020 LZ.All Right Reserved
 *Author : LZ
 *Date: 2020.2.6
 *Email: 2117610943@qq.com
 */

#include "Garlic.h"
#include "../EmissionPlants/Bullet/Bullet.h"

#include "Zombies/Zombies.h"
#include "Scenes/GameScene/GSData.h"

Garlic::Garlic(Node* node):
	_canEatNumbers(10)
,   _currentCanEatNumbers(10)
,   _animationId(0)
{
	_node = node;
	_plantImage = nullptr;

	_healthPoint = 2000;
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
	if (getPlantIsSurvive() && getZombieIsSameLineWithPlant(zombie) && getzombieIsEncounterPlant(zombie)) /* 植物存活 && 植物与僵尸在同一行 && 僵尸遇到植物 */
	{
		if (zombie->getZombieIsSurvive() && !zombie->getZombieIsEat())
		{
			const string eateffect[3] = { "chomp","chomp2","chompsoft" };
			zombie->setZombieEatPlantNumber(_plantNumber);
			zombie->setZombieStop();
			zombie->setZombieIsEat(true);
			zombie->getZombieAnimation()->setAnimation(1, "Zombies_Eat", true);

			--_currentCanEatNumbers;

			Bullet::playSoundEffect(eateffect[rand() % 3]);
			setPlantHurtBlink();

			auto zombies = zombie;
			_node->runAction(Sequence::create(DelayTime::create(1.0f),
				CallFunc::create([&, zombies]()
					{
						rand() % 2 == 0 ? Bullet::playSoundEffect("squash_hmm") : Bullet::playSoundEffect("squash_hmm2");
						changeZombiePositionY(zombies);

						_node->runAction(Sequence::create(DelayTime::create(0.5f),
							CallFunc::create([&, zombies]()
								{
									zombieRecoveryMove(zombies);
								}), nullptr));
					}), nullptr));
		}
	}
}

void Garlic::zombieRecoveryMove(Zombies* zombie)
{
	if (zombie->getZombieEatPlantNumber() == _plantNumber &&   /* 僵尸是吃的该植物 */
		zombie->getZombieIsEat() && zombie->getZombieIsStop()) /* 僵尸正在吃植物 && 僵尸正在停止移动 */
	{
		zombie->setZombieIsEat(false);
		if (!zombie->getZombieIsPlayDieAnimation()) /* 僵尸没有播放死亡动画 */
		{
			zombie->getZombieAnimation()->setMix("Zombies_Eat", Zombies::getZombieAniamtionName(zombie->getZombieType()), 1.5f);
			zombie->getZombieAnimation()->addAnimation(1, Zombies::getZombieAniamtionName(zombie->getZombieType()), true);
			zombie->setZombieCurrentSpeed(zombie->getZombieSpeed());
		}
	}
}

void Garlic::checkPlantHealthPoint()
{
	if (_currentCanEatNumbers <= static_cast<int>(_canEatNumbers * 2.f / 3.f) && _animationId == 0)
	{
		_animationId = 1;
		_plantAnimation->setAnimation(0, "Garlic_Damage", true);
	}
	if (_currentCanEatNumbers <= static_cast<int>(_canEatNumbers * 1.f / 3.f) && _animationId == 1)
	{
		_animationId = 2;
		_plantAnimation->setAnimation(0, "Garlic_Damage2", true);
	}
	if (_currentCanEatNumbers <= 0)
	{
		_plantAnimation->setVisible(false);
	}
}

bool Garlic::getPlantIsSurvive() const
{
	return _currentCanEatNumbers > 0 ? true : false;
}

bool Garlic::getzombieIsEncounterPlant(Zombies* zombie) const
{
	return fabs(zombie->getZombiePositionX() - _plantAnimation->getPositionX()) <= 50 ? true : false;
}

void Garlic::changeZombiePositionY(Zombies* zombie)
{
	if (zombie->getZombieAnimation()->getPositionY() == 130)
	{
		zombie->getZombieAnimation()->runAction(MoveBy::create(0.5f, Vec2(-20, 138)));
		zombie->getZombieAnimation()->setLocalZOrder(zombie->getZombieAnimation()->getLocalZOrder() - 20);
	}
	else if (zombie->getZombieAnimation()->getPositionY() == 682)
	{
		zombie->getZombieAnimation()->runAction(MoveBy::create(0.5f, Vec2(-20, -138)));
		zombie->getZombieAnimation()->setLocalZOrder(zombie->getZombieAnimation()->getLocalZOrder() + 20);
	}
	else
	{
		auto number = rand() % 2;
		zombie->getZombieAnimation()->runAction(MoveBy::create(0.5f, Vec2(-20, number ? -138 : 138)));
		zombie->getZombieAnimation()->setLocalZOrder(zombie->getZombieAnimation()->getLocalZOrder() + (number ? 20 : -20));
	}
}