/**
 *Copyright (c) 2021 LZ.All Right Reserved
 *Author : LZ
 *Date: 2021.1.22
 *Email: 2117610943@qq.com
 */

#include "LZGloomShroom.h"
#include "Zombies/LZZombies.h"
#include "Scenes/GameScenes/Adventure/GameScene/LZAGSData.h"
#include "Based/LZPlayMusic.h"

GloomShroom::GloomShroom(Node* node):
	_isChangeAnimation(false)
,   _isHaveZombies(false)
,   _isAttack(false)
{
	_node = node;
	_plantImage = nullptr;

	_healthPoint = 300;
	_isLoop = true;
	_combatEffecttiveness = 20;
	_plantsType = PlantsType::GloomShroom;
}

GloomShroom::~GloomShroom()
{
}

GloomShroom* GloomShroom::create(Node* node)
{
	GloomShroom* gloomShroom = new (std::nothrow) GloomShroom(node);
	if (gloomShroom && gloomShroom->init())
	{
		gloomShroom->autorelease();
		return gloomShroom;
	}
	CC_SAFE_DELETE(gloomShroom);
	return nullptr;
}

Sprite* GloomShroom::createPlantImage()
{
	imageInit("GloomShroom", INIT);
	_plantImage->setScale(1.4f);
	_plantImage->setAnchorPoint(Vec2(0.5f, 0.55f));
	return _plantImage;
}

void GloomShroom::createPlantAnimation()
{
	_plantAnimation = plantInit("GloomShroom", "GloomShroom_Normal");
	_plantAnimation->setTimeScale(1.2f);
	_plantAnimation->setScale(1.3f);
	_plantAnimation->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	_node->addChild(_plantAnimation);

	// 影子
	setPlantShadow(1.8f);

	// 泥土飞溅动画
	setPlantSoilSplashAnimation(0.8f);

	// 监听
	createListener();
}

void GloomShroom::createListener()
{
	_plantAnimation->setEventListener([=](spTrackEntry* entry, spEvent* event)
		{
			if (!strcmp(event->data->name, "attack"))
			{
				_isAttack =true;
			}
		});
}

void GloomShroom::determineRelativePositionPlantsAndZombies()
{
	for (auto zombie : ZombiesGroup)
	{
		zombieEatPlant(zombie);      /* 僵尸吃植物 */

		plantAttack(zombie);         /* 植物攻击 */

		zombieRecoveryMove(zombie);  /* 僵尸恢复移动 */
	}

	plantRecovery("GloomShroom_Normal");
}

void GloomShroom::plantAttack(Zombies* zombie)
{
	if (getPlantIsSurvive() && zombie->getZombieIsSurvive() &&                  /* 植物存活 && 僵尸没有死亡  */
		zombie->getZombieIsEnterMap() && getZombieIsInAttackRange(zombie) &&    /* 僵尸进入地图 && 僵尸在攻击范围内 */
		zombie->getZombieIsCanBeAttack())                                       /* 僵尸是否可以被攻击到 */
	{
		_isHaveZombies = true;

		if (!_isChangeAnimation)
		{
			_isChangeAnimation = true;
			_plantAnimation->addAnimation(0, "GloomShroom_Attack", true);
		}

		if (_isAttack)
		{
			hurtZombies(zombie);
			zombie->setZombieHurtBlink();
			PlayMusic::playMusic(rand() % 2 ? "splat" : (rand() % 2 ? "splat2" : "splat3"));
		}
	}
}

bool GloomShroom::getZombieIsInAttackRange(Zombies* zombie)
{
	/* 僵尸是否在攻击范围判断 */
	return fabs(zombie->getZombieAnimation()->getPositionX() - _plantAnimation->getPositionX()) < (zombie->getZombieIsEat() ? 230 : 195) &&
		fabs((zombie->getZombieAnimation()->getPositionY() + 50) - (_plantAnimation->getPositionY() + 60)) < 195;
}

void GloomShroom::plantRecovery(const string& name)
{
	if (!_isHaveZombies) /* 如果没有僵尸 */
	{
		if (_isChangeAnimation)   /* 如果植物还在攻击 */
		{
			_isChangeAnimation = false;
			_plantAnimation->addAnimation(0, name, true);
		}
	}

	_isHaveZombies = false;
	_isAttack = false;
}

SkeletonAnimation* GloomShroom::showPlantAnimationAndText()
{
	auto& lta = _global->userInformation->getGameText();
	SPSSpriteLayer::plantCardTextScrollView->setInnerContainerSize(Size(lta.find("GloomShroom_1")->second->position));

	_isLoop = true;
	_plantAnimation = plantInit("GloomShroom", "GloomShroom_Normal");
	_plantAnimation->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	_plantAnimation->setScale(1.8f);
	_plantAnimation->setPosition(Vec2(200, 610));

	SPSSpriteLayer::createPlantsText(0, lta.find("GloomShroom_1")->second->text, Vec2(190, 910), lta.find("GloomShroom_1")->second->fontsize);
	SPSSpriteLayer::createPlantsText(2, lta.find("GloomShroom_2")->second->text, Vec2(360, 1000), lta.find("GloomShroom_2")->second->fontsize, Color3B::YELLOW, false);
	SPSSpriteLayer::createPlantsText(3, lta.find("GloomShroom_3")->second->text, Vec2(440, 1000), lta.find("GloomShroom_3")->second->fontsize, Color3B::RED, false);
	SPSSpriteLayer::createPlantsText(1, SPSSpriteLayer::selectRequirementText(lta, PlantsType::GloomShroom, "GloomShroom_4", "GloomShroom_5"), Vec2(360, 870),
		lta.find("GloomShroom_4")->second->fontsize, SPSSpriteLayer::isPlantIsCanSelect[static_cast<unsigned int>(PlantsType::GloomShroom)] ? Color3B::ORANGE : Color3B(255, 70, 0), false);

	return _plantAnimation;
}
