/**
 *Copyright (c) 2021 LZ.All Right Reserved
 *Author : LZ
 *Date: 2021.6.19
 *Email: 2117610943@qq.com
 */

#include "LZMagnetShroom.h"
#include "Zombies/LZZombies.h"
#include "Scenes/GameScenes/Adventure/GameScene/LZAGSData.h"
#include "Scenes/GameScenes/Adventure/SelectPlantsScene/LZASPSSpriteLayer.h"
#include "Based/LZPlayMusic.h"

MagnetShroom::MagnetShroom(Node* node):
	_isCanAttack(true)
{
	_node = node;
	_plantImage = nullptr;
	_plantsType = PlantsType::MagnetShroom;
	_healthPoint = 300;
}

MagnetShroom::~MagnetShroom()
{
}

MagnetShroom* MagnetShroom::create(Node* node)
{
	MagnetShroom* magnetShroom = new (std::nothrow) MagnetShroom(node);
	if (magnetShroom && magnetShroom->init())
	{
		magnetShroom->autorelease();
		return magnetShroom;
	}
	CC_SAFE_DELETE(magnetShroom);
	return nullptr;
}

Sprite* MagnetShroom::createPlantImage()
{
	imageInit("MagnetShroom", INIT);
	_plantImage->setScale(1.3f);
	_plantImage->setAnchorPoint(Vec2(0.35f, 0.5f));
	return _plantImage;
}

void MagnetShroom::createPlantAnimation()
{
	_plantAnimation = plantInit("MagnetShroom", "MagnetShroom_Normal");
	_plantAnimation->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	_node->addChild(_plantAnimation);

	// 影子
	setPlantShadow(1.5f);

	// 泥土飞溅动画
	setPlantSoilSplashAnimation(1.0f);

	// 创建监听
	createListener();
}

void MagnetShroom::createListener()
{
	_plantAnimation->setEventListener([this](spTrackEntry* entry, spEvent* event)
		{
			if (!strcmp(event->data->name, "finished"))
			{
				_isCanAttack = true;
			}
		});
}

void MagnetShroom::determineRelativePositionPlantsAndZombies()
{
	for (auto zombie : ZombiesGroup)
	{
		zombieEatPlant(zombie);      /* 僵尸吃植物 */

		zombieRecoveryMove(zombie);  /* 僵尸恢复移动 */

		plantAttack(zombie);         /* 攻击僵尸 */
	}
}

void MagnetShroom::plantAttack(Zombies* zombie)
{
	if (_isCanAttack && zombie->getZombieIsHaveShield() &&              /* 可以攻击 && 僵尸有护盾  */
		getZombieIsInRange(zombie) && zombie->getZombieIsEnterMap())    /* 在攻击范围内 && 僵尸进入地图 */
	{
		if (zombie->getZombieBodyShieldType() == ShieldType::IronBodyShield)
		{
			zombie->setZombieCurrentBodyShieldVolume(0);
			zombie->setZombieShieldType(ShieldType::none, false);
			showShieldMoveAction(zombie, false);

			changeAnimation();
		}
		else if (zombie->getZombieHeadShieldType() == ShieldType::IronHeadShield)
		{
			zombie->setZombieCurrentHeadShieldVolume(0);
			zombie->setZombieShieldType(ShieldType::none, true);
			zombie->setZombieIsShowLoseShieldAnimation(false);
			showShieldMoveAction(zombie, true);

			changeAnimation();
		}
	}
}

void MagnetShroom::changeAnimation()
{
	PlayMusic::playMusic("magnetshroom");
	_plantAnimation->setAnimation(0, "MagnetShroom_Attack", false);
	_plantAnimation->addAnimation(0, "MagnetShroom_Recovery", false);
	_plantAnimation->addAnimation(0, "MagnetShroom_Normal", true);
	_isCanAttack = false;
}

bool MagnetShroom::getZombieIsInRange(Zombies* zombie)
{
	return fabs(zombie->getZombieAnimation()->getPositionX() - _plantAnimation->getPositionX()) < 333 &&
		fabs((zombie->getZombieAnimation()->getPositionY() + 50) - (_plantAnimation->getPositionY() + 60)) < 333;
}

