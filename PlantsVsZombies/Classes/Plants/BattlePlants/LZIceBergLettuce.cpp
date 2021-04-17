/**
 *Copyright (c) 2021 LZ.All Right Reserved
 *Author : LZ
 *Date: 2021.1.30
 *Email: 2117610943@qq.com
 */

#include "LZIceBergLettuce.h"
#include "Zombies/LZZombies.h"
#include "Scenes/GameScenes/Adventure/GameScene/LZAGSData.h"
#include "Based/LZPlayMusic.h"

IceBergLettuce::IceBergLettuce(Node* node):
	_isHaveZombies(false),
	_isChanged(false),
	_excludeZombieZorder(0),
	_excludeZombiePosition(Vec2::ZERO),
	_excludeZombie(nullptr)
{
	_node = node;
	_plantImage = nullptr;

	_healthPoint = 300;
	_isLoop = false;
	_combatEffecttiveness = 1800;
	_plantsType = PlantsType::IceBergLettuce;
}

IceBergLettuce::~IceBergLettuce()
{
}

IceBergLettuce* IceBergLettuce::create(Node* node)
{
	IceBergLettuce* iceBergLettuce = new (std::nothrow) IceBergLettuce(node);
	if (iceBergLettuce && iceBergLettuce->init())
	{
		iceBergLettuce->autorelease();
		return iceBergLettuce;
	}
	CC_SAFE_DELETE(iceBergLettuce);
	return nullptr;
}

Sprite* IceBergLettuce::createPlantImage()
{
	imageInit("IceBergLettuce", INIT);
	_plantImage->setScale(1.30f);
	_plantImage->setAnchorPoint(Vec2(0.4f, 0.65f));
	return _plantImage;
}

void IceBergLettuce::createPlantAnimation()
{
	_plantAnimation = plantInit("IceBergLettuce", "IceBergLettuce_Out");
	_plantAnimation->addAnimation(0, "IceBergLettuce_Normal", true);
	_plantAnimation->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	_plantAnimation->setScale(1.2f);
	_node->addChild(_plantAnimation);

	// 影子
	setPlantShadow(1.8f);
	_plantAnimation->getChildByName("plantshadow")->setPosition(Vec2(0, 5));

	// 泥土飞溅动画
	setPlantSoilSplashAnimation(0.8f);

	// 事件监听
	createListener();
}

void IceBergLettuce::createListener()
{
	_plantAnimation->setEventListener([=](spTrackEntry* entry, spEvent* event)
		{
			if (strcmp(event->data->name, "explode") == 0)
			{
				if (!_isChanged)
				{
					_isChanged = true;
					setZombiesActionStop(_excludeZombie, 10);
					explodeEffectZombies(0);
				}
				else
				{
					explodeEffectZombies(0.5f);
				}
			}
		});

	_plantAnimation->setCompleteListener([this](spTrackEntry* entry)
		{
			if (strcmp(entry->animation->name, "IceBergLettuce_Skill") == 0)
			{
				_plantAnimation->runAction(Sequence::create(DelayTime::create(0.2f),
					CallFunc::create([this]()
						{
							_plantAnimation->setVisible(false);
						}), nullptr));
			}
		});
}

void IceBergLettuce::determineRelativePositionPlantsAndZombies()
{
	for (auto zombie : ZombiesGroup)
	{
		plantExplode(zombie);        /* 植物攻击 */
	}
}

void IceBergLettuce::plantExplode(Zombies* zombie)
{
	if (getPlantIsSurvive() && zombie->getZombieIsSurvive() && zombie->getZombieIsCanBeAttack() &&         /* 植物存活 && 僵尸存活 && 僵尸可以被攻击到 */
		getZombieIsSameLineWithPlant(zombie) && getZombieIsEncounterPlant(zombie))                         /* 植物与僵尸在同一行 && 僵尸遇到植物 */
	{
		if (!_isHaveZombies)
		{
			PlayMusic::playMusic("frozen");
			_isHaveZombies = true;
			_excludeZombie = zombie;
			_excludeZombieZorder = zombie->getZombieAnimation()->getLocalZOrder() + 1;
			_excludeZombiePosition = zombie->getZombieAnimation()->getPosition() - Vec2(30, 10);

			_plantAnimation->setAnimation(0, "IceBergLettuce_Skill", false);
			_plantAnimation->setLocalZOrder(_plantAnimation->getLocalZOrder() + 100);
		}
	}
}

