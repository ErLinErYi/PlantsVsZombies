/**
 *Copyright (c) 2020 LZ.All Right Reserved
 *Author : LZ
 *Date: 2020.2.6
 *Email: 2117610943@qq.com
 */

#include "LZCherryBomb.h"

#include "Scenes/GameScenes/Adventure/GameScene/LZAGSData.h"
#include "Scenes/GameScenes/Adventure/GameScene/LZAGSBackgroundLayer.h"
#include "Zombies/LZZombies.h"
#include "Based/LZPlayMusic.h"

CherryBomb::CherryBomb(Node* node):
	_isReadyExplode(false)
,   _isExplode(false)
{
	_node = node;
	_plantImage = nullptr;

	_healthPoint = 300;
	_isLoop = false;
	_combatEffecttiveness = 1800;
	_plantsType = PlantsType::CherryBomb;
}

CherryBomb::~CherryBomb()
{
}

CherryBomb* CherryBomb::create(Node* node)
{
	CherryBomb* cherryBomb = new (std::nothrow) CherryBomb(node);
	if (cherryBomb && cherryBomb->init())
	{
		cherryBomb->autorelease();
		return cherryBomb;
	}
	CC_SAFE_DELETE(cherryBomb);
	return nullptr;
}
Sprite* CherryBomb::createPlantImage()
{
	imageInit("CherryBomb", INIT);
	_plantImage->setScale(1.3f);
	_plantImage->setAnchorPoint(Vec2(0.45f, 0.6f));
	return _plantImage;
}

void CherryBomb::createPlantAnimation()
{
	_plantAnimation = plantInit("CherryBomb", "CherryBomb_Explode");
	_plantAnimation->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	_plantAnimation->setTimeScale(1.5f);
	_node->addChild(_plantAnimation);

	// 影子
	setPlantShadow(1.8f);

	// 泥土飞溅动画
	setPlantSoilSplashAnimation(0.8f);

	// 事件监听
	createListener();
}

void CherryBomb::setPlantNoramlAnimation()
{
	_plantAnimation->setAnimation(0, "CherryBomb_Normal", true);
}

void CherryBomb::createListener()
{
	_plantAnimation->setEventListener([this](spTrackEntry* entry, spEvent* event)
		{
			if (!strcmp(event->data->name, "Finished"))
			{
				_isReadyExplode = true;
				_healthPoint = 0;
				_plantAnimation->setVisible(false);
			}
		});
	PlayMusic::playMusic("wakeup");
}

bool CherryBomb::getPlantIsReadyExplode() const
{
	return _isReadyExplode;
}

bool CherryBomb::getZombieIsInExplodeRange(Zombies* zombie)
{
	/* 僵尸是否在爆炸范围判断 */
	return fabs(zombie->getZombieAnimation()->getPositionX() - _plantAnimation->getPositionX()) < 200 &&
		fabs((zombie->getZombieAnimation()->getPositionY() + 50) - (_plantAnimation->getPositionY() + 60)) < 200;
}

void CherryBomb::determineRelativePositionPlantsAndZombies()
{
	for (auto zombie : ZombiesGroup)
	{
		zombieEatPlant(zombie);      /* 僵尸吃植物 */

		zombieRecoveryMove(zombie);  /* 僵尸恢复移动 */
	}

	plantExplode();                  /* 植物攻击 */
}

void CherryBomb::plantExplode()
{
	if (!getPlantIsSurvive() || getPlantIsReadyExplode()) /* 植物死亡 || 植物准备爆炸 */
	{
		if (!_isExplode) /* 如果没有爆炸 */
		{
			_isExplode = true;
			explodeHurtZombies();
			showExplodeAnimation();
		}
	}
}

void CherryBomb::explodeHurtZombies()
{
	for (auto zombie : ZombiesGroup)
	{
		if (zombie->getZombieIsSurvive() && zombie->getZombieIsEnterMap() && getZombieIsInExplodeRange(zombie)) /* 僵尸存活 && 僵尸进入地图 && 僵尸在爆炸范围内 */
		{
			hurtZombies(zombie);
			zombie->setZombieHurtBlink();
			
			if (!zombie->getZombieIsSurvive())
			{
				zombie->setZombieIsShowLoseLimbsAnimation(false);
				zombie->setZombieVisible(false);
				zombie->playZombiesAshesAnimation();
			}
		}
	}
}

void CherryBomb::showExplodeAnimation()
{
	PlayMusic::playMusic("cherrybomb");
	GSBackgroundLayer::backgroundRunAction();

	/* 爆炸动画 */
	auto cherryBomb_Explode = SkeletonAnimation::createWithData(_global->userInformation->getAnimationData().find("CherryBomb_Explode")->second);
	cherryBomb_Explode->setPosition(_plantAnimation->getPosition());
	cherryBomb_Explode->setAnimation(0, "CherryBomb_Explode", false);
	cherryBomb_Explode->setScale(1.5f);
	cherryBomb_Explode->setLocalZOrder(_plantAnimation->getLocalZOrder() + 100); // 植物绘制顺序加10正好等于僵尸绘制顺序 ，爆炸就可以覆盖到僵尸上面
	_node->addChild(cherryBomb_Explode);

	cherryBomb_Explode->setEventListener([cherryBomb_Explode](spTrackEntry* entry, spEvent* event)
		{
			if (!strcmp(event->data->name, "Finished"))
			{
				cherryBomb_Explode->runAction(FadeOut::create(0.5f));
			}
		});
	_node->runAction(Sequence::create(DelayTime::create(2.0f), CallFunc::create([cherryBomb_Explode]() {cherryBomb_Explode->removeFromParent(); }), nullptr));
}

SkeletonAnimation* CherryBomb::showPlantAnimationAndText()
{
	auto& lta = _global->userInformation->getGameText();
	SPSSpriteLayer::plantCardTextScrollView->setInnerContainerSize(Size(lta.find("CHERRYBOMB_1")->second->position));

	_isLoop = true;
	_plantAnimation = plantInit("CherryBomb", "CherryBomb_Normal");
	_plantAnimation->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	_plantAnimation->setScale(1.8f);
	_plantAnimation->setPosition(Vec2(200, 610));

	SPSSpriteLayer::createPlantsText(0, lta.find("CHERRYBOMB_1")->second->text, Vec2(190, 910), lta.find("CHERRYBOMB_1")->second->fontsize);
	SPSSpriteLayer::createPlantsText(2, lta.find("CHERRYBOMB_2")->second->text, Vec2(360, 1000), lta.find("CHERRYBOMB_2")->second->fontsize, Color3B::YELLOW, false);
	SPSSpriteLayer::createPlantsText(3, lta.find("CHERRYBOMB_3")->second->text, Vec2(440, 1000), lta.find("CHERRYBOMB_3")->second->fontsize, Color3B::RED, false);
	SPSSpriteLayer::createPlantsText(1, SPSSpriteLayer::selectRequirementText(lta, PlantsType::CherryBomb, "CHERRYBOMB_4", "CHERRYBOMB_5"), Vec2(360, 870),
		lta.find("CHERRYBOMB_4")->second->fontsize, SPSSpriteLayer::isPlantIsCanSelect[static_cast<unsigned int>(PlantsType::CherryBomb)] ? Color3B::ORANGE : Color3B(255, 70, 0), false);

	return _plantAnimation;
}