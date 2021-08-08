/**
 *Copyright (c) 2020 LZ.All Right Reserved
 *Author : LZ
 *Date: 2020.2.6
 *Email: 2117610943@qq.com
 */

#include "LZFumeShroom.h"
#include "Bullet/LZFumeShroomBullet.h"

#include "Zombies/LZZombies.h"
#include "Scenes/GameScenes/Adventure/GameScene/LZAGSData.h"


FumeShroom::FumeShroom(Node* node)
{
	_node = node;
	_plantImage = nullptr;

	_healthPoint = 300;
	_combatEffecttiveness = 0;
	_plantsType = PlantsType::FumeShroom;
}

FumeShroom::~FumeShroom()
{
}

FumeShroom* FumeShroom::create(Node* node)
{
	FumeShroom* fumeShroom = new (std::nothrow) FumeShroom(node);
	if (fumeShroom && fumeShroom->init())
	{
		fumeShroom->autorelease();
		return fumeShroom;
	}
	CC_SAFE_DELETE(fumeShroom);
	return nullptr;
}

Sprite* FumeShroom::createPlantImage()
{
	imageInit("FumeShroom", INIT);
	_plantImage->setScale(1.3f);
	_plantImage->setAnchorPoint(Vec2(0.4f, 0.5f));
	return _plantImage;
}

void FumeShroom::createPlantAnimation()
{
	_plantAnimation = plantInit("FumeShroom", "FumeShroom_Normal");
	_plantAnimation->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	_plantAnimation->setScale(1.0f);
	_node->addChild(_plantAnimation);

	// 影子
	setPlantShadow(2.f);
	_plantAnimation->getChildByName("plantshadow")->setPosition(Vec2(0, 20));

	// 泥土飞溅动画
	setPlantSoilSplashAnimation(0.8f);

	// 创建监听
	createListener("FumeShroom_Attack", "attack");
}

void FumeShroom::determineRelativePositionPlantsAndZombies()
{
	for (auto zombie : ZombiesGroup)
	{
		zombieEatPlant(zombie);      /* 僵尸吃植物 */

		zombieRecoveryMove(zombie);  /* 僵尸恢复移动 */

		plantAttack(zombie);         /* 植物攻击 */
	}

	plantRecovery("FumeShroom_Normal");
}

void FumeShroom::plantAttack(Zombies* zombie)
{
	if (getPlantIsSurvive() && zombie->getZombieIsSurvive() && zombie->getZombieIsEnterMap() &&     /* 植物存活 && 僵尸没有死亡 && 僵尸进入地图 */
		getZombieIsTheFrontOfPlant(zombie) && getZombieIsSameLineWithPlant(zombie) &&               /* 僵尸与植物在同一行 && 僵尸在植物的前方 */
		zombie->getZombieIsCanBeAttack())                                                           /* 僵尸是否可以被攻击到 */
	{
		plantEmission("FumeShroom_Attack");
	}
}

bool FumeShroom::getZombieIsTheFrontOfPlant(Zombies* zombie)
{
	return (zombie->getZombiePositionX() >= _plantAnimation->getPositionX()) && (zombie->getZombiePositionX() - _plantAnimation->getPositionX() < 600);
}
void FumeShroom::plantEmission(const string& plantAnimation)
{
	_isHaveZombies = true; /* 表示有僵尸与植物在同一行 */
	if (!_isChanged)     /* 判断动画是否已经改变 */
	{
		_plantAnimation->addAnimation(0, plantAnimation, true);
		_isChanged = true;
	}
}

void FumeShroom::plantRecovery(const string& plantAnimation)
{
	if (!_isHaveZombies) /* 判断是否有僵尸与豌豆射手在同一行 */
	{
		if (_isChanged)
		{
			_plantAnimation->addAnimation(0, plantAnimation, true);
			_isChanged = false;
		}
	}

	_isHaveZombies = false; /* 每循环一次就初始化 */
}

void FumeShroom::createBullet()
{
	_bulletAnimation = new FumeShroomBullet(_node);
	_bulletAnimation->setBulletPosition(_position);
	_bulletAnimation->setBulletInRow(_rowAndColumn.y);
	_bulletAnimation->createBullet();

	BulletGroup.push_back(_bulletAnimation);
}

SkeletonAnimation* FumeShroom::showPlantAnimationAndText()
{
	auto& lta = _global->userInformation->getGameText();
	SPSSpriteLayer::plantCardTextScrollView->setInnerContainerSize(Size(lta.find("FUMESHROOM_1")->second->position));

	_isLoop = true;
	_plantAnimation = plantInit("FumeShroom", "FumeShroom_Normal");
	_plantAnimation->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	_plantAnimation->setScale(1.8f);
	_plantAnimation->setPosition(Vec2(200, 600));

	SPSSpriteLayer::createPlantsText(0, lta.find("FUMESHROOM_1")->second->text, Vec2(190, 910), lta.find("FUMESHROOM_1")->second->fontsize);
	SPSSpriteLayer::createPlantsText(2, lta.find("FUMESHROOM_2")->second->text, Vec2(360, 1000), lta.find("FUMESHROOM_2")->second->fontsize, Color3B::YELLOW, false);
	SPSSpriteLayer::createPlantsText(3, lta.find("FUMESHROOM_3")->second->text, Vec2(440, 1000), lta.find("FUMESHROOM_3")->second->fontsize, Color3B::RED, false);
	SPSSpriteLayer::createPlantsText(1, SPSSpriteLayer::selectRequirementText(lta, PlantsType::FumeShroom, "FUMESHROOM_4", "FUMESHROOM_5"), Vec2(360, 870),
		lta.find("FUMESHROOM_4")->second->fontsize, SPSSpriteLayer::isPlantIsCanSelect[static_cast<unsigned int>(PlantsType::FumeShroom)] ? Color3B::ORANGE : Color3B(255, 70, 0), false);

	return _plantAnimation;
}

