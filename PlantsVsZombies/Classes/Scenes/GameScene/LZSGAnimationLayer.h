/**
 *Copyright (c) 2020 LZ.All Right Reserved
 *Author : LZ
 *Date: 2020.2.4
 *Email: 2117610943@qq.com
 */

#pragma once
#include "cocos2d.h"
#include "Based/LZBLevelData.h"
#include "Based/LZBGlobalVariable.h"

using namespace cocos2d;

class Plants;
class SunFlower;
class Zombies;
class Bullet;

enum class PlantsType;
enum class ZombiesType;
enum class BulletType;

class GSAnimationLayer :public Layer
{
public:
	CREATE_FUNC(GSAnimationLayer);
	static GSAnimationLayer* create(Node* node);
	Plants* createDifferentPlants(PlantsType plantsType);
	Zombies* createDifferentZombies(ZombiesType zombiesType, Node* node = nullptr);
	Bullet* createDifferentBullet(BulletType bulletType);
	void addLayer(Node* node, const int order, const string& name) { node->addChild(this, order, name);}
	void plantPlants();
	void deletePlants();
	void createZombies();
	void createZombiesOnSurvival();
	GSAnimationLayer* getAnimationLayer();

CC_CONSTRUCTOR_ACCESS:
	GSAnimationLayer(Node* node = nullptr);
	~GSAnimationLayer();
	virtual bool init();

private:
	void createRandomSuns();
	void showCars();
	void gameMainLoop(float delta);
	void zombiesEventUpdate(float delta);
	void plantsEventUpdate();
	void plantsDeleteUpdate(map<int,Plants*>::iterator& plant);
	void bulletEventUpdate();
	void sunsDeleteUpdate();
	void coinDeleteUpdate();
	void carsEventUpdate();
	
private:
	Global* _global;
	Node* _gameScene;
	SunFlower* _randomSuns;
	OpenLevelData* _openLevelData;
	default_random_engine _random; 
	random_device _device;
};
