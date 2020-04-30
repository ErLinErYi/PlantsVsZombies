/**
 *Copyright (c) 2020 LZ.All Right Reserved
 *Author : LZ
 *Date: 2020.2.27
 *Email: 2117610943@qq.com
 */

#include "Citron.h"
#include "Bullet/CitronBullet.h"

#include "Zombies/Zombies.h"
#include "Scenes/GameScene/GSData.h"

Citron::Citron(Node* node):
	_attackInterval(0)
,   _animationId(0)
,   _isBaginReady(true)
,   _readyFinished(false)
{
	_node = node;
	_plantImage = nullptr;

	_healthPoint = 300;
	_combatEffecttiveness = 0;
	_plantsType = PlantsType::Citron;
	srand(time(nullptr));
}

Citron::~Citron()
{
}

Citron* Citron::create(Node* node)
{
	Citron* citron = new (std::nothrow) Citron(node);
	if (citron && citron->init())
	{
		citron->autorelease();
		return citron;
	}
	CC_SAFE_DELETE(citron);
	return nullptr;
}

Sprite* Citron::createPlantImage()
{
	imageInit("Citron", INIT);
	_plantImage->setScale(1.3f);
	return _plantImage;
}

void Citron::createPlantAnimation()
{
	_plantAnimation = plantInit("Citron", "normal");
	_plantAnimation->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	_plantAnimation->setScale(1.0f);
	_plantAnimation->setTimeScale(0.6f);
	_node->addChild(_plantAnimation);

	// 影子
	setPlantShadow(1.9f);
	_plantAnimation->getChildByName("plantshadow")->setPosition(Vec2(0, 15));

	// 泥土飞溅动画
	setPlantSoilSplashAnimation(0.8f);

	createListener();
}

void Citron::determineRelativePositionPlantsAndZombies()
{
	for (auto zombie : ZombiesGroup)
	{
		zombieEatPlant(zombie);      /* 僵尸吃植物 */

		plantAttack(zombie);         /* 植物攻击 */

		zombieRecoveryMove(zombie);  /* 僵尸恢复移动 */
	}
}

void Citron::plantAttack(Zombies* zombie)
{
	if (getPlantIsSurvive() && zombie->getZombieIsSurvive() && zombie->getZombieIsEnterMap() &&     /* 植物存活 && 僵尸没有死亡 && 僵尸进入地图 */
		getZombieIsTheFrontOfPlant(zombie) && getZombieIsSameLineWithPlant(zombie))                 /* 僵尸与植物在同一行 && 僵尸在植物的前方 */
	{
		plantEmission();
	}
}

void Citron::plantEmission()
{
	if (_readyFinished)
	{
		if (!_isChanged)     /* 判断动画是否已经改变 */
		{
			_plantAnimation->addAnimation(0, calculateGreatEvocationProbability() ? "shoot1" : "shoot", true);
			_isChanged = true;
		}

		_plantAnimation->setEventListener([&](spTrackEntry* entry, spEvent* event)
			{
				if (strcmp(event->data->name, "shoot") == 0)
				{
					rand() % 2 == 0 ? Bullet::playSoundEffect("throw") : Bullet::playSoundEffect("throw2");
					createBullet();
					plantRecovery("normal");
				}
			});
	}
}

void Citron::plantRecovery(const string& plantAnimation)
{
	if (_isChanged)
	{
		_plantAnimation->addAnimation(0, plantAnimation, true);
		_isBaginReady = true; /* 开始准备 */
		_readyFinished = false;
		_attackInterval = 0;

		_isChanged = false;
	}
}

void Citron::createListener()
{
	_plantAnimation->runAction(RepeatForever::create(Sequence::create(DelayTime::create(0.5f), 
		CallFunc::create([&]() 
			{
				++_attackInterval;
				if (_attackInterval >= 16 && _isBaginReady)
				{
					_readyFinished = true;
					_isBaginReady = false;
				}
			}), nullptr)));
}

int Citron::calculateGreatEvocationProbability()
{
	int number = rand() % 100;
	if (number < 5)
		_animationId = 1;
	else
		_animationId = 0;
	return _animationId;
}

void Citron::createBullet()
{
	_bulletAnimation = new CitronBullet(_node, _animationId);
	_bulletAnimation->setBulletPosition(_position);
	_bulletAnimation->createBullet();

	BulletGroup.push_back(_bulletAnimation);
}
