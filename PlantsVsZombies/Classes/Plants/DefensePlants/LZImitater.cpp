/**
 *Copyright (c) 2020 LZ.All Right Reserved
 *Author : LZ
 *Date: 2020.2.6
 *Email: 2117610943@qq.com
 */

#include "LZImitater.h"
#include "Scenes/GameScenes/Adventure/SelectPlantsScene/LZASPSSpriteLayer.h"
#include "Scenes/GameScenes/Adventure/GameScene/LZAGSData.h"
#include "Scenes/GameScenes/Adventure/GameScene/LZAGSAnimationLayer.h"
#include "Scenes/GameScenes/BigMap/GameScene/LZBigMapGameScene.h"

Imitater::Imitater(Node* node)
{
	_node = node;
	_plantImage = nullptr;

	_healthPoint = 30000;
	_totalHealthPoint = 30000;
	_isLoop = false;
	_plantsType = PlantsType::Imitater;
}

Imitater::~Imitater()
{
}

Imitater* Imitater::create(Node* node)
{
	Imitater* imitater = new (std::nothrow) Imitater(node);
	if (imitater && imitater->init())
	{
		imitater->autorelease();
		return imitater;
	}
	CC_SAFE_DELETE(imitater);
	return nullptr;
}
Sprite* Imitater::createPlantImage()
{
	imageInit("Imitater", INIT);
	_plantImage->setScale(1.2f);
	_plantImage->setAnchorPoint(Vec2(0.4f, 0.55f));
	return _plantImage;
}

void Imitater::setPlantNoramlAnimation()
{
	_plantAnimation->setAnimation(0, "Imitater_Normal", true);
}

void Imitater::createPlantAnimation()
{
	_plantAnimation = plantInit("Imitater", "Imitater_Transformation");
	_plantAnimation->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	_node->addChild(_plantAnimation);

	// 影子
	setPlantShadow(2.f);

	// 泥土飞溅动画
	setPlantSoilSplashAnimation(1.0f);

	createListener();
}

void Imitater::createListener()
{
	_plantAnimation->setEventListener([&](spTrackEntry* entry, spEvent* event)
		{
			if (strcmp(event->data->name, "finished") == 0)
			{
				createOtherPlant();
				_plantAnimation->setVisible(false);
			}
		});
}

void Imitater::createOtherPlant()
{
	int t = 0;
	for (int i = _global->userInformation->getUserSelectCrads().size() - 1; i >= 0; --i)
	{
		if (_global->userInformation->getUserSelectCrads()[i].cardTag == static_cast<int>(PlantsType::Imitater))
		{
			t = i - 1;
			break;
		}
	}

	auto plants = animationLayerInformation->createDifferentPlants(static_cast<PlantsType>(_global->userInformation->getUserSelectCrads()[t].cardTag), _node);
	plants->setPlantPosition(_position);
	plants->setPlantLocalZOrder(_plantAnimation->getLocalZOrder());
	plants->setPlantRowAndColumn(getPlantRowAndColumn());
	plants->setPlantTag(_plantAnimation->getTag());
	plants->createPlantAnimation();
	if (BigMapGameScene::bigMapWorld)plants->setPlantScale(0.7f);
	_plantAnimation->setTag(-1);
	
	PlantsGroup.insert(pair<int, Plants*>(plants->getPlantAnimation()->getTag(), plants));
}

SkeletonAnimation* Imitater::showPlantAnimationAndText()
{
	auto& lta = _global->userInformation->getGameText();
	SPSSpriteLayer::plantCardTextScrollView->setInnerContainerSize(Size(lta.find("IMITATER_1")->second->position));

	_isLoop = true;
	_plantAnimation = plantInit("Imitater", "Imitater_Normal");
	_plantAnimation->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	_plantAnimation->setScale(1.5f);
	_plantAnimation->setPosition(Vec2(200, 610));

	SPSSpriteLayer::createPlantsText(0, lta.find("IMITATER_1")->second->text, Vec2(190, 910), lta.find("IMITATER_1")->second->fontsize);
	SPSSpriteLayer::createPlantsText(2, lta.find("IMITATER_2")->second->text, Vec2(360, 1000), lta.find("IMITATER_2")->second->fontsize, Color3B::YELLOW, false);
	SPSSpriteLayer::createPlantsText(3, lta.find("IMITATER_3")->second->text, Vec2(360, 910), lta.find("IMITATER_3")->second->fontsize, Color3B::RED, false);
	SPSSpriteLayer::createPlantsText(1, SPSSpriteLayer::selectRequirementText(lta, PlantsType::Imitater, "IMITATER_4", "IMITATER_5"), Vec2(360, 890),
		lta.find("IMITATER_4")->second->fontsize, SPSSpriteLayer::isPlantIsCanSelect[static_cast<unsigned int>(PlantsType::Imitater)] ? Color3B::ORANGE : Color3B(255, 70, 0), false);

	return _plantAnimation;
}
