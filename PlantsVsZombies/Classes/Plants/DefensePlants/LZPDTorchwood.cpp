/**
 *Copyright (c) 2020 LZ.All Right Reserved
 *Author : LZ
 *Date: 2020.2.6
 *Email: 2117610943@qq.com
 */

#include "LZPDTorchwood.h"

#include "Zombies/LZZZombies.h"
#include "Plants/EmissionPlants/Bullet/LZPEBPea.h"
#include "Scenes/GameScene/LZSGData.h"
#include "Scenes/SelectPlantsScene/LZSSSpriteLayer.h"
#include "Based/LZBPlayMusic.h"

Torchwood::Torchwood(Node* node)
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
		if (getBulletIsPea(bullet) && getPlantIsSurvive() &&                                 /* 子弹是豌豆 && 植物存活 */
			getPeaIsSameLineWithTorchwood(bullet) && getPeaIsEncounterTorchwood(bullet))     /* 豌豆与火炬树桩在同一行 && 豌豆遇到火炬树桩 */
		{
			PlayMusic::playMusic("firepea");
			if (dynamic_cast<Pea*>(bullet)->getIsIce() && dynamic_cast<Pea*>(bullet)->getTorchwoodTag() != _plantAnimation->getTag())        // 如果豌豆被冰冻
			{
				bullet->getBullet()->setAnimation(0, "normal", true);
				dynamic_cast<Pea*>(bullet)->setTorchwoodTag(_plantAnimation->getTag());
				dynamic_cast<Pea*>(bullet)->addPeaFireNumbers();
				dynamic_cast<Pea*>(bullet)->setIsIce(false);
			}
			else if (!dynamic_cast<Pea*>(bullet)->getIsFire() && dynamic_cast<Pea*>(bullet)->getTorchwoodTag() != _plantAnimation->getTag()) // 如果豌豆没有燃烧
			{
				bullet->getBullet()->setAnimation(0, "fire", true);
				bullet->getBullet()->getChildByName("shadow")->setPosition(Vec2(0, -52));
				bullet->getBullet()->getChildByName("shadow")->setScaleY(0.7f);
				bullet->getBullet()->getChildByName("shadow")->setScaleX(1.0f);
				bullet->getBullet()->runAction(ScaleTo::create(0.2f, 1.5f));
				bullet->setBulletAttack(bullet->getBulletAttack() * _combatEffecttiveness);
				dynamic_cast<Pea*>(bullet)->setTorchwoodTag(_plantAnimation->getTag());
				dynamic_cast<Pea*>(bullet)->addPeaFireNumbers();
				dynamic_cast<Pea*>(bullet)->setIsFire(true);
			}
			else if (dynamic_cast<Pea*>(bullet)->getIsFire() && dynamic_cast<Pea*>(bullet)->getTorchwoodTag() != _plantAnimation->getTag())
			{
				dynamic_cast<Pea*>(bullet)->setTorchwoodTag(_plantAnimation->getTag());
				dynamic_cast<Pea*>(bullet)->addPeaFireNumbers();
			}

			makePeaFire(bullet);
		}
	}
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

void Torchwood::makePeaFire(Bullet* bullet)
{
	if (dynamic_cast<Pea*>(bullet)->getPeaFireNumbers() > 1)
	{
		bullet->setBulletAttack(0);
		bullet->setBulletIsUsed(true);
		bullet->getBullet()->setOpacity(0);
		bullet->getBullet()->getChildByName("shadow")->setVisible(false);
	}
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
	SPSSpriteLayer::createPlantsText(1, lta.find("TORCHWOOD_4")->second->text, Vec2(360, 870), lta.find("TORCHWOOD_4")->second->fontsize, Color3B::YELLOW, false);

	return _plantAnimation;
}
