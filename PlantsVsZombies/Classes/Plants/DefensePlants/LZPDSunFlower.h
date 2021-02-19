/**
 *Copyright (c) 2020 LZ.All Right Reserved
 *Author : LZ
 *Date: 2020.2.6
 *Email: 2117610943@qq.com
 */

#pragma once
#include "Plants/LZPPlants.h"

class Sun;

class SunFlower :public Plants
{
public:
	static SunFlower* create(Node* node = nullptr);
	static void stopSun();
	static void sunRecovery(Sun* sun);
	void createRandomSuns();
	Sprite* createPlantImage() override;
	void createPlantAnimation() override;
	
CC_CONSTRUCTOR_ACCESS:
	SunFlower(Node* node = nullptr);
	~SunFlower();

protected:
	virtual void cavePlantInformation(rapidjson::Value& object, rapidjson::Document::AllocatorType& allocator) override;
	virtual void readPlantInforamtion(rapidjson::Document* levelDataDocument, char* key, int i) override;
	virtual SkeletonAnimation* showPlantAnimationAndText() override;
	virtual void createSuns();
	virtual void createListener();
	virtual void playAnimation();
	virtual void calculateSunShowTime();
	
protected:
	static int _sunTag;
	Sun* _sun;
	Vec2 _sunShowTime;
	bool _isCreateSun;
};

class Sun
{
public:
	void createSuns(float scale = 1.2f);
	void setSunTag(const int& sunTag);
	void setPosition(const Vec2& position);
	void setEnable(bool isUsed);
	int getSunTag() const;
	bool getEnable() const;
	void releaseSun();
	float calculateMoveTime();
	SkeletonAnimation* getSun();
	static void deleteSun(list<Sun*>::iterator& sun);

CC_CONSTRUCTOR_ACCESS:
	Sun(Node* node = nullptr);
	~Sun() {}

private:
	int _sunTag;
	bool _isEnable;
	bool _canDelete;
	Vec2 _position;
	SkeletonAnimation* _sun;
	Node* _node;
	Global* _global;
};