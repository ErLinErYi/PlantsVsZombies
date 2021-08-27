/**
 *Copyright (c) 2020 LZ.All Right Reserved
 *Author : LZ
 *Date: 2020.2.6
 *Email: 2117610943@qq.com
 */

#include "LZPotatoMine.h"

#include "Zombies/LZZombies.h"
#include "Scenes/GameScenes/Adventure/GameScene/LZAGSData.h"
#include "Scenes/GameScenes/Adventure/GameScene/LZAGSBackgroundLayer.h"
#include "Scenes/GameScenes/Adventure/GameScene/LZAGSControlLayer.h"
#include "Based/LZPlayMusic.h"

PotatoMine::PotatoMine(Node* node):
	_isReady(false)
,   _isBeginExplode(false)
,   _isExplodeFinished(false)
,   _isCanKillZombies(false)
,   _isKillZombiesFinished(false)
,   _isReadFile(false)
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
	_plantImage->setScale(1.1f);
	_plantImage->setAnchorPoint(Vec2(0.5f, 0.75f));
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

void PotatoMine::setPlantNoramlAnimation()
{
	_plantAnimation->setAnimation(0, "PotatoMine_Normal", true);
}

void PotatoMine::createListener()
{
	if (_breakGround > 0)
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
	else
	{
		_isReady = true;
	}
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
		
		_isReadFile ? nullptr : _plantAnimation->setAnimation(0, "PotatoMine_Blast", false);
		_plantAnimation->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
		_plantAnimation->setScale(1.2f);
		_plantAnimation->setLocalZOrder(_plantAnimation->getLocalZOrder() + 100);
		_plantAnimation->setEventListener([=](spTrackEntry* entry, spEvent* event)
			{
				if (strcmp(event->data->name, "BlastBegin") == 0)
				{
					_isCanKillZombies = true;
					_plantAnimation->getChildByName("plantshadow")->setVisible(false);
					GSBackgroundLayer::backgroundRunAction();
				}
			});
		_plantAnimation->runAction(Sequence::create(DelayTime::create(2.f),
			CallFunc::create([this]()
				{
					_plantAnimation->setVisible(false);
				}), nullptr));
	}

	if (_isCanKillZombies&& !_isKillZombiesFinished)
	{
		explodeHurtZombies();
		_isKillZombiesFinished = true;
	}
}

void PotatoMine::zombieEatPlant(Zombies* zombie)
{
	if (getPlantIsSurvive() && zombie->getZombieIsSurvive() && zombie->getZombieIsCanBeAttack() &&       /* 植物存活 && 僵尸存活 && 僵尸可以被攻击到 */
		getZombieIsSameLineWithPlant(zombie) && getZombieIsEncounterPlant(zombie))                       /* 植物与僵尸在同一行 && 僵尸遇到植物 */
	{
		if (_isReady)  /* 土豆雷准备好 */
		{
			_isBeginExplode = true;
		}

		if (zombie->getZombieIsEatPlants()) /* 僵尸吃植物 */
		{
			setZombieEatPlantControl(zombie);
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
			zombie->setZombieHurtBlink();

			if (!zombie->getZombieIsSurvive() && zombie->getZombieType() != ZombiesType::GargantuarZombies)
			{
				zombie->setZombieOpacity(0);
				zombie->setZombieVisible(false);
			}
		}
	}
}

bool PotatoMine::getZombieIsInExplodeRange(Zombies* zombie)
{
	return (getZombieIsSameLineWithPlant(zombie) && fabs(zombie->getZombieAnimation()->getPositionX() - _plantAnimation->getPositionX()) < 130) ? true : false;
}

void PotatoMine::cavePlantInformation(rapidjson::Value& object, rapidjson::Document::AllocatorType& allocator)
{
	object.AddMember("BreakGround", _breakGround, allocator);
}

void PotatoMine::readPlantInforamtion(rapidjson::Document* levelDataDocument, char* key, int i)
{
	_breakGround = (*levelDataDocument)[key]["Plants"][to_string(i).c_str()]["BreakGround"].GetFloat();

	if (!strcmp((*levelDataDocument)[key]["Plants"][to_string(i).c_str()]["1"]["PlantsAnimationName"].GetString(), "PotatoMine_Blast"))
	{
		_isReadFile = true;
	}
}

SkeletonAnimation* PotatoMine::showPlantAnimationAndText()
{
	auto& lta = _global->userInformation->getGameText();
	SPSSpriteLayer::plantCardTextScrollView->setInnerContainerSize(Size(lta.find("POTATOMINE_1")->second->position));

	_isLoop = true;
	_plantAnimation = plantInit("PotatoMine", "PotatoMine_Normal");
	_plantAnimation->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	_plantAnimation->setScale(1.8f);
	_plantAnimation->setPosition(Vec2(200, 610));

	SPSSpriteLayer::createPlantsText(0, lta.find("POTATOMINE_1")->second->text, Vec2(190, 910), lta.find("POTATOMINE_1")->second->fontsize);
	SPSSpriteLayer::createPlantsText(2, lta.find("POTATOMINE_2")->second->text, Vec2(360, 1000), lta.find("POTATOMINE_2")->second->fontsize, Color3B::YELLOW, false);
	SPSSpriteLayer::createPlantsText(3, lta.find("POTATOMINE_3")->second->text, Vec2(440, 1000), lta.find("POTATOMINE_3")->second->fontsize, Color3B::RED, false);
	SPSSpriteLayer::createPlantsText(1, lta.find("POTATOMINE_4")->second->text, Vec2(360, 870), lta.find("POTATOMINE_4")->second->fontsize, Color3B::ORANGE, false);

	return _plantAnimation;
}
