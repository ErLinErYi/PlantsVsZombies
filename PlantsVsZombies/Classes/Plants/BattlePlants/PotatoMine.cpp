/**
 *Copyright (c) 2020 LZ.All Right Reserved
 *Author : LZ
 *Date: 2020.2.6
 *Email: 2117610943@qq.com
 */

#include "PotatoMine.h"
#include "../EmissionPlants/Bullet/Bullet.h"

#include "Zombies/Zombies.h"
#include "Scenes/GameScene/GSData.h"
#include "Scenes/GameScene/GSBackgroundLayer.h"
#include "Scenes/GameScene/GSControlLayer.h"
#include "Based/PlayMusic.h"

PotatoMine::PotatoMine(Node* node):
	_isReady(false)
,   _isBeginExplode(false)
,   _isExplodeFinished(false)
,   _isCanKillZombies(false)
,   _isKillZombiesFinished(false)
,   _breakGround(15.f)
{
	_node = node;
	_plantImage = nullptr;

	_healthPoint = 300;
	_isLoop = false;
	_combatEffecttiveness = 1800;
	_plantsType = PlantsType::PotatoMine;
}

PotatoMine::~PotatoMine()
{
}

PotatoMine* PotatoMine::create(Node* node)
{
	PotatoMine* potatoMine = new (std::nothrow) PotatoMine(node);
	if (potatoMine && potatoMine->init())
	{
		potatoMine->autorelease();
		return potatoMine;
	}
	CC_SAFE_DELETE(potatoMine);
	return nullptr;
}

void PotatoMine::setBreakGround(const float time)
{
	_breakGround = time;
}

float PotatoMine::getBreakGround() const
{
	return _breakGround;
}
Sprite* PotatoMine::createPlantImage()
{
	imageInit("PotatoMine", INIT);
	_plantImage->setScale(1.3f);
	return _plantImage;
}

void PotatoMine::createPlantAnimation()
{
	_plantAnimation = plantInit("PotatoMine", "PotatoMine_Ready");
	_plantAnimation->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	_plantAnimation->setScale(1.0f);
	_node->addChild(_plantAnimation);

	// 影子
	setPlantShadow(1.4f);

	// 泥土飞溅动画
	setPlantSoilSplashAnimation(0.8f);

	// 创建监听
	createListener();
}

void PotatoMine::createListener()
{
	_plantAnimation->runAction(Sequence::create(DelayTime::create(_breakGround), 
		CallFunc::create([this]()
		{
			PlayMusic::playMusic("dirt_rise");
			_isReady = true;
			_plantAnimation->setAnimation(0, "PotatoMine_Out", false);
			_plantAnimation->addAnimation(0, "PotatoMine_Normal", true);
		}), nullptr));
	_plantAnimation->runAction(Repeat::create(Sequence::create(DelayTime::create(1.0f),
		CallFunc::create([this]()
			{
				if (_breakGround > 0)
					--_breakGround;
			}), nullptr), 15));
}

void PotatoMine::determineRelativePositionPlantsAndZombies()
{
	for (auto zombie : ZombiesGroup)
	{
		zombieEatPlant(zombie);      /* 僵尸吃植物 */

		plantExplode();              /* 植物攻击 */

		zombieRecoveryMove(zombie);  /* 僵尸恢复移动 */
	}
}

void PotatoMine::plantExplode()
{
	if (_isBeginExplode && !_isExplodeFinished)/* 土豆雷爆炸 */
	{
		_isExplodeFinished = true;
		_plantAnimation->setVisible(false);

		auto plantAnimation = SkeletonAnimation::createWithData(_global->userInformation->getAnimationData().find("PotatoMine")->second);
		plantAnimation->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
		plantAnimation->setScale(1.2f);
		plantAnimation->setLocalZOrder(_plantAnimation->getLocalZOrder() + 1);
		plantAnimation->setPosition(_plantAnimation->getPosition());
		plantAnimation->setAnimation(0, "PotatoMine_Blast", false);
		plantAnimation->setEventListener([=](spTrackEntry* entry, spEvent* event)
			{
				if (strcmp(event->data->name, "BlastBegin") == 0)
				{
					_isCanKillZombies = true;
					GSBackgroundLayer::backgroundRunAction();
				}
			});
		plantAnimation->runAction(Sequence::create(DelayTime::create(2.f),
			CallFunc::create([plantAnimation]()
				{
					plantAnimation->setVisible(false);
				}), nullptr));
		_node->addChild(plantAnimation);
	}

	if (_isCanKillZombies&& !_isKillZombiesFinished)
	{
		explodeHurtZombies();
		_isKillZombiesFinished = true;
	}
}

void PotatoMine::zombieEatPlant(Zombies* zombie)
{
	if (getPlantIsSurvive() && getZombieIsSameLineWithPlant(zombie) && getzombieIsEncounterPlant(zombie)) /* 植物存活 && 植物与僵尸在同一行 && 僵尸遇到植物 */
	{
		if (_isReady)  /* 土豆雷准备好 */
		{
			_isBeginExplode = true;
		}

		if (zombie->getZombieIsSurvive() && !zombie->getZombieIsEat())
		{
			const string eateffect[3] = { "chomp","chomp2","chompsoft" };
			zombie->setZombieEatPlantNumber(_plantNumber);
			zombie->setZombieStop();
			zombie->setZombieIsEat(true);
			zombie->getZombieAnimation()->setAnimation(1, "Zombies_Eat", true);
			zombie->getZombieAnimation()->setEventListener([this, eateffect](spTrackEntry* entry, spEvent* event)
				{
					if (!strcmp(event->data->name, "eat"))
					{
						if (event->intValue == 1)
						{
							reducePlantHealthPoint(100);
							Bullet::playSoundEffect(eateffect[rand() % 3]);
							setPlantHurtBlink(PlantsType::PotatoMine);
						}
					}
				});
		}
	}
}

void PotatoMine::explodeHurtZombies()
{
	PlayMusic::playMusic("potato_mine");
	
	for (auto zombie : ZombiesGroup)
	{
		if (zombie->getZombieIsSurvive() && zombie->getZombieIsEnterMap() && getZombieIsInExplodeRange(zombie)) /* 僵尸存活 && 僵尸进入地图 && 僵尸在爆炸范围内 */
		{
			hurtZombies(zombie);

			if (!zombie->getZombieIsSurvive())
			{
				zombie->setZombieOpacity(0);
			}
		}
	}
}

bool PotatoMine::getZombieIsInExplodeRange(Zombies* zombie)
{
	return (getZombieIsSameLineWithPlant(zombie) && fabs(zombie->getZombieAnimation()->getPositionX() - _plantAnimation->getPositionX()) < 190) ? true : false;
}
