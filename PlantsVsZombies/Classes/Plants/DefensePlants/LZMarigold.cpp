/**
 *Copyright (c) 2021 LZ.All Right Reserved
 *Author : LZ
 *Date: 2021.6.18
 *Email: 2117610943@qq.com
 */

#include "LZMarigold.h"

#include "Scenes/GameScenes/Adventure/GameScene/LZAGSData.h"
#include "Scenes/GameScenes/Adventure/GameScene/LZAGSBackgroundLayer.h"
#include "Scenes/GameScenes/Adventure/GameScene/LZAGSInformationLayer.h"
#include "Scenes/GameScenes/Adventure/GameScene/LZAGSControlLayer.h"
#include "Scenes/GameScenes/Adventure/GameScene/LZAGSZombiesAppearControl.h"
#include "Scenes/GameScenes/Adventure/SelectPlantsScene/LZASPSSpriteLayer.h"
#include "Scenes/GameScenes/BigMap/GameScene/LZBigMapGameScene.h"

#include "Based/LZGameType.h"
#include "Based/LZMouseEventControl.h"
#include "Based/LZPlayMusic.h"

Marigold::Marigold(Node* node):
	_coin(nullptr)
,   _isCreateCoin(false)
,   _coinShowTime(Vec2(4 + rand() % 3, 16 + rand() % 5))
{
	_node = node;
	_plantImage = nullptr;
	_plantsType = PlantsType::Marigold;
	_healthPoint = 300;
}

Marigold::~Marigold()
{
}

Marigold* Marigold::create(Node* node)
{
	Marigold* marigold = new (std::nothrow) Marigold(node);
	if (marigold && marigold->init())
	{
		marigold->autorelease();
		return marigold;
	}
	CC_SAFE_DELETE(marigold);
	return nullptr;
}

Sprite* Marigold::createPlantImage()
{
	imageInit("Marigold", INIT);
	_plantImage->setScale(1.1f);
	_plantImage->setAnchorPoint(Vec2(0.37f, 0.59f));
	return _plantImage;
}

void Marigold::createPlantAnimation()
{
	_plantAnimation = plantInit("Marigold", "animation");
	_plantAnimation->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	_plantAnimation->setScale(0.6f);
	_node->addChild(_plantAnimation);

	// 影子
	setPlantShadow(2.8f);

	// 泥土飞溅动画
	setPlantSoilSplashAnimation(1.0f);

	// 创建监听
	createListener();
}

void Marigold::createListener()
{
	if (controlLayerInformation && controlLayerInformation->_zombiesAppearControl && 
		controlLayerInformation->_zombiesAppearControl->getZombiesAppearFrequency() !=
		OpenLevelData::getInstance()->readLevelData(OpenLevelData::getInstance()->getLevelNumber())->getZombiesFrequency())
	{
		calculateCoinShowTime();

		playAnimation();
	}
}

void Marigold::playAnimation()
{
	auto delaytime = DelayTime::create(_coinShowTime.x);
	auto delaytime1 = DelayTime::create(1.f);
	auto delaytime2 = DelayTime::create(_coinShowTime.y);
	auto callfunc = CallFunc::create([&]() { createCoins(); });
	auto callfunc1 = CallFunc::create([=]() { _isCreateCoin = false; });

	_plantAnimation->runAction(RepeatForever::create(Sequence::create(delaytime, callfunc, delaytime1, delaytime2, callfunc1, nullptr)));
}

void Marigold::calculateCoinShowTime()
{
	_plantAnimation->runAction(RepeatForever::create(Sequence::create(DelayTime::create(1.f),
		CallFunc::create([this]()
			{
				if (_coinShowTime.x > 0)_coinShowTime.x -= 1.f;
				else if (_coinShowTime.y > 0)_coinShowTime.y -= 1.f;
				else _coinShowTime = Vec2(4 + rand() % 3, 16 + rand() % 5);
			}), nullptr)));
}

void Marigold::createCoins()
{
	PlayMusic::playMusic(rand() % 2 == 0 ? "throw" : "throw2");
	
	_coin = new Coin(goodsLayerInformation);
	_coin->setPosition(_plantAnimation->getPosition());
	_coin->createCoin();
	CoinsGroup.push_back(_coin);

	_isCreateCoin = true;
}

void Marigold::stopCreateCoin()
{
	for (auto& plant : PlantsGroup)
	{
		if (plant.second->getPlantType() == PlantsType::Marigold)
		{
			plant.second->getPlantAnimation()->stopAllActions();
		}
	}
}

SkeletonAnimation* Marigold::showPlantAnimationAndText()
{
	auto& lta = _global->userInformation->getGameText();
	SPSSpriteLayer::plantCardTextScrollView->setInnerContainerSize(Size(lta.find("MARIGOLD_1")->second->position));

	_isLoop = true;
	_plantAnimation = plantInit("Marigold", "animation");
	_plantAnimation->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	_plantAnimation->setScale(1.f);
	_plantAnimation->setPosition(Vec2(200, 610));

	SPSSpriteLayer::createPlantsText(0, lta.find("MARIGOLD_1")->second->text, Vec2(190, 910), lta.find("MARIGOLD_1")->second->fontsize);
	SPSSpriteLayer::createPlantsText(1, lta.find("MARIGOLD_2")->second->text, Vec2(360, 1000), lta.find("MARIGOLD_2")->second->fontsize, Color3B::YELLOW, false);
	SPSSpriteLayer::createPlantsText(2, lta.find("MARIGOLD_3")->second->text, Vec2(440, 1000), lta.find("MARIGOLD_3")->second->fontsize, Color3B::RED, false);
	SPSSpriteLayer::createPlantsText(3, SPSSpriteLayer::selectRequirementText(lta, PlantsType::Marigold, "MARIGOLD_4", "MARIGOLD_5"), Vec2(360, 870),
		lta.find("MARIGOLD_5")->second->fontsize, SPSSpriteLayer::isPlantIsCanSelect[static_cast<unsigned int>(PlantsType::Marigold)] ? Color3B::ORANGE : Color3B(255, 70, 0), false);

	return _plantAnimation;
}

void Marigold::cavePlantInformation(rapidjson::Value& object, rapidjson::Document::AllocatorType& allocator)
{
	object.AddMember("CoinShowTime", _isCreateCoin ? _coinShowTime.x + _coinShowTime.y : _coinShowTime.x, allocator);
}

void Marigold::readPlantInforamtion(rapidjson::Document* levelDataDocument, char* key, int i)
{
	_coinShowTime.x = (*levelDataDocument)[key]["Plants"][to_string(i).c_str()]["CoinShowTime"].GetFloat();
}