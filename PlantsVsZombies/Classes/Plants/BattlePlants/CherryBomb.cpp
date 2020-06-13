/**
 *Copyright (c) 2020 LZ.All Right Reserved
 *Author : LZ
 *Date: 2020.2.6
 *Email: 2117610943@qq.com
 */

#include "CherryBomb.h"
#include "../EmissionPlants/Bullet/Bullet.h"

#include "Scenes/GameScene/GSData.h"
#include "Zombies/Zombies.h"
#include "Scenes/GameScene/GSBackgroundLayer.h"

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
	return _plantImage;
}

void CherryBomb::createPlantAnimation()
{
	_plantAnimation = plantInit("CherryBomb", "CherryBomb_Explode");
	_plantAnimation->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	_plantAnimation->setTimeScale(1.5f);
	_node->addChild(_plantAnimation);

	// Ó°×Ó
	setPlantShadow(1.8f);

	// ÄàÍÁ·É½¦¶¯»­
	setPlantSoilSplashAnimation(0.8f);

	// ÊÂ¼þ¼àÌý
	setEventListener();
}

void CherryBomb::setEventListener()
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
	Bullet::playSoundEffect("wakeup");
}

bool CherryBomb::getPlantIsReadyExplode() const
{
	return _isReadyExplode;
}

bool CherryBomb::getZombieIsInExplodeRange(Zombies* zombie) const
{
	/* ½©Ê¬ÊÇ·ñÔÚ±¬Õ¨·¶Î§ÅÐ¶Ï */
	return sqrt(pow(zombie->getZombieAnimation()->getPositionX() - _plantAnimation->getPositionX(), 2) +
		pow(zombie->getZombieAnimation()->getPositionY() - _plantAnimation->getPositionY(), 2)) <= 195 ? true : false;
}

void CherryBomb::determineRelativePositionPlantsAndZombies()
{
	for (auto zombie : ZombiesGroup)
	{
		zombieEatPlant(zombie);      /* ½©Ê¬³ÔÖ²Îï */

		plantExplode();              /* Ö²Îï¹¥»÷ */

		zombieRecoveryMove(zombie);  /* ½©Ê¬»Ö¸´ÒÆ¶¯ */
	}
}

void CherryBomb::plantExplode()
{
	if (!getPlantIsSurvive() || getPlantIsReadyExplode()) /* Ö²ÎïËÀÍö || Ö²Îï×¼±¸±¬Õ¨ */
	{
		if (!_isExplode) /* Èç¹ûÃ»ÓÐ±¬Õ¨ */
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
		if (zombie->getZombieIsSurvive() && zombie->getZombieIsEnterMap() && getZombieIsInExplodeRange(zombie)) /* ½©Ê¬´æ»î && ½©Ê¬½øÈëµØÍ¼ && ½©Ê¬ÔÚ±¬Õ¨·¶Î§ÄÚ */
		{
			hurtZombies(zombie);
			zombie->setZombieIsShowLoseLimbsAnimation(false);

			if (!zombie->getZombieIsSurvive())
			{
				zombie->setZombieOpacity(0);
				zombie->playZombiesDieAnimation();
			}
		}
	}
}

void CherryBomb::showExplodeAnimation()
{
	Bullet::playSoundEffect("cherrybomb");
	GSBackgroundLayer::backgroundRunAction();

	/* ±¬Õ¨¶¯»­ */
	auto cherryBomb_Explode = SkeletonAnimation::createWithData(_global->userInformation->getAnimationData().find("CherryBomb_Explode")->second);
	cherryBomb_Explode->setPosition(_plantAnimation->getPosition());
	cherryBomb_Explode->setAnimation(0, "CherryBomb_Explode", false);
	cherryBomb_Explode->setScale(1.5f);
	cherryBomb_Explode->setLocalZOrder(_plantAnimation->getLocalZOrder() + 10); // Ö²Îï»æÖÆË³Ðò¼Ó10ÕýºÃµÈÓÚ½©Ê¬»æÖÆË³Ðò £¬±¬Õ¨¾Í¿ÉÒÔ¸²¸Çµ½½©Ê¬ÉÏÃæ
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
