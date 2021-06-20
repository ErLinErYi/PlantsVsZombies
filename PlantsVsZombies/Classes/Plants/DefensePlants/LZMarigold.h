/**
 *Copyright (c) 2021 LZ.All Right Reserved
 *Author : LZ
 *Date: 2021.6.18
 *Email: 2117610943@qq.com
 */
#pragma once
#include "Plants/LZPlants.h"
#include "Based/LZCoin.h"

class Marigold :public Plants
{
public:
	static Marigold* create(Node* node = nullptr);
	static void stopCreateCoin();
	Sprite* createPlantImage() override;
	void createPlantAnimation() override;

CC_CONSTRUCTOR_ACCESS:
	Marigold(Node* node = nullptr);
	~Marigold();

protected:
	virtual void cavePlantInformation(rapidjson::Value& object, rapidjson::Document::AllocatorType& allocator) override;
	virtual void readPlantInforamtion(rapidjson::Document* levelDataDocument, char* key, int i) override;
	virtual SkeletonAnimation* showPlantAnimationAndText() override;
	virtual void createCoins();
	virtual void createListener() override;
	virtual void playAnimation();
	virtual void calculateCoinShowTime();

protected:
	static int _coinTag;
	Coin* _coin;
	Vec2 _coinShowTime;
	bool _isCreateCoin;
};