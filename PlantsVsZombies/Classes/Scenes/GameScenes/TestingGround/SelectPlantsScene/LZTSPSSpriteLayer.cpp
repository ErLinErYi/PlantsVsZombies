/**
 *Copyright (c) 2019 LZ.All Right Reserved
 *Author : LZ
 *Date: 2021.4.08
 *Email: 2117610943@qq.com
 */

#include "LZTSPSSpriteLayer.h"

TSPSSpriteLayer::TSPSSpriteLayer()
{
}

TSPSSpriteLayer::~TSPSSpriteLayer()
{
}

bool TSPSSpriteLayer::init()
{
	if (!Layer::init())return false;

	createSelectPlantsDialog();
	
	return true;
}

void TSPSSpriteLayer::controlPlantCanSelect(Button* button, int priority)
{
	/* 该模式下除金盏花,向日葵外所有植物都可使用 */
	if (plantsCardInformation[priority].type == PlantsType::Marigold ||
		plantsCardInformation[priority].type == PlantsType::SunFlower ||
		plantsCardInformation[priority].type == PlantsType::SunFlowerTwin)
	{
		button->setColor(Color3B(70, 70, 70));
		button->setCascadeColorEnabled(true);  /* 设置父节点影响子节点 */
		isPlantIsCanSelect[static_cast<unsigned int>(plantsCardInformation[priority].type)] = false;
	}
}

Text* TSPSSpriteLayer::showPlantsInformation(Button* button, bool showHeart)
{
	/* 图片 */
	auto plantImage = Sprite::createWithSpriteFrameName(plantsCardInformation[button->getTag()].information[0] + ".png");
	plantImage->setScale(0.8f);
	plantImage->setFlippedX(plantsCardInformation[button->getTag()].flippedX);
	plantImage->setPosition(Vec2(85, 50));
	button->addChild(plantImage);

	string str[] = { "PlantsLevel_Gold.png","PlantsLevel_Silver.png","PlantsLevel_Copper.png" };
	auto plantsLevelIamge = Sprite::createWithSpriteFrameName(str[plantsCardInformation[button->getTag()].quality - 1]);
	plantsLevelIamge->setPosition(Vec2(155, 74));
	plantsLevelIamge->setScale(0.65f);
	button->addChild(plantsLevelIamge);

	auto background = Sprite::createWithSpriteFrameName("PlantsIconBackground.png");
	background->setOpacity(0);
	button->addChild(background);

	auto plantsIconBackground = Sprite::createWithSpriteFrameName("PlantsIconBackground.png");
	plantsIconBackground->setPosition(Vec2(95, 150));
	plantsIconBackground->setScale(0.28f);
	plantsIconBackground->setColor(plantsCardInformation[button->getTag()].color);
	background->addChild(plantsIconBackground);

	auto plantsIcon = Sprite::createWithSpriteFrameName(plantsCardInformation[button->getTag()].information[1] + ".png");
	plantsIcon->setPosition(Vec2(75, 75));
	plantsIcon->setScale(1.2f);
	plantsIconBackground->addChild(plantsIcon);

	auto prohibit = Sprite::createWithSpriteFrameName("Prohibit.png");
	prohibit->setPosition(Vec2(160, 20));
	prohibit->setScale(0.5f);
	prohibit->setVisible(!isPlantIsCanSelect[static_cast<unsigned int>(plantsCardInformation[button->getTag()].type)]);
	button->addChild(prohibit, 1);

	/* 文本 */
	auto plantsNeedSuns = ui::Text::create();
	plantsNeedSuns->setFontName(GAME_FONT_NAME_3);
	plantsNeedSuns->setColor(Color3B::BLACK);
	plantsNeedSuns->setAnchorPoint(Vec2(1, 0.5f));
	plantsNeedSuns->setPosition(Vec2(175, 18));
	plantsNeedSuns->setFontSize(35);
	plantsNeedSuns->setString("0");
	button->addChild(plantsNeedSuns);

	auto plantsLevelText = ui::Text::create();
	plantsLevelText->setFontName(GAME_FONT_NAME_1);
	plantsLevelText->setColor(Color3B::BLACK);
	plantsLevelText->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	plantsLevelText->setPosition(Vec2(153, 75));
	plantsLevelText->setFontSize(25);
	plantsLevelText->setString(plantsCardInformation[button->getTag()].information[2]);
	button->addChild(plantsLevelText);

	return plantsNeedSuns;
}