/**
 *Copyright (c) 2020 LZ.All Right Reserved
 *Author : LZ
 *Date: 2021.7.08
 *Email: 2117610943@qq.com
 */

#include "LZSquash.h"

#include "Scenes/GameScenes/Adventure/GameScene/LZAGSData.h"
#include "Scenes/GameScenes/Adventure/GameScene/LZAGSBackgroundLayer.h"
#include "Zombies/LZZombies.h"
#include "Based/LZPlayMusic.h"

Squash::Squash(Node* node):
	_isUsed(false),
	_isAttack(false),
	_delete(false),
	_direction(1),
	_zombiePositionX(0)
{
	_node = node;
	_plantImage = nullptr;

	_healthPoint = 3000;
	_combatEffecttiveness = 1800;
	_plantsType = PlantsType::Squash;
}

Squash::~Squash()
{
}

Squash* Squash::create(Node* node)
{
	Squash* squash = new (std::nothrow) Squash(node);
	if (squash && squash->init())
	{
		squash->autorelease();
		return squash;
	}
	CC_SAFE_DELETE(squash);
	return nullptr;
}
Sprite* Squash::createPlantImage()
{
	imageInit("Squash", INIT);
	_plantImage->setScale(1.3f);
	_plantImage->setAnchorPoint(Vec2(0.4f, 0.5f));
	return _plantImage;
}

void Squash::createPlantAnimation()
{
	_plantAnimation = plantInit("Squash", "Squash_Normal");
	_plantAnimation->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	_plantAnimation->setScale(0.9f);
	_node->addChild(_plantAnimation);

	// 影子
	setPlantShadow(2.3f);
	_plantAnimation->getChildByName("plantshadow")->setPosition(Vec2(7, 20));

	// 泥土飞溅动画
	setPlantSoilSplashAnimation(0.8f);

	// 事件监听
	createListener();
}

void Squash::createListener()
{
	_plantAnimation->setEventListener([this](spTrackEntry* entry, spEvent* event)
		{
			if (!strcmp(event->data->name, "jump"))
			{
				_delete = true;
				_plantAnimation->getChildByName("plantshadow")->setVisible(false);
				_plantAnimation->runAction(Sequence::create(EaseExponentialOut::create(MoveTo::create(0.5f, Vec2(_zombiePositionX - 40, _position.y + 230))),
					CallFunc::create([=]() 
						{
							_isAttack = true;
							_plantAnimation->addAnimation(0, "Squash_LeftDown", false);
						}), EaseExponentialIn::create(MoveBy::create(0.3f, Vec2(0, -230))),
					CallFunc::create([]() 
						{
							PlayMusic::playMusic("gargantuar_thump");
							GSBackgroundLayer::backgroundRunAction();
						}), nullptr));
			}
			if (!strcmp(event->data->name, "attack"))
			{
			}
			if (!strcmp(event->data->name, "turn_finish"))
			{
				_plantAnimation->setScaleX(_direction * _plantAnimation->getScaleX());
				_plantAnimation->setLocalZOrder(_plantAnimation->getLocalZOrder() + 21);
				_plantAnimation->addAnimation(0, "Squash_LeftJump", false);
			}
			if (!strcmp(event->data->name, "attack_finish"))
			{
				_healthPoint = 0;
				_plantAnimation->setVisible(false);
			}
		});
}

void Squash::determineRelativePositionPlantsAndZombies()
{
	for (auto zombie : ZombiesGroup)
	{
		zombieEatPlant(zombie);      /* 僵尸吃植物 */

		plantAttack(zombie);         /* 植物攻击 */
		 
		zombieRecoveryMove(zombie);  /* 僵尸恢复移动 */
	}

	plantExplode();
}

void Squash::plantAttack(Zombies* zombie)
{
	for (auto zombie : ZombiesGroup)
	{
		if (zombie->getZombieIsSurvive() && zombie->getZombieIsEnterMap()&& getZombieIsSameLineWithPlant(zombie)) /* 僵尸存活 && 僵尸进入地图 && 与僵尸同一行 */
		{
			if (checkZombiesPosition(zombie) == 0)
			{
				PlayMusic::playMusic(rand() % 2 ? "squash_hmm" : "squash_hmm2");
				_isUsed = true;
				_plantAnimation->setScaleX(_direction * _plantAnimation->getScaleX());
				_plantAnimation->setLocalZOrder(_plantAnimation->getLocalZOrder() + 21);
				_plantAnimation->setAnimation(0, "Squash_LeftJump", false);
			}
			else if(checkZombiesPosition(zombie) == 1)
			{
				PlayMusic::playMusic(rand() % 2 ? "squash_hmm" : "squash_hmm2");
				_isUsed = true;
				_plantAnimation->setLocalZOrder(_plantAnimation->getLocalZOrder() + 21);
				_plantAnimation->setAnimation(0, "Squash_LeftJump", false);
			}
		}
	}
}

