/**
 *Copyright (c) 2020 LZ.All Right Reserved
 *Author : LZ
 *Date: 2020.2.6
 *Email: 2117610943@qq.com
 */

#include "Jalapeno.h"
#include "../EmissionPlants/Bullet/Bullet.h"

#include "Zombies/Zombies.h"
#include "Scenes/GameScene/GSData.h"
#include "Scenes/GameScene/GSBackgroundLayer.h"

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
	setEventListener();
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
	Bullet::playSoundEffect("cherrybomb");
	GSBackgroundLayer::backgroundRunAction();

	for (int i = 0; i < 9; i++)
	{
		auto jalapenoFire = SkeletonAnimation::createWithData(_global->userInformation->getAnimationData().find("Jalapeno_Fire")->second);
		jalapenoFire->setPosition(Vec2(570 + 122 * i + 60, _plantAnimation->getPositionY() - 10));
		jalapenoFire->setAnimation(0, "animation", false);
		jalapenoFire->setScaleY(1.3f);
		jalapenoFire->setLocalZOrder(_plantAnimation->getLocalZOrder() + 10); // 植物绘制顺序加10正好等于僵尸绘制顺序 ，爆炸就可以覆盖到僵尸上面
		jalapenoFire->runAction(Sequence::create(DelayTime::create(2),
			CallFunc::create([jalapenoFire]()
				{
					jalapenoFire->removeFromParent();
				}), nullptr));
		_node->addChild(jalapenoFire);
	}
}

bool Jalapeno::getZombieIsInExplodeRange(Zombies* zombie) const
{
	return fabs(zombie->getZombieAnimation()->getPositionY() - 10 - _plantAnimation->getPositionY()) <= 10 ? true : false;
}