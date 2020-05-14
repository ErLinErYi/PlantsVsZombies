/**
 *Copyright (c) 2020 LZ.All Right Reserved
 *Author : LZ
 *Date: 2020.2.5
 *Email: 2117610943@qq.com
 */

#include "Plants.h"
#include "EmissionPlants/Bullet/Bullet.h"

#include "Zombies/Zombies.h"
#include "Scenes/GameScene/GSData.h"

int Plants::plantNumber = -1;
GLProgram* Plants::_normalGLProgram = nullptr;
GLProgram* Plants::_highLightGLProgram = nullptr;

Plants::Plants(Node* node, const Vec2& position) :
	_node(node)
,	_position(position)
,	_plantShadow(nullptr)
,	_plantImage(nullptr)
,	_plantAnimation(nullptr)
,	_soilSplashAnimation(nullptr)
,   _highLightGLProgramState(nullptr)
,	_sunNeed(NOINITIALIZATION)
,	_coolDownTime(NOINITIALIZATION)
,	_costGold(NOINITIALIZATION)
,	_costMasonry(NOINITIALIZATION)
,	_healthPoint(NOINITIALIZATION)
,	_plantTag(NOINITIALIZATION)
,	_isLoop(true)
,   _isCanDelete{ false,false }
,   _zOrder(0)
,   _highLightIntensity(0.6f)
,   _plantNumber(++plantNumber)
,   _combatEffecttiveness(0)
,   _plantsType(PlantsType::None)
,	_global(Global::getInstance())
{
}

Plants::~Plants()
{
}

SkeletonAnimation* Plants::plantInit(const std::string& plantname, const std::string& animaionname)
{
	auto iter = _global->userInformation->getAnimationData().find(plantname);
	if (iter != _global->userInformation->getAnimationData().end())/* 如果可以找到 */
	{
		_plantAnimation = SkeletonAnimation::createWithData(iter->second);
		_plantAnimation->setPosition(_position);
		_plantAnimation->setAnimation(0, animaionname, _isLoop);
		_plantAnimation->setLocalZOrder(_zOrder);
		_plantAnimation->setTag(_plantTag);
		_plantAnimation->setTimeScale(0.7f + rand() % 4 / 10.f);
		setPlantGLProgram();
		return _plantAnimation;
	}
	return nullptr;
}

void Plants::imageInit(const std::string& name, const Vec2& position)
{
	_plantImage = Sprite::createWithSpriteFrameName(name + ".png");
	_plantImage->setPosition(position);
	_plantImage->setName("Preview");
	_plantImage->setOpacity(150);
	_node->addChild(_plantImage, 99);
}

void Plants::determineRelativePositionPlantsAndZombies()
{
	for (auto zombie : ZombiesGroup)
	{
		zombieEatPlant(zombie);      /* 僵尸吃植物 */

		zombieRecoveryMove(zombie);  /* 僵尸恢复移动 */
	}
}

void Plants::stopPlantsAllAction()
{
	for (auto& sunFlower : PlantsGroup)
	{
		sunFlower.second->getPlantAnimation()->stopAllActions();
	}
}

void Plants::setPlantSoilSplashAnimation(const float& scale)
{
	/* 创建种植泥土飞溅动画 */
	auto iter = _global->userInformation->getAnimationData().find("SplashOfSoil");
	if (iter != _global->userInformation->getAnimationData().end())
	{
		_soilSplashAnimation = SkeletonAnimation::createWithData(iter->second);
		_soilSplashAnimation->setPosition(Vec2(0, -50));
		_soilSplashAnimation->setAnimation(0, "SplashOfSoil", false);
		_soilSplashAnimation->setScale(scale);
		_soilSplashAnimation->setName("SplashOfSoil");
		_soilSplashAnimation->setGlobalZOrder(_plantAnimation->getGlobalZOrder());
		_plantAnimation->addChild(_soilSplashAnimation);
	}
}

void Plants::setPlantShadow(const float& scale)
{
	/* 创建植物影子 */
	_plantShadow = Sprite::createWithSpriteFrameName("plantshadow.png");
	_plantShadow->setScale(scale);
	_plantShadow->setPosition(Vec2(-5, 15));
	_plantShadow->setName("plantshadow");
	_plantAnimation->addChild(_plantShadow, -1);
}

