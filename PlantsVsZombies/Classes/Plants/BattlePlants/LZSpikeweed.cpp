/**
 *Copyright (c) 2020 LZ.All Right Reserved
 *Author : LZ
 *Date: 2020.2.6
 *Email: 2117610943@qq.com
 */

#include "LZSpikeweed.h"

#include "../EmissionPlants/Bullet/LZBullet.h"
#include "Zombies/LZZombies.h"
#include "Scenes/GameScenes/Adventure/GameScene/LZAGSData.h"

Spikeweed::Spikeweed(Node* node)
{
	_node = node;
	_plantImage = nullptr;

	_healthPoint = 300;
	_combatEffecttiveness = 20;
	_plantsType = PlantsType::Spikeweed;

	_isChangeAnimation = false;
	_isHaveZombies = false;
	_isAttack = false;
}

Spikeweed::~Spikeweed()
{
}

Spikeweed* Spikeweed::create(Node* node)
{
	Spikeweed* spikeweed = new (std::nothrow) Spikeweed(node);
	if (spikeweed && spikeweed->init())
	{
		spikeweed->autorelease();
		return spikeweed;
	}
	CC_SAFE_DELETE(spikeweed);
	return nullptr;
}
Sprite* Spikeweed::createPlantImage()
{
	imageInit("Spikeweed", INIT);
	_plantImage->setScale(1.1f);
	_plantImage->setAnchorPoint(Vec2(0.5f, 1.f));
	return _plantImage;
}

void Spikeweed::createPlantAnimation()
{
	_plantAnimation = plantInit("Spikeweed", "Spikeweed_Normal");
	_plantAnimation->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	_plantAnimation->setScale(0.8f);
	_node->addChild(_plantAnimation);

	// 影子
	setPlantShadow(1.0f);

	// 泥土飞溅动画
	setPlantSoilSplashAnimation(0.8f);

	// 创建监听
	createListener();
}

void Spikeweed::createListener()
{
	_plantAnimation->setEventListener([this](spTrackEntry* entry, spEvent* event)
		{
			if (strcmp(event->data->name, "Attack") == 0)
			{
				_isAttack = true;
			}
		});
}

void Spikeweed::determineRelativePositionPlantsAndZombies()
{
	plantAttack();         /* 植物攻击 */

	// 僵尸对植物进行伤害？？？ 在此添加
	for (auto zombie : ZombiesGroup)
	{
		zombieEatPlant(zombie);      /* 僵尸吃植物 */

		zombieRecoveryMove(zombie);  /* 僵尸恢复移动 */
	}
}

void Spikeweed::plantAttack()
{
	for (auto zombie : ZombiesGroup)
	{
		if (zombie->getZombieIsSurvive() && getZombieIsSameLineWithPlant(zombie) && getZombieIsEncounterPlant(zombie))       /* 僵尸没有死亡 && 僵尸与植物在同一行 && 僵尸遇到植物 */
		{
			_isHaveZombies = true; /* 有僵尸标记 */

			if (!_isChangeAnimation)       /* 判断动画是否已经改变 */
			{
				_plantAnimation->addAnimation(0, "Spikeweed_Attack", true);
				_isChangeAnimation = true;
			}

			if (_isAttack)
			{
				Bullet::selectSoundEffect(zombie->getZombieBodyAttackSoundEffect(), zombie->getZombieHeadAttackSoundEffect());

				zombie->setZombieHurtBlink();

				hurtZombies(zombie);
			}
		}
	}

	plantRecovery("Spikeweed_Normal");
}

void Spikeweed::zombieEatPlant(Zombies* zombie)
{
	if (getPlantIsSurvive() && Plants::getZombieIsSameLineWithPlant(zombie) &&   /* 植物存活 && 植物与僵尸在同一行 */
		Plants::getZombieIsEncounterPlant(zombie) &&                             /* 僵尸遇到植物 */
		zombie->getZombieType() == ZombiesType::GargantuarZombies)               /* 僵尸是巨人僵尸 */
	{
		if (zombie->getZombieIsSurvive() && !zombie->getZombieIsEat() && zombie->getZombieIsFrozen() != 2)
		{
			zombie->setZombieEatPlantNumber(_plantNumber);
			zombie->setZombieStop();
			zombie->setZombieIsEat(true);

			zombieAttackPlant(zombie);
		}
	}
}

bool Spikeweed::getZombieIsEncounterPlant(Zombies* zombie)
{
	return fabs(zombie->getZombiePositionX() - _plantAnimation->getPositionX()) <= 110 ? true : false;
}

SkeletonAnimation* Spikeweed::showPlantAnimationAndText()
{
	auto& lta = _global->userInformation->getGameText();
	SPSSpriteLayer::plantCardTextScrollView->setInnerContainerSize(Size(lta.find("SPIKEWEED_1")->second->position));

	_isLoop = true;
	_plantAnimation = plantInit("Spikeweed", "Spikeweed_Normal");
	_plantAnimation->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	_plantAnimation->setScale(1.5f);
	_plantAnimation->setPosition(Vec2(200, 610));

	SPSSpriteLayer::createPlantsText(0, lta.find("SPIKEWEED_1")->second->text, Vec2(190, 910), lta.find("SPIKEWEED_1")->second->fontsize);
	SPSSpriteLayer::createPlantsText(2, lta.find("SPIKEWEED_2")->second->text, Vec2(360, 1000), lta.find("SPIKEWEED_2")->second->fontsize, Color3B::YELLOW, false);
	SPSSpriteLayer::createPlantsText(3, lta.find("SPIKEWEED_3")->second->text, Vec2(440, 1000), lta.find("SPIKEWEED_3")->second->fontsize, Color3B::RED, false);
	SPSSpriteLayer::createPlantsText(1, lta.find("SPIKEWEED_4")->second->text, Vec2(360, 870), lta.find("SPIKEWEED_4")->second->fontsize, Color3B::ORANGE, false);
	
	return _plantAnimation;
}
