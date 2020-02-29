/**
 *Copyright (c) 2020 LZ.All Right Reserved
 *Author : LZ
 *Date: 2020.2.4
 *Email: 2117610943@qq.com
 */

#pragma once
#include "cocos2d.h"
#include "Based/LevelData.h"
#include "Based/GlobalVariable.h"

using namespace cocos2d;

class Plants;
class SunFlower;

class GSAnimationLayer :public Layer
{
public:
	CREATE_FUNC(GSAnimationLayer);
	static GSAnimationLayer* create(Node* node);
	void stopRandomSun();
	void addLayer(Node* node, const int order, const string& name) { node->addChild(this, order, name);}
	void plantPlants();
	void deletePlants();
	void createZombies();

CC_CONSTRUCTOR_ACCESS:
	GSAnimationLayer(Node* node = nullptr);
	~GSAnimationLayer();
	virtual bool init();

private:
	Plants* createDifferentPlants();
	void createSunLayer();
	void createRandomSuns();
	void showCars();
	void gameMainLoop(float delta);
	void zombiesEventUpdate(float delta);
	void plantsEventUpdate();
	void plantsDeleteUpdate(map<int,Plants*>::iterator& plant);
	void bulletEventUpdate();
	void sunsDeleteUpdate();
	void carsEventUpdate();
	
private:
	Global* _global;
	Node* _gameScene;
	Layer* _sunLayer;
	SunFlower* _randomSuns;
	OpenLevelData* _openLevelData;
};
