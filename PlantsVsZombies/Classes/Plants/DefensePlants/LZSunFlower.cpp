﻿/**
 *Copyright (c) 2020 LZ.All Right Reserved
 *Author : LZ
 *Date: 2020.2.6
 *Email: 2117610943@qq.com
 */

#include "LZSunFlower.h"

#include "Scenes/GameScenes/Adventure/GameScene/LZAGSData.h"
#include "Scenes/GameScenes/Adventure/GameScene/LZAGSBackgroundLayer.h"
#include "Scenes/GameScenes/Adventure/GameScene/LZAGSInformationLayer.h"
#include "Scenes/GameScenes/Adventure/SelectPlantsScene/LZASPSSpriteLayer.h"
#include "Scenes/GameScenes/BigMap/GameScene/LZBigMapGameScene.h"

#include "Based/LZGameType.h"
#include "Based/LZMouseEventControl.h"
#include "Based/LZPlayMusic.h"

int SunFlower::_sunTag = -1;

SunFlower::SunFlower(Node* node) :
  _sun(nullptr)
, _isCreateSun(false)
, _isIZombiesType(false)
, _iZombiesCreateSun(0)
, _sunShowTime(Vec2(4 + rand() % 3, 16 + rand() % 5))
{
	_node = node;
	_plantImage = nullptr;
	_plantsType = PlantsType::SunFlower;
	_healthPoint = 300;
	_totalHealthPoint = 300;
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

Sprite* SunFlower::createPlantImage()
{
	imageInit("SunFlower", INIT);
	_plantImage->setScale(1.5f);
	_plantImage->setAnchorPoint(Vec2(0.37f, 0.52f));
	return _plantImage;
}

void SunFlower::createPlantAnimation()
{
	_plantAnimation = plantInit("SunFlower", "SunFlower_Normal");
	_plantAnimation->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	_plantAnimation->setScale(0.8f);
	_plantAnimation->setSkin("SunFlower_Normal");
	_node->addChild(_plantAnimation);

	// 影子
	setPlantShadow(2.3f);

	// 泥土飞溅动画
	setPlantSoilSplashAnimation(1.0f);
	
	// 创建监听
	createListener();
}

void SunFlower::createListener()
{
	if (!_isIZombiesType)
	{
		calculateSunShowTime();

		playAnimation();
	}
}

void SunFlower::playAnimation()
{
	auto delaytime = DelayTime::create(_sunShowTime.x);
	auto delaytime1 = DelayTime::create(1.f);
	auto delaytime2 = DelayTime::create(_sunShowTime.y);
	auto callfunc = CallFunc::create([&]() { createSuns(); });
	auto callfunc1 = CallFunc::create([=]() { _plantAnimation->setSkin("SunFlower_Normal"); /* 设置皮肤 */});
	auto callfunc2 = CallFunc::create([=]() { _isCreateSun = false; });

	_plantAnimation->runAction(RepeatForever::create(Sequence::create(delaytime, callfunc, delaytime1, callfunc1, delaytime2, callfunc2, nullptr)));
}

void SunFlower::calculateSunShowTime()
{
	_plantAnimation->runAction(RepeatForever::create(Sequence::create(DelayTime::create(1.f),
		CallFunc::create([this]()
			{
				if (_sunShowTime.x > 0)_sunShowTime.x -= 1.f;
				else if (_sunShowTime.y > 0)_sunShowTime.y -= 1.f;
				else _sunShowTime = Vec2(4 + rand() % 3, 16 + rand() % 5);
			}), nullptr)));
}

void SunFlower::sunRecovery(Sun* sun)
{
	auto pos = Vec2(290, 1025);
	auto temporary = sun->getSun();
	
	auto actionMoveTo = EaseSineOut::create(MoveTo::create(sun->calculateMoveTime(), pos));
	auto actionScaleFade = Spawn::create(ScaleTo::create(0.2f, 0.3f), FadeOut::create(0.2f), nullptr);
	auto actionCallFunc = CallFunc::create([=]()
		{
			Global::getInstance()->userInformation->setSunNumbers(Global::getInstance()->userInformation->getSunNumbers() + 50);
			informationLayerInformation->updateSunNumbers();
			if (informationLayerInformation->gameType)
				informationLayerInformation->gameType->updateRequirementNumbers("阳光数量增加");
		});
	auto actionCallFunc2 = CallFunc::create([temporary]()
		{
			temporary->setVisible(false);
		});

	/*if (BigMapGameScene::scrollView)
	{
		auto offset = BigMapGameScene::scrollView->getContentOffset();
		auto point = temporary->getPosition();
		
		temporary->retain();
		temporary->removeFromParent();
		informationLayerInformation->addChild(temporary);
		temporary->setPosition(Vec2(point.x - fabs(offset.x), point.y - fabs(offset.y)));
		temporary->setScale(0.5f);
	}*/

	sun->setEnable(false);
	temporary->stopAllActions();
	temporary->runAction(Sequence::create(actionMoveTo, actionScaleFade, actionCallFunc, DelayTime::create(5.0f), actionCallFunc2, nullptr));
}

void SunFlower::createSuns()
{
	PlayMusic::playMusic(rand() % 2 == 0 ? "throw" : "throw2");
	_plantAnimation->setSkin("SunFlower_ProduceSun"); /* 设置皮肤 */

	_sun = new Sun(goodsLayerInformation);
	_sun->setSunTag(++_sunTag);
	_sun->setPosition(_plantAnimation->getPosition());
	_sun->createSuns();
	SunsGroup.emplace_back(_sun);

	_isCreateSun = true;
}

void SunFlower::createRandomSuns()
{
	auto callFunc = CallFunc::create([=]()
		{
			auto pos = Vec2(rand() % 1100 + 500, 1200);
			auto pos1 = Vec2(0, -(rand() % 500) - 500);

			auto sun = new Sun(goodsLayerInformation);
			sun->setSunTag(++_sunTag);
			sun->setPosition(pos);
			sun->createSuns();

			const auto temporary = sun->getSunTag();
			sun->getSun()->setScale(1.2f);
			sun->getSun()->runAction(Sequence::create(
				MoveBy::create(5.0f, pos1),
				DelayTime::create(14), FadeOut::create(0.5f),
				CallFunc::create([sun]()
					{
						sun->getSun()->setVisible(false);
					}), nullptr));

			SunsGroup.emplace_back(sun);
		});

	_node->runAction(RepeatForever::create(Sequence::create(DelayTime::create(5), callFunc, DelayTime::create(20), nullptr)));
}

void SunFlower::setIZombiesType(const bool type)
{
	_isIZombiesType = type;
}

void SunFlower::checkPlantHealthPoint()
{
	if (_isIZombiesType)
	{
		for (int i = 0; i < 6; ++i)
		{
			if (_healthPoint <= _totalHealthPoint - 50 * (i + 1) && _iZombiesCreateSun == i)
			{
				createSuns();
				_sun->getSun()->setColor(Color3B::MAGENTA);
				_iZombiesCreateSun = i + 1;
			}
		}
	}
}

void SunFlower::stopSun()
{
	for (auto sun : SunsGroup)
	{
		sunRecovery(sun);
	}
}

SkeletonAnimation* SunFlower::showPlantAnimationAndText()
{
	auto& lta = _global->userInformation->getGameText();
	SPSSpriteLayer::plantCardTextScrollView->setInnerContainerSize(Size(lta.find("SUNFLOWER_1")->second->position));

	_isLoop = true;
	_plantAnimation = plantInit("SunFlower", "SunFlower_Normal");
	_plantAnimation->setSkin("SunFlower_Normal");
	_plantAnimation->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	_plantAnimation->setScale(1.5f);
	_plantAnimation->setPosition(Vec2(200, 610));

	SPSSpriteLayer::createPlantsText(0, lta.find("SUNFLOWER_1")->second->text, Vec2(190, 910), lta.find("SUNFLOWER_1")->second->fontsize);
	SPSSpriteLayer::createPlantsText(1, lta.find("SUNFLOWER_2")->second->text, Vec2(360, 1000), lta.find("SUNFLOWER_2")->second->fontsize, Color3B::YELLOW, false);
	SPSSpriteLayer::createPlantsText(2, lta.find("SUNFLOWER_3")->second->text, Vec2(440, 1000), lta.find("SUNFLOWER_3")->second->fontsize, Color3B::RED, false);
	SPSSpriteLayer::createPlantsText(3, lta.find("SUNFLOWER_4")->second->text, Vec2(360, 830), lta.find("SUNFLOWER_4")->second->fontsize, Color3B::ORANGE, false);

	return _plantAnimation;
}

void SunFlower::cavePlantInformation(rapidjson::Value& object, rapidjson::Document::AllocatorType& allocator)
{
	object.AddMember("SunShowTime", _isCreateSun ? _sunShowTime.x + _sunShowTime.y : _sunShowTime.x, allocator);
}

void SunFlower::readPlantInforamtion(rapidjson::Document* levelDataDocument, char* key, int i)
{
	_sunShowTime.x = (*levelDataDocument)[key]["Plants"][to_string(i).c_str()]["SunShowTime"].GetFloat();
}

Sun::Sun(Node* node) :
    _node(node)
,	_sunTag(0)
,   _sun(nullptr)
,   _isEnable(true)
,   _global(Global::getInstance())
{
}

void Sun::createSuns(float scale)
{
	_sun = SkeletonAnimation::createWithData(_global->userInformation->getAnimationData().find("Sun")->second);
	_sun->addAnimation(0, "Sun_rotate", true);
	_sun->setPosition(_position);
	_sun->update(0);
	_sun->runAction(Sequence::create(
		Spawn::create(ScaleTo::create(0.5f, scale), JumpBy::create(0.6f, Vec2(rand() % 180 - 90, 0), rand() % 100 + 100, 1), nullptr),
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

void Sun::releaseFunction()
{
	_sun->removeFromParent();
	_sun = nullptr;
}

float Sun::calculateMoveTime()
{
	const auto distance = sqrt(pow(_sun->getPosition().x - 290, 2) + pow(_sun->getPosition().y - 1010, 2));
	return static_cast<float>(distance / (750 + distance));
}

SkeletonAnimation* Sun::getSun()
{
	return _sun;
}

void Sun::deleteSun(list<Sun*>::iterator& sun)
{
	if (!(*sun)->getSun()->isVisible())
	{
		(*sun)->releaseFunction();
		delete* sun;
		*sun = nullptr;
		SunsGroup.erase(sun++);
	}
	else
	{
		++sun;
	}
}