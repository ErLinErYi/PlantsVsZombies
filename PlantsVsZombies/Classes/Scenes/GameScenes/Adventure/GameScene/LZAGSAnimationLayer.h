/**
 *Copyright (c) 2020 LZ.All Right Reserved
 *Author : LZ
 *Date: 2020.2.4
 *Email: 2117610943@qq.com
 */

#pragma once
#include "cocos2d.h"
#include "Based/LZLevelData.h"
#include "Based/LZGlobalVariable.h"

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
	static Zombies* createDifferentZombies(ZombiesType zombiesType, Node* node);
	static Plants* createDifferentPlants(PlantsType plantsType, Node* node);
	static Bullet* createDifferentBullet(BulletType bulletType, Node* node);
	virtual void addLayer(Node* node, const int order, const string& name) { node->addChild(this, order, name);}
	virtual void plantPlants();
	virtual void deletePlants();
	virtual void createZombies();
	virtual void createZombiesOnSurvival();
	virtual GSAnimationLayer* getAnimationLayer();

CC_CONSTRUCTOR_ACCESS:
	GSAnimationLayer(Node* node = nullptr);
	~GSAnimationLayer();
	virtual bool init();

protected:
	virtual void createRandomSuns();
	virtual void showCars();
	virtual void gameMainLoop(float delta);
	virtual void zombiesEventUpdate(float delta);
	virtual void plantsEventUpdate();
	virtual void plantsDeleteUpdate(map<int,Plants*>::iterator& plant);
	virtual void bulletEventUpdate();
	virtual void sunsDeleteUpdate();
	virtual void coinDeleteUpdate();
	virtual void carsEventUpdate();
	
protected:
	Global* _global;
	Node* _gameScene;
	SunFlower* _randomSuns;
	OpenLevelData* _openLevelData;
	default_random_engine _random; 
	random_device _device;
};
