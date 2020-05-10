/**
 *Copyright (c) 2020 LZ.All Right Reserved
 *Author : LZ
 *Date: 2020.2.6
 *Email: 2117610943@qq.com
 */

#include "SunFlower.h"
#include "../EmissionPlants/Bullet/Bullet.h"

#include "Based/GameType.h"
#include "Scenes/GameScene/GSData.h"
#include "Scenes/GameScene/GSBackgroundLayer.h"
#include "Scenes/GameScene/GSInformationLayer.h"

#include "Based/Coin.h"

int SunFlower::_sunTag = -1;

SunFlower::SunFlower(Node* node, Node* sunLayer) :
  _sun(nullptr)
, _sunLayer(sunLayer)
, _sunShowTime(Vec2(4 + rand() % 3, 16 + rand() % 5))
, _sunShowTime1(_sunShowTime)
{
	_node = node;
	_coolDownTime = 7.5f;
	_plantImage = nullptr;
	_plantsType = PlantsType::SunFlower;
	_healthPoint = 300;
}

SunFlower::~SunFlower()
{
}

SunFlower* SunFlower::create(Node* node)
{
	SunFlower* sunFlower = new (std::nothrow) SunFlower(node);
	if (sunFlower && sunFlower->init())
	{
		sunFlower->autorelease();
		return sunFlower;
	}
	CC_SAFE_DELETE(sunFlower);
	return nullptr;
}

void SunFlower::stopSun()
{
	for (auto sun : SunsGroup)
	{
		sunRecovery(sun);
	}
}

void SunFlower::createRandomSuns()
{
	/* 获取太阳层 */
	_sunLayer->runAction(RepeatForever::create(Sequence::create(DelayTime::create(5),
		CallFunc::create([this]()
			{
				auto sun = new Sun(_sunLayer);
				sun->setSunTag(++_sunTag);
				sun->setPosition(Vec2(rand() % 1100 + 500, 1150));
				sun->createSuns();

				auto temporary = sun->getSunTag();
				sun->getSun()->setScale(1.2f);
				sun->getSun()->runAction(Sequence::create(
					MoveBy::create(5.0f, Vec2(0, -rand() % 500 - 500)), 
					DelayTime::create(14), FadeOut::create(0.5f), 
					CallFunc::create([sun]() 
						{
							sun->getSun()->setVisible(false); 
						}), nullptr));
				
				SunsGroup.push_back(sun);
			}), DelayTime::create(20), nullptr)));
	goodsRecovery();
}

Sprite* SunFlower::createPlantImage()
{
	imageInit("SunFlower_2", INIT);
	_plantImage->setScale(1.5f);
	return _plantImage;
}

void SunFlower::createPlantAnimation()
{
	_plantAnimation = plantInit("SunFlower_2", "SunFlower_Normal");
	_plantAnimation->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	_plantAnimation->setScale(0.8f);
	_plantAnimation->setSkin("SunFlower_Normal");
	_node->addChild(_plantAnimation);

	// 影子
	setPlantShadow(2.3f);

	// 泥土飞溅动画
	setPlantSoilSplashAnimation(1.0f);
	
	createListener();
}

void SunFlower::createListener()
{
	if (_global->userInformation->getIsReadFileData())
	{
		if (_sunShowTime1.x <= 0)
		{
			_plantAnimation->runAction(Sequence::create(DelayTime::create(_sunShowTime1.y),
				CallFunc::create([&]()
					{
						playAnimation();
					}), nullptr));
		}
		else
		{
			_plantAnimation->runAction(Sequence::create(DelayTime::create(_sunShowTime1.x),
				CallFunc::create([&]() { createSuns(); }), DelayTime::create(0.5f),
				CallFunc::create([=]() { _plantAnimation->setSkin("SunFlower_Normal"); }), DelayTime::create(_sunShowTime1.y),
				CallFunc::create([&]()
					{
						playAnimation();
					}), nullptr));
		}
	}
	else
		playAnimation();

	calculateSunShowTime();
	goodsRecovery();
}

void SunFlower::playAnimation()
{
	auto delaytime = DelayTime::create(_sunShowTime.x);
	auto delaytime1 = DelayTime::create(0.5f);
	auto delaytime2 = DelayTime::create(_sunShowTime.y);
	auto callfunc = CallFunc::create([&]() { createSuns(); });
	auto callfunc1 = CallFunc::create([=]() { _plantAnimation->setSkin("SunFlower_Normal"); /* 设置皮肤 */});

	_plantAnimation->runAction(RepeatForever::create(Sequence::create(delaytime, callfunc, delaytime1, callfunc1, delaytime2, nullptr)));
}

