/**
 *Copyright (c) 2020 LZ.All Right Reserved
 *Author : LZ
 *Date: 2020.2.25
 *Email: 2117610943@qq.com
 */

#include "LZPEAcidLemon.h"
#include "Bullet/LZPEBAcidLemonBullet.h"
#include "Zombies/LZZZombies.h"
#include "Scenes/GameScene/LZSGData.h"
#include "Based/LZBPlayMusic.h"

AcidLemonShooter::AcidLemonShooter(Node* node) :
	_isCreateAcidLemon(false)
{
	_node = node;
	_plantImage = nullptr;

	_healthPoint = 300;
	_combatEffecttiveness = 0;
	_plantsType = PlantsType::AcidLemonShooter;
}

AcidLemonShooter::~AcidLemonShooter()
{
}

AcidLemonShooter* AcidLemonShooter::create(Node* node)
{
	AcidLemonShooter* acidLemonShooter = new (std::nothrow) AcidLemonShooter(node);
	if (acidLemonShooter && acidLemonShooter->init())
	{
		acidLemonShooter->autorelease();
		return acidLemonShooter;
	}
	CC_SAFE_DELETE(acidLemonShooter);
	return nullptr;
}

Sprite* AcidLemonShooter::createPlantImage()
{
	imageInit("LemonShooter", INIT);
	_plantImage->setScale(1.3f);
	return _plantImage;
}

void AcidLemonShooter::createPlantAnimation()
{
	_plantAnimation = plantInit("LemonShooter", "LemonNormal");
	_plantAnimation->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	_plantAnimation->setScale(1.2f);
	_plantAnimation->setTimeScale(0.8f);
	_node->addChild(_plantAnimation);

	// 影子
	setPlantShadow(1.5f);
	_plantAnimation->getChildByName("plantshadow")->setPosition(Vec2(0, 10));

	// 泥土飞溅动画
	setPlantSoilSplashAnimation(0.8f);
}

void AcidLemonShooter::determineRelativePositionPlantsAndZombies()
{
	for (auto zombie : ZombiesGroup)
	{
		zombieEatPlant(zombie);      /* 僵尸吃植物 */

		plantAttack(zombie);         /* 植物攻击 */

		zombieRecoveryMove(zombie);  /* 僵尸恢复移动 */
	}

	plantRecovery("LemonNormal");
}

void AcidLemonShooter::plantAttack(Zombies* zombie)
{
	if (getPlantIsSurvive() && zombie->getZombieIsSurvive() && zombie->getZombieIsEnterMap() &&     /* 植物存活 && 僵尸没有死亡 && 僵尸进入地图 */
		getZombieIsTheFrontOfPlant(zombie) && getZombieIsSameLineWithPlant(zombie))                 /* 僵尸与植物在同一行 && 僵尸在植物的前方 */
	{
		plantEmission("LemonShooter");
	}
}

void AcidLemonShooter::plantEmission(const string& plantAnimation)
{
	_isCreateAcidLemon = true; /* 表示有僵尸与植物在同一行 */
	if (!_isChanged)     /* 判断动画是否已经改变 */
	{
		_plantAnimation->addAnimation(0, plantAnimation, true);
		_isChanged = true;
	}

	_plantAnimation->setEventListener([&](spTrackEntry* entry, spEvent* event)
		{
			if (strcmp(event->data->name, "shoot") == 0)
			{
				rand() % 2 == 0 ? PlayMusic::playMusic("throw") : PlayMusic::playMusic("throw2");
				createAcidLemon();
			}
		});
}

void AcidLemonShooter::plantRecovery(const string& plantAnimation)
{
	if (!_isCreateAcidLemon)  /* 判断是否有僵尸与强酸柠檬射手在同一行 */
	{
		if (_isChanged)
		{
			_plantAnimation->addAnimation(0, plantAnimation, true);
			_isChanged = false;
		}
	}

	_isCreateAcidLemon = false; /* 每循环一次就初始化 */
}

void AcidLemonShooter::createAcidLemon()
{
	_bulletAnimation = new AcidLemonBullet(_node);
	_bulletAnimation->setBulletPosition(_position);
	_bulletAnimation->setBulletInRow(_rowAndColumn.y);
	_bulletAnimation->createBullet();

	BulletGroup.push_back(_bulletAnimation);
}

SkeletonAnimation* AcidLemonShooter::showPlantAnimationAndText()
{
	auto& lta = _global->userInformation->getGameText();
	SPSSpriteLayer::plantCardTextScrollView->setInnerContainerSize(Size(lta.find("ACIDLEMON_1")->second->position));

	_isLoop = true;
	_plantAnimation = plantInit("LemonShooter", "LemonNormal");
	_plantAnimation->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	_plantAnimation->setScale(2.2f);
	_plantAnimation->setPosition(Vec2(200, 610));

	SPSSpriteLayer::createPlantsText(0, lta.find("ACIDLEMON_1")->second->text, Vec2(190, 910), lta.find("ACIDLEMON_1")->second->fontsize);
	SPSSpriteLayer::createPlantsText(2, lta.find("ACIDLEMON_2")->second->text, Vec2(360, 1000), lta.find("ACIDLEMON_2")->second->fontsize, Color3B::YELLOW, false);
	SPSSpriteLayer::createPlantsText(3, lta.find("ACIDLEMON_3")->second->text, Vec2(440, 1000), lta.find("ACIDLEMON_3")->second->fontsize, Color3B::RED, false);
	SPSSpriteLayer::createPlantsText(1, SPSSpriteLayer::selectRequirementText(lta, PlantsType::AcidLemonShooter, "ACIDLEMON_4", "ACIDLEMON_5"), Vec2(360, 870),
		lta.find("ACIDLEMON_4")->second->fontsize, SPSSpriteLayer::isPlantIsCanSelect[static_cast<unsigned int>(PlantsType::AcidLemonShooter)] ? Color3B::YELLOW : Color3B(255, 70, 0), false);
	
	return _plantAnimation;
}