int Squash::checkZombiesPosition(Zombies* zombie)
{
	if (!_isUsed && getZombieIsInExplodeRange(zombie))
	{
		if (zombie->getZombiePosition().x < _plantAnimation->getPositionX())
		{
			_direction = -1;
			_zombiePositionX = zombie->getZombieAnimation()->getPositionX();
			return 0;
		}
		else
		{
			_direction = 1;
			_zombiePositionX = zombie->getZombieAnimation()->getPositionX();
			return 1;
		}
	}
	return -1;
}

void Squash::plantExplode()
{
	bool flag = false;
	for (auto zombie : ZombiesGroup)
	{
		if (_isAttack && getZombieIsSameLineWithPlant(zombie) && zombie->getZombieIsSurvive() && zombie->getZombieIsEnterMap() &&
			zombie->getZombieAnimation()->getBoundingBox().intersectsRect(_plantAnimation->getBoundingBox()))
		{
			flag = true;
			hurtZombies(zombie);
			zombie->setZombieHurtBlink();

			if (!zombie->getZombieIsSurvive())
			{
				zombie->setZombieVisible(false);
			}
		}
	}

	if (flag)
	{
		_isAttack = false;
	}
}

bool Squash::getZombieIsInExplodeRange(Zombies* zombie)
{
	/* 僵尸是否在爆炸范围判断 */
	return fabs(zombie->getZombieAnimation()->getPositionX() - _plantAnimation->getPositionX()) < 200;
}

SkeletonAnimation* Squash::showPlantAnimationAndText()
{
	auto& lta = _global->userInformation->getGameText();
	SPSSpriteLayer::plantCardTextScrollView->setInnerContainerSize(Size(lta.find("SQUASH_1")->second->position));

	_isLoop = true;
	_plantAnimation = plantInit("Squash", "Squash_Normal");
	_plantAnimation->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	_plantAnimation->setScale(1.5f);
	_plantAnimation->setPosition(Vec2(190, 610));

	SPSSpriteLayer::createPlantsText(0, lta.find("SQUASH_1")->second->text, Vec2(190, 910), lta.find("SQUASH_1")->second->fontsize);
	SPSSpriteLayer::createPlantsText(2, lta.find("SQUASH_2")->second->text, Vec2(360, 1000), lta.find("SQUASH_2")->second->fontsize, Color3B::YELLOW, false);
	SPSSpriteLayer::createPlantsText(3, lta.find("SQUASH_3")->second->text, Vec2(440, 1000), lta.find("SQUASH_3")->second->fontsize, Color3B::RED, false);
	SPSSpriteLayer::createPlantsText(1, SPSSpriteLayer::selectRequirementText(lta, PlantsType::AcidLemonShooter, "SQUASH_4", "SQUASH_5"), Vec2(360, 870),
		lta.find("SQUASH_4")->second->fontsize, SPSSpriteLayer::isPlantIsCanSelect[static_cast<unsigned int>(PlantsType::AcidLemonShooter)] ? Color3B::ORANGE : Color3B(255, 70, 0), false);

	return _plantAnimation;
}

void Squash::cavePlantInformation(rapidjson::Value& object, rapidjson::Document::AllocatorType& allocator)
{
	object.AddMember("ZombiePositionX", _zombiePositionX, allocator);
	object.AddMember("Delete", _delete, allocator);
}

void Squash::readPlantInforamtion(rapidjson::Document* levelDataDocument, char* key, int i)
{
	_zombiePositionX = (*levelDataDocument)[key]["Plants"][to_string(i).c_str()]["ZombiePositionX"].GetFloat();
}

void Squash::setPlantOtherInformation(rapidjson::Document* levelDataDocument, char* key, int i)
{
	_delete = (*levelDataDocument)[key]["Plants"][to_string(i).c_str()]["Delete"].GetBool();
	if (_delete)
	{
		_healthPoint = 0;
		_isAttack = false;
		_isUsed = true;
		_plantAnimation->setVisible(false);
		_plantAnimation->stopAllActions();
	}
	else
	{
		_plantAnimation->setAnimation(0, "Squash_Normal", true);
	}
}
