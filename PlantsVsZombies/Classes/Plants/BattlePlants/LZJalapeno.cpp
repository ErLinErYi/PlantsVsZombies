/**
 *Copyright (c) 2020 LZ.All Right Reserved
 *Author : LZ
 *Date: 2020.2.6
 *Email: 2117610943@qq.com
 */

#include "LZJalapeno.h"

#include "Zombies/LZZombies.h"
#include "Scenes/GameScenes/Adventure/GameScene/LZAGSData.h"
#include "Scenes/GameScenes/Adventure/GameScene/LZAGSBackgroundLayer.h"
#include "Scenes/GameScenes/BigMap/GameScene/LZBigMapGameScene.h"
#include "Based/LZPlayMusic.h"

Jalapeno::Jalapeno(Node* node)
{
	_node = node;
	_plantImage = nullptr;

	_healthPoint = 300;
	_isLoop = false;
	_combatEffecttiveness = 1800;
	_plantsType = PlantsType::Jalapeno;
}

Jalapeno::~Jalapeno()
{
}

Jalapeno* Jalapeno::create(Node* node)
{
	Jalapeno* jalapeno = new (std::nothrow) Jalapeno(node);
	if (jalapeno && jalapeno->init())
	{
		jalapeno->autorelease();
		return jalapeno;
	}
	CC_SAFE_DELETE(jalapeno);
	return nullptr;
}
Sprite* Jalapeno::createPlantImage()
{
	imageInit("Jalapeno", INIT);
	_plantImage->setScale(1.4f);
	return _plantImage;
}

void Jalapeno::createPlantAnimation()
{
	_plantAnimation = plantInit("Jalapeno", "Jalapeno_Explode");
	_plantAnimation->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	_plantAnimation->setTimeScale(1.5f);
	_node->addChild(_plantAnimation);

	// 影子
	setPlantShadow(1.8f);

	// 泥土飞溅动画
	setPlantSoilSplashAnimation(0.8f);

	// 事件监听
	createListener();
}

void Jalapeno::plantExplode()
{
	if (!getPlantIsSurvive() || getPlantIsReadyExplode()) /* 植物死亡 || 植物准备爆炸 */
	{
		if (!_isExplode) /* 如果没有爆炸 */
		{
			_isExplode = true;
			explodeHurtZombies();
			showExplodeAnimation();
		}
	}
}

void Jalapeno::showExplodeAnimation()
{
	PlayMusic::playMusic("jalapeno");

	GSBackgroundLayer::backgroundRunAction();

	unsigned int row = 9;
	float left = GRASS_POSITION_LEFT;
	float hight = 122;
	float bottom = _plantAnimation->getPositionY() - 10;
	float scale = 1.f;
	if (BigMapGameScene::bigMapWorld)
	{
		row = 18;
		left = GRASS_BIGMAP_POSITION_LEFT - 20;
		hight = 77;
		bottom = _plantAnimation->getPositionY() - 35;
		scale = 0.6f;
	}

	for (unsigned int i = 0; i < row; i++)
	{
		auto jalapenoFire = SkeletonAnimation::createWithData(_global->userInformation->getAnimationData().find("Jalapeno_Fire")->second);
		jalapenoFire->setPosition(Vec2(left + hight * i + 60, bottom));
		jalapenoFire->setAnimation(0, "animation", false);
		jalapenoFire->setScale(scale, 3.0f);
		jalapenoFire->setLocalZOrder(_plantAnimation->getLocalZOrder() + 100); // 植物绘制顺序加100正好等于僵尸绘制顺序 ，爆炸就可以覆盖到僵尸上面
		jalapenoFire->runAction(Sequence::create(DelayTime::create(2.f),
			CallFunc::create([jalapenoFire]()
				{
					jalapenoFire->removeFromParent();
				}), nullptr));
		_node->addChild(jalapenoFire);
	}
}

bool Jalapeno::getZombieIsInExplodeRange(Zombies* zombie)
{
	return getPlantRow() == zombie->getZombieInRow() && zombie->getZombieAnimation()->getPositionX() > 570 ? true : false;
}

void Jalapeno::setPlantNoramlAnimation()
{
	_plantAnimation->setAnimation(0, "Jalapeno_Normal", true);
}

SkeletonAnimation* Jalapeno::showPlantAnimationAndText()
{
	auto& lta = _global->userInformation->getGameText();
	SPSSpriteLayer::plantCardTextScrollView->setInnerContainerSize(Size(lta.find("JALAPENO_1")->second->position));

	_isLoop = true;
	_plantAnimation = plantInit("Jalapeno", "Jalapeno_Normal");
	_plantAnimation->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	_plantAnimation->setScale(1.5f);
	_plantAnimation->setPosition(Vec2(200, 610));

	SPSSpriteLayer::createPlantsText(0, lta.find("JALAPENO_1")->second->text, Vec2(190, 910), lta.find("JALAPENO_1")->second->fontsize);
	SPSSpriteLayer::createPlantsText(2, lta.find("JALAPENO_2")->second->text, Vec2(360, 1000), lta.find("JALAPENO_2")->second->fontsize, Color3B::YELLOW, false);
	SPSSpriteLayer::createPlantsText(3, lta.find("JALAPENO_3")->second->text, Vec2(440, 1000), lta.find("JALAPENO_3")->second->fontsize, Color3B::RED, false);
	SPSSpriteLayer::createPlantsText(1, SPSSpriteLayer::selectRequirementText(lta, PlantsType::Jalapeno, "JALAPENO_4", "JALAPENO_5"), Vec2(360, 870),
		lta.find("JALAPENO_4")->second->fontsize, SPSSpriteLayer::isPlantIsCanSelect[static_cast<unsigned int>(PlantsType::Jalapeno)] ? Color3B::ORANGE : Color3B(255, 70, 0), false);
	
	return _plantAnimation;
}
