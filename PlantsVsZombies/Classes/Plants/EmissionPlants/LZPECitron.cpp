/**
 *Copyright (c) 2020 LZ.All Right Reserved
 *Author : LZ
 *Date: 2020.2.27
 *Email: 2117610943@qq.com
 */

#include "LZPECitron.h"
#include "Bullet/LZPEBCitronBullet.h"

#include "Zombies/LZZZombies.h"
#include "Scenes/GameScene/LZSGData.h"
#include "Based/LZBPlayMusic.h"

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
					rand() % 2 == 0 ? PlayMusic::playMusic("throw") : PlayMusic::playMusic("throw2");
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
	_bulletAnimation->setBulletInRow(_rowAndColumn.y);
	_bulletAnimation->createBullet();

	BulletGroup.push_back(_bulletAnimation);
}

SkeletonAnimation* Citron::showPlantAnimationAndText()
{
	auto& lta = _global->userInformation->getGameText();
	SPSSpriteLayer::plantCardTextScrollView->setInnerContainerSize(Size(lta.find("CITRON_1")->second->position));

	_isLoop = true;
	_plantAnimation = plantInit("Citron", "normal");
	_plantAnimation->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	_plantAnimation->setScale(2.0f);
	_plantAnimation->setPosition(Vec2(200, 610));

	SPSSpriteLayer::createPlantsText(0, lta.find("CITRON_1")->second->text, Vec2(190, 910), lta.find("CITRON_1")->second->fontsize);
	SPSSpriteLayer::createPlantsText(2, lta.find("CITRON_2")->second->text, Vec2(360, 1000), lta.find("CITRON_2")->second->fontsize, Color3B::YELLOW, false);
	SPSSpriteLayer::createPlantsText(3, lta.find("CITRON_3")->second->text, Vec2(440, 1000), lta.find("CITRON_3")->second->fontsize, Color3B::RED, false);
	SPSSpriteLayer::createPlantsText(1, SPSSpriteLayer::selectRequirementText(lta, PlantsType::Citron, "CITRON_4", "CITRON_5"), Vec2(360, 870),
		lta.find("CITRON_4")->second->fontsize, SPSSpriteLayer::isPlantIsCanSelect[static_cast<unsigned int>(PlantsType::Citron)] ? Color3B::YELLOW : Color3B(255, 70, 0), false);
	
	return _plantAnimation;
}