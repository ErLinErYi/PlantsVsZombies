/**
 *Copyright (c) 2021 LZ.All Right Reserved
 *Author : LZ
 *Date: 2021.6.18
 *Email: 2117610943@qq.com
 */

#include "LZBlover.h"
#include "Zombies/LZZombies.h"
#include "Scenes/GameScenes/Adventure/GameScene/LZAGSData.h"
#include "Based/LZPlayMusic.h"

Blover::Blover(Node* node)
{
	_node = node;
	_plantImage = nullptr;

	_healthPoint = 300;
	_isLoop = false;
	_plantsType = PlantsType::Blover;
}

Blover::~Blover()
{
}

Blover* Blover::create(Node* node)
{
	Blover* blover = new (std::nothrow) Blover(node);
	if (blover && blover->init())
	{
		blover->autorelease();
		return blover;
	}
	CC_SAFE_DELETE(blover);
	return nullptr;
}

Sprite* Blover::createPlantImage()
{
	imageInit("Blover", INIT);
	_plantImage->setScale(1.5f);
	_plantImage->setAnchorPoint(Vec2(0.45f, 0.42f));
	return _plantImage;
}

void Blover::createPlantAnimation()
{
	_plantAnimation = plantInit("Blovers", "animation");
	_plantAnimation->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	_plantAnimation->setScale(0.8f);
	_node->addChild(_plantAnimation);

	// 影子
	setPlantShadow(1.8f);

	// 泥土飞溅动画
	setPlantSoilSplashAnimation(1.f);

	// 创建监听
	createListener();
}

void Blover::createListener()
{
	_plantAnimation->setEventListener([this](spTrackEntry* entry, spEvent* event)
		{
			if (!strcmp(event->data->name, "finished"))
			{
				_healthPoint = 0;
				_plantAnimation->setVisible(false);
			}
			if (!strcmp(event->data->name, "begin"))
			{
				PlayMusic::playMusic("blover");
				plantAttack(nullptr);
			}
		});
}

void Blover::determineRelativePositionPlantsAndZombies()
{
	for (auto zombie : ZombiesGroup)
	{
		zombieEatPlant(zombie);      /* 僵尸吃植物 */

		zombieRecoveryMove(zombie);  /* 僵尸恢复移动 */
	}
}

void Blover::plantAttack(Zombies*)
{
	for (auto zombie : ZombiesGroup)
	{
		if (zombie->getZombieType() == ZombiesType::BalloonZombies && /*getZombieIsTheFrontOfPlant(zombie) &&*/     /* 僵尸是气球僵尸 && 僵尸在植物前方 */
			getZombieIsSameLineWithPlant(zombie) && zombie->getZombieIsSurvive() && zombie->getZombieIsEnterMap())  /* 僵尸与植物在同一行 && 僵尸没有死亡 && 僵尸进入地图 */
		{
			zombie->getZombieAnimation()->runAction(Sequence::create(MoveBy::create(1.f, Vec2(1500, 0)),
				CallFunc::create([=]()
					{
						zombie->setZombieDeath(true);
					}), nullptr));
		}
	}
}

SkeletonAnimation* Blover::showPlantAnimationAndText()
{
	auto& lta = _global->userInformation->getGameText();
	SPSSpriteLayer::plantCardTextScrollView->setInnerContainerSize(Size(lta.find("BLOVER_1")->second->position));

	_isLoop = true;
	_plantAnimation = plantInit("Blovers", "animation");
	_plantAnimation->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	_plantAnimation->setScale(1.1f);
	_plantAnimation->setPosition(Vec2(190, 610));

	SPSSpriteLayer::createPlantsText(0, lta.find("BLOVER_1")->second->text, Vec2(190, 910), lta.find("BLOVER_1")->second->fontsize);
	SPSSpriteLayer::createPlantsText(2, lta.find("BLOVER_2")->second->text, Vec2(360, 1000), lta.find("BLOVER_2")->second->fontsize, Color3B::YELLOW, false);
	SPSSpriteLayer::createPlantsText(3, lta.find("BLOVER_3")->second->text, Vec2(440, 1000), lta.find("BLOVER_3")->second->fontsize, Color3B::RED, false);
	SPSSpriteLayer::createPlantsText(1, SPSSpriteLayer::selectRequirementText(lta, PlantsType::Blover, "BLOVER_4", "BLOVER_5"), Vec2(360, 870),
		lta.find("BLOVER_4")->second->fontsize, SPSSpriteLayer::isPlantIsCanSelect[static_cast<unsigned int>(PlantsType::Blover)] ? Color3B::ORANGE : Color3B(255, 70, 0), false);

	return _plantAnimation;
}