void SunFlower::calculateSunShowTime()
{
	_plantAnimation->runAction(RepeatForever::create(Sequence::create(DelayTime::create(0.05f),
		CallFunc::create([this]()
			{
				if (_sunShowTime1.x > 0)_sunShowTime1.x -= 0.05f;
				else if (_sunShowTime1.y > 0)_sunShowTime1.y -= 0.05f;
				else _sunShowTime1 = _sunShowTime;
			}), nullptr)));
}

void SunFlower::setSunShowTime(const Vec2 time)
{
	_sunShowTime1 = time;
}

Vec2 SunFlower::getSunShowTime() const
{
	return _sunShowTime1;
}

void SunFlower::sunRecovery(Sun* sun)
{
	auto temporary = sun;
	sun->setEnable(false);
	sun->getSun()->stopAllActions();
	sun->getSun()->runAction(Sequence::create(EaseExponentialOut::create(MoveTo::create(0.5f, Vec2(290, 1010))),
		Spawn::create(ScaleTo::create(0.2f, 0.3f), FadeOut::create(0.2f), nullptr),
		CallFunc::create([=]()
			{
				Global::getInstance()->userInformation->setSunNumbers(Global::getInstance()->userInformation->getSunNumbers() + 50);
				informationLayerInformation->updateSunNumbers();
				backgroundLayerInformation->gameType->updateRequirementNumbers("阳光数量增加");
			}), DelayTime::create(5.0f),
		CallFunc::create([temporary]()
			{
				temporary->getSun()->setVisible(false);
			}), nullptr));
}

void SunFlower::goodsRecovery()
{
	auto linster = EventListenerTouchOneByOne::create();
	linster->onTouchBegan = [=](Touch* t, Event* e)
	{
		Point p = t->getLocation();
		if (Global::getInstance()->userInformation->getIsMirrorScene())
		{	
			p.x = 1920 - p.x;
		}
		for (auto sun : SunsGroup)
		{
			if ( sun->getSun()->getBoundingBox().containsPoint(p) && sun->getEnable())
			{
				Bullet::playSoundEffect("points");
				sunRecovery(sun);
			}
		}

		for (auto coin : CoinsGroup)
		{
			if (coin->getCoin()->getBoundingBox().containsPoint(p) && coin->getEnable())
			{
				Bullet::playSoundEffect("coin");
				Coin::coinRecoveryAction(coin);
			}
		}
		return true;
	};
	Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(linster, _sunLayer);
}

void SunFlower::createSuns()
{
	Bullet::playSoundEffect(rand() % 2 == 0 ? "throw" : "throw2");
	_plantAnimation->setSkin("SunFlower_ProduceSun"); /* 设置皮肤 */

	_sun = new Sun(_sunLayer);
	_sun->setSunTag(++_sunTag);
	_sun->setPosition(_plantAnimation->getPosition());
	_sun->createSuns();

	SunsGroup.push_back(_sun);
}

Sun::Sun(Node* node) :
    _node(node)
,	_sunTag(0)
,   _sun(nullptr)
,   _isEnable(true)
,   _global(Global::getInstance())
{
}

void Sun::createSuns()
{
	_sun = SkeletonAnimation::createWithData(_global->userInformation->getAnimationData().find("Sun")->second);
	_sun->addAnimation(0, "Sun_rotate", true);
	_sun->setPosition(_position);
	_sun->runAction(Sequence::create(
		Spawn::create(ScaleTo::create(0.5f, 1.2f), JumpBy::create(0.6f, Vec2(rand() % 120 - 60, 0), rand() % 100 + 100, 1), nullptr), 
		DelayTime::create(14), FadeOut::create(0.5f), DelayTime::create(5.0f), CallFunc::create([this]() {_sun->setVisible(false); }), nullptr));
	_node->addChild(_sun);
}

void Sun::setSunTag(const int& sunTag)
{
	_sunTag = sunTag;
}

void Sun::setPosition(const Vec2& position)
{
	_position = position;
}

void Sun::setEnable(bool isUsed)
{
	_isEnable = isUsed;
}

int Sun::getSunTag() const
{
	return _sunTag;
}

bool Sun::getEnable() const
{
	return _isEnable;
}

void Sun::releaseSun() const
{
	_sun->removeFromParent();
}

SkeletonAnimation* Sun::getSun()
{
	return _sun;
}

void Sun::deleteSun(list<Sun*>::iterator& sun)
{
	if (!(*sun)->getSun()->isVisible())
	{
		(*sun)->releaseSun();
		delete* sun;
		*sun = nullptr;
		SunsGroup.erase(sun++);
	}
	else
	{
		++sun;
	}
}
