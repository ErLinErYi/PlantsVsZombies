/**
 *Copyright (c) 2020 LZ.All Right Reserved
 *Author : LZ
 *Date: 2021.7.10
 *Email: 2117610943@qq.com
 */

#pragma once
#include "LZBullet.h"

class ScaredyShroomBullet :public Bullet
{
public:
	/**
	 *创建子弹
	 */
	virtual void createBullet() override;

	/**
	 *子弹与僵尸碰撞检测
	 */
	virtual void bulletAndZombiesCollision() override;

	/**
	 *创建子弹影子
	 */
	virtual void createShadow() override;

CC_CONSTRUCTOR_ACCESS:
	ScaredyShroomBullet(Node* node = nullptr);
	~ScaredyShroomBullet();

private:
	void createBulletExplode();
	bool getBulletIsEncounterWithZombie(Zombies* zombie);
	void readBulletAnimationInformation(rapidjson::Document* levelDataDocument, char* key, int i) override;
};