void Plants::setPlantHurtBlink()
{
	auto action = Repeat::create(Sequence::create(
		CallFunc::create([this]()
			{
				_highLightIntensity -= 0.01f;
				_highLightGLProgramState->setUniformFloat("intensity", _highLightIntensity);
			}), DelayTime::create(0.005f), nullptr), 50);

	_plantAnimation->runAction(Sequence::create(
		CallFunc::create([this]()
			{
				_plantAnimation->setGLProgram(_highLightGLProgram);
				_highLightGLProgramState = _plantAnimation->getGLProgramState();
				_highLightGLProgramState->setUniformFloat("intensity", 0.6f);
			}), DelayTime::create(0.15f),
		CallFunc::create([this]()
			{
				_plantAnimation->setGLProgram(_normalGLProgram);
				_highLightIntensity = 0.6f;
			}), nullptr));
}

void Plants::setPlantHurtBlink(PlantsType type) const
{
	_plantAnimation->runAction(Sequence::create(TintTo::create(0.15f, Color3B(70, 70, 70)), TintTo::create(0.15f, Color3B::WHITE), nullptr));
}

void Plants::setPlantVisible(const bool visible)
{
	_plantAnimation->setVisible(visible);
}

void Plants::setPlantScale()
{
	_plantAnimation->setScale(_plantAnimation->getScale() + (4 - _plantTag / 10) / 20.f);
}

bool Plants::getPlantIsSurvive() const
{
	if (_healthPoint > 0 && _plantAnimation->isVisible())
		return true;
	return false;
}

bool Plants::getPlantVisible()
{
	return _plantAnimation->isVisible();
}

bool* Plants::getPlantIsCanDelete()
{
	return _isCanDelete;
}

bool Plants::getZombieIsTheFrontOfPlant(Zombies* zombie) const
{
	return zombie->getZombiePositionX() >= _plantAnimation->getPositionX() ? true : false;
}

bool Plants::getZombieIsSameLineWithPlant(Zombies* zombie) const
{
	return fabs(zombie->getZombiePositionY() - 10 - _plantAnimation->getPositionY()) <= 10 ? true : false;
}

bool Plants::getzombieIsEncounterPlant(Zombies* zombie) const
{
	return fabs(zombie->getZombiePositionX() - _plantAnimation->getPositionX()) <= 80 ? true : false;
}

void Plants::zombieEatPlant(Zombies* zombie)
{
	if (getPlantIsSurvive() && getZombieIsSameLineWithPlant(zombie) && getzombieIsEncounterPlant(zombie)&& /* 植物存活 && 植物与僵尸在同一行 && 僵尸遇到植物 */
		zombie->getZombieType() != ZombiesType::SnowZombies) /* 僵尸不是雪人僵尸 */
	{
		if (zombie->getZombieIsSurvive() && !zombie->getZombieIsEat())
		{
			const string eateffect[3] = { "chomp","chomp2","chompsoft" };
			zombie->setZombieEatPlantNumber(_plantNumber);
			zombie->setZombieStop();
			zombie->setZombieIsEat(true);
			zombie->getZombieAnimation()->setAnimation(1,
				zombie->getZombieType() == ZombiesType::LmpZombies ? "Zombies_Eat" : rand() % 2 ? "Zombies_Eat" : "Zombies_Eat1", true);
			zombie->getZombieAnimation()->setEventListener([this, eateffect](spTrackEntry* entry, spEvent* event)
				{
					if (!strcmp(event->data->name, "eat"))
					{
						if (event->intValue == 1)
						{
							reducePlantHealthPoint(100);
							Bullet::playSoundEffect(eateffect[rand() % 3]);
							setPlantHurtBlink();
						}
					}
				});
		}
	}
}

void Plants::zombieRecoveryMove(Zombies* zombie)
{
	if (!getPlantIsSurvive() && zombie->getZombieEatPlantNumber() == _plantNumber &&  /* 植物死亡 && 僵尸是吃的该植物 */
		zombie->getZombieIsEat() && zombie->getZombieIsStop()) /* 僵尸正在吃植物 && 僵尸正在停止移动 */
	{
		setPlantVisible(false);
		_plantAnimation->stopAllActions();
		Bullet::playSoundEffect("gulp");
		zombie->setZombieIsEat(false);
		if (!zombie->getZombieIsPlayDieAnimation()) /* 僵尸没有播放死亡动画 */
		{
			zombie->getZombieAnimation()->setMix("Zombies_Eat", Zombies::getZombieAniamtionName(zombie->getZombieType()), 1.5f);
			zombie->getZombieAnimation()->addAnimation(1, Zombies::getZombieAniamtionName(zombie->getZombieType()), true);
			zombie->setZombieCurrentSpeed(zombie->getZombieSpeed());
		}
	}
}

void Plants::setPlantGLProgram()
{
	if (!_normalGLProgram || !_highLightGLProgram)
	{
		_normalGLProgram = _plantAnimation->getGLProgram();
		_highLightGLProgram = Zombies::getHighLight();
	}
}