void MagnetShroom::showShieldMoveAction(Zombies* zombie, const bool type)
{
	string name;
	switch (zombie->getZombieType())
	{
	case ZombiesType::Bucket4Door4Zombies:   name = type ? "Zombie_bucket4" : "Door2";  break;
	case ZombiesType::BucketDoorFlagZombies: name = type ? "Zombie_bucket" : "Door";    break;
	case ZombiesType::BucketDoorZombies:     name = type ? "Zombie_bucket" : "Door";    break;
	case ZombiesType::BucketFlagZombies:     name = "Zombie_bucket";                    break;
	case ZombiesType::BucketZombies:         name = "Zombie_bucket";                    break;
	case ZombiesType::BucketZombies2:        name = "Zombie_bucket2";                   break;
	case ZombiesType::BucketZombies3:        name = "Zombie_bucket3";                   break;
	case ZombiesType::BucketZombies4:        name = "Zombie_bucket4";                   break;
	case ZombiesType::BucketZombies5:        name = "Zombie_bucket5";                   break;
	case ZombiesType::BucketZombies6:        name = "Zombie_bucket6";                   break;
	case ZombiesType::CommonDoorFlagZombies: name = "Door";                             break;
	case ZombiesType::CommonDoorZombies:     name = "Door";                             break;
	case ZombiesType::ConeDoorFlagZombies:   name = "Door";                             break;
	case ZombiesType::ConeDoorZombies:       name = "Door";                             break;
	case ZombiesType::DoorZombies4:          name = "Doo2r";                            break;
	default:                                                                            break;
	}

	auto shield = Sprite::createWithSpriteFrameName(name + ".png");
	shield->setPosition(zombie->getZombieAnimation()->getPosition() + (type ? Vec2(0, 100) : Vec2(0, 50)));
	shield->setLocalZOrder(_plantAnimation->getLocalZOrder() + 1);
	shield->setScale(zombie->getZombieAnimation()->getScale());
	shield->runAction(Sequence::create(MoveTo::create(0.5f, _plantAnimation->getPosition() + Vec2(0, 100)),
		DelayTime::create(2.f),ScaleTo::create(7.f, 0.f),
		CallFunc::create([shield]()
			{
				shield->removeFromParent();
			}), nullptr));
	_node->addChild(shield);
}

SkeletonAnimation* MagnetShroom::showPlantAnimationAndText()
{
	auto& lta = _global->userInformation->getGameText();
	SPSSpriteLayer::plantCardTextScrollView->setInnerContainerSize(Size(lta.find("MAGNETSHROOM_1")->second->position));

	_isLoop = true;
	_plantAnimation = plantInit("MagnetShroom", "MagnetShroom_Normal");
	_plantAnimation->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	_plantAnimation->setScale(1.6f);
	_plantAnimation->setPosition(Vec2(200, 610));

	SPSSpriteLayer::createPlantsText(0, lta.find("MAGNETSHROOM_1")->second->text, Vec2(190, 910), lta.find("MAGNETSHROOM_1")->second->fontsize);
	SPSSpriteLayer::createPlantsText(1, lta.find("MAGNETSHROOM_2")->second->text, Vec2(360, 1000), lta.find("MAGNETSHROOM_2")->second->fontsize, Color3B::YELLOW, false);
	SPSSpriteLayer::createPlantsText(2, lta.find("MAGNETSHROOM_3")->second->text, Vec2(440, 1000), lta.find("MAGNETSHROOM_3")->second->fontsize, Color3B::RED, false);
	SPSSpriteLayer::createPlantsText(3, SPSSpriteLayer::selectRequirementText(lta, PlantsType::MagnetShroom, "MAGNETSHROOM_4", "MAGNETSHROOM_5"), Vec2(360, 870),
		lta.find("MAGNETSHROOM_5")->second->fontsize, SPSSpriteLayer::isPlantIsCanSelect[static_cast<unsigned int>(PlantsType::MagnetShroom)] ? Color3B::ORANGE : Color3B(255, 70, 0), false);

	return _plantAnimation;
}

void MagnetShroom::cavePlantInformation(rapidjson::Value& object, rapidjson::Document::AllocatorType& allocator)
{
	object.AddMember("IsCanAttack", _isCanAttack, allocator);
}

void MagnetShroom::readPlantInforamtion(rapidjson::Document* levelDataDocument, char* key, int i)
{
	_isCanAttack = (*levelDataDocument)[key]["Plants"][to_string(i).c_str()]["IsCanAttack"].GetBool();
}