void IceBergLettuce::explodeEffectZombies(const float time)
{
	auto iter = _global->userInformation->getAnimationData().find("IceBergLettuceEffect");
	if (iter != _global->userInformation->getAnimationData().end())/* 如果可以找到 */
	{
		auto iceEffect = SkeletonAnimation::createWithData(iter->second);
		iceEffect->setPosition(_excludeZombiePosition);
		iceEffect->setAnimation(0, "animation", false);
		iceEffect->setScaleX(-iceEffect->getScaleX());
		iceEffect->setLocalZOrder(_excludeZombieZorder);
		iceEffect->update(time);
		_node->addChild(iceEffect);

		iceEffect->runAction(Sequence::create(DelayTime::create(2.f), FadeOut::create(8.f),
			CallFunc::create([=]()
				{
					iceEffect->removeFromParent();
				}), nullptr));
	}
}

void IceBergLettuce::setZombiesActionStop(Zombies* zombie,const int &time)
{
	if (zombie && zombie->getZombieType() != ZombiesType::SnowZombies)
	{
		zombie->setZombieActionStop();
		zombie->createZombieTimer();
		zombie->setZombieTimerTime(time);
	}
}

bool IceBergLettuce::getZombieIsInExplodeRange(Zombies* zombie)
{
	return (zombie->getZombieIsSurvive() && zombie->getZombieIsEnterMap()) ? true : false;
}

SkeletonAnimation* IceBergLettuce::showPlantAnimationAndText()
{
	auto& lta = _global->userInformation->getGameText();
	SPSSpriteLayer::plantCardTextScrollView->setInnerContainerSize(Size(lta.find("ICEBERGLETTUCE_1")->second->position));

	_isLoop = true;
	_plantAnimation = plantInit("IceBergLettuce", "IceBergLettuce_Normal");
	_plantAnimation->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	_plantAnimation->setScale(1.8f);
	_plantAnimation->setPosition(Vec2(200, 610));

	SPSSpriteLayer::createPlantsText(0, lta.find("ICEBERGLETTUCE_1")->second->text, Vec2(190, 910), lta.find("ICEBERGLETTUCE_1")->second->fontsize);
	SPSSpriteLayer::createPlantsText(2, lta.find("ICEBERGLETTUCE_2")->second->text, Vec2(360, 1000), lta.find("ICEBERGLETTUCE_2")->second->fontsize, Color3B::YELLOW, false);
	SPSSpriteLayer::createPlantsText(3, lta.find("ICEBERGLETTUCE_3")->second->text, Vec2(440, 1000), lta.find("ICEBERGLETTUCE_3")->second->fontsize, Color3B::RED, false);
	SPSSpriteLayer::createPlantsText(1, SPSSpriteLayer::selectRequirementText(lta, PlantsType::IceBergLettuce, "ICEBERGLETTUCE_4", "ICEBERGLETTUCE_5"), Vec2(360, 870),
		lta.find("ICEBERGLETTUCE_4")->second->fontsize, SPSSpriteLayer::isPlantIsCanSelect[static_cast<unsigned int>(PlantsType::IceBergLettuce)] ? Color3B::ORANGE : Color3B(255, 70, 0), false);

	return _plantAnimation;
}

void IceBergLettuce::cavePlantInformation(rapidjson::Value& object, rapidjson::Document::AllocatorType& allocator)
{
	object.AddMember("IsChanged", _isChanged, allocator);
	object.AddMember("IsHaveZombies", _isHaveZombies, allocator);
	object.AddMember("ExcludeZombieZorder", _excludeZombieZorder, allocator);
	object.AddMember("ExcludeZombiePositionX", _excludeZombiePosition.x, allocator);
	object.AddMember("ExcludeZombiePositionY", _excludeZombiePosition.y, allocator);
}

void IceBergLettuce::readPlantInforamtion(rapidjson::Document* levelDataDocument, char* key, int i)
{
	_isChanged = (*levelDataDocument)[key]["Plants"][to_string(i).c_str()]["IsChanged"].GetBool();
	_isHaveZombies = (*levelDataDocument)[key]["Plants"][to_string(i).c_str()]["IsHaveZombies"].GetBool();
	_excludeZombieZorder = (*levelDataDocument)[key]["Plants"][to_string(i).c_str()]["ExcludeZombieZorder"].GetInt();
	_excludeZombiePosition.x = (*levelDataDocument)[key]["Plants"][to_string(i).c_str()]["ExcludeZombiePositionX"].GetFloat();
	_excludeZombiePosition.y = (*levelDataDocument)[key]["Plants"][to_string(i).c_str()]["ExcludeZombiePositionY"].GetFloat();
}
