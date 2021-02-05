/**
 *Copyright (c) 2020 LZ.All Right Reserved
 *Author : LZ
 *Date: 2020.2.6
 *Email: 2117610943@qq.com
 */

#include "LZPDTorchwood.h"

#include "Zombies/LZZZombies.h"
#include "Plants/EmissionPlants/Bullet/LZPEBPea.h"
#include "Plants/EmissionPlants/Bullet/LZPEBFirePea.h"
#include "Plants/EmissionPlants/Bullet/LZPEBIcePea.h"
#include "Scenes/GameScene/LZSGData.h"
#include "Scenes/SelectPlantsScene/LZSSSpriteLayer.h"
#include "Based/LZBPlayMusic.h"

Torchwood::Torchwood(Node* node):
	_playMusicMaxNumber(0)
{
	_node = node;
	_plantImage = nullptr;

	_combatEffecttiveness = 2.0f; // 使豌豆攻击力翻倍
	_healthPoint = 300;
	_plantsType = PlantsType::Torchwood;
}

Torchwood::~Torchwood()
{
}

Torchwood* Torchwood::create(Node* node)
{
	Torchwood* torchwood = new (std::nothrow) Torchwood(node);
	if (torchwood && torchwood->init())
	{
		torchwood->autorelease();
		return torchwood;
	}
	CC_SAFE_DELETE(torchwood);
	return nullptr;
}
Sprite* Torchwood::createPlantImage()
{
	imageInit("Torchwood", INIT);
	_plantImage->setScale(1.5f);
	_plantImage->setAnchorPoint(Vec2(0.4f, 0.5f));
	return _plantImage;
}

void Torchwood::createPlantAnimation()
{
	_plantAnimation = plantInit("Torchwood", "animation");
	_plantAnimation->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	_plantAnimation->setScale(1.0f);
	_node->addChild(_plantAnimation);

	// 影子
	setPlantShadow(1.5f);
	_plantAnimation->getChildByName("plantshadow")->setPosition(Vec2(0, 10));

	// 泥土飞溅动画
	setPlantSoilSplashAnimation(0.8f);

	//创建监听
	createListener();
}

void Torchwood::createListener()
{
	_plantAnimation->runAction(RepeatForever::create(Sequence::create(DelayTime::create(1.f), 
		CallFunc::create([&]() 
			{
				_playMusicMaxNumber = 0;
			}), nullptr)));
}

void Torchwood::determineRelativePositionPlantsAndZombies()
{
	for (auto zombie : ZombiesGroup)
	{
		zombieEatPlant(zombie);      /* 僵尸吃植物 */

		zombieRecoveryMove(zombie);  /* 僵尸恢复移动 */
	}

	judgeTorchwoodAndPeaPosition();
}

void Torchwood::judgeTorchwoodAndPeaPosition()
{
	for (auto bullet : BulletGroup)
	{
		if (!bullet->getBulletIsUsed()&& getBulletIsPea(bullet) && getPlantIsSurvive() &&    /* 子弹没有使用 && 子弹是豌豆 && 植物存活 */
			getPeaIsSameLineWithTorchwood(bullet) && getPeaIsEncounterTorchwood(bullet))     /* 豌豆与火炬树桩在同一行 && 豌豆遇到火炬树桩 */
		{
			if (dynamic_cast<Pea*>(bullet)->getTorchwoodTag() != _plantAnimation->getTag())
			{
				++_playMusicMaxNumber < 2 ? PlayMusic::playMusic(rand() % 2 ? "firepea" : "firepea1") : nullptr;
				bullet->setBulletOpacity();
				bullet->setBulletIsUsed(true);
				
				switch (bullet->getBulletType())
				{
				case BulletType::Pea:    createFirePea(bullet);  break;
				case BulletType::IcePea: createPea(bullet);      break;
				}
			}
		}
	}
}

void Torchwood::createFirePea(Bullet* bullet)
{
	FirePea* firePea = new FirePea(_node);
	firePea->setBulletPosition(bullet->getBulletPosition() - Vec2(70, 85));
	firePea->setBulletInRow(bullet->getBulletInRow());
	firePea->setPeaDirectionType(dynamic_cast<Pea*>(bullet)->getPeaDirectionType());
	firePea->calculateDirectionDistance(bullet->getBulletInitPosition() + Vec2(70, 85), bullet->getBulletPosition());
	firePea->createBullet();
	firePea->setTorchwoodTag(_plantAnimation->getTag());

	BulletGroup.push_back(firePea);
}

void Torchwood::createPea(Bullet* bullet)
{
	Pea* pea = new Pea(_node);
	pea->setBulletPosition(bullet->getBulletPosition() - Vec2(70, 85));
	pea->setBulletInRow(bullet->getBulletInRow());
	pea->setPeaDirectionType(dynamic_cast<Pea*>(bullet)->getPeaDirectionType());
	pea->calculateDirectionDistance(bullet->getBulletInitPosition() + Vec2(70, 85), bullet->getBulletPosition());
	pea->createBullet();
	pea->setTorchwoodTag(_plantAnimation->getTag());

	BulletGroup.push_back(pea);
}

bool Torchwood::getPeaIsSameLineWithTorchwood(Bullet* bullet)
{
	return getPlantRow() == bullet->getBulletInRow() ? true : false;
}

bool Torchwood::getPeaIsEncounterTorchwood(Bullet* bullet)
{
	return fabs(_plantAnimation->getPositionX() - bullet->getBulletPositionX()) <= 20 ? true : false;
}

bool Torchwood::getBulletIsPea(Bullet* bullet)
{
	return (bullet->getBulletType() == BulletType::Pea ||
		bullet->getBulletType() == BulletType::FirePea ||
		bullet->getBulletType() == BulletType::IcePea) ? true : false;
}

SkeletonAnimation* Torchwood::showPlantAnimationAndText()
{
	auto& lta = _global->userInformation->getGameText();
	SPSSpriteLayer::plantCardTextScrollView->setInnerContainerSize(Size(lta.find("TORCHWOOD_1")->second->position));

	_isLoop = true;
	_plantAnimation = plantInit("Torchwood", "animation");
	_plantAnimation->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	_plantAnimation->setScale(1.8f);
	_plantAnimation->setPosition(Vec2(200, 610));
	
	SPSSpriteLayer::createPlantsText(0, lta.find("TORCHWOOD_1")->second->text, Vec2(190, 910), lta.find("TORCHWOOD_1")->second->fontsize);
	SPSSpriteLayer::createPlantsText(2, lta.find("TORCHWOOD_2")->second->text, Vec2(360, 1000), lta.find("TORCHWOOD_2")->second->fontsize, Color3B::YELLOW, false);
	SPSSpriteLayer::createPlantsText(3, lta.find("TORCHWOOD_3")->second->text, Vec2(440, 1000), lta.find("TORCHWOOD_3")->second->fontsize, Color3B::RED, false);
	SPSSpriteLayer::createPlantsText(1, lta.find("TORCHWOOD_4")->second->text, Vec2(360, 870), lta.find("TORCHWOOD_4")->second->fontsize, Color3B::ORANGE, false);

	return _plantAnimation;
}
