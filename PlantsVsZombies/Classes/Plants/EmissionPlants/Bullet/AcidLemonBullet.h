/**
 *Copyright (c) 2020 LZ.All Right Reserved
 *Author : LZ
 *Date: 2020.2.25
 *Email: 2117610943@qq.com
 */

#pragma once
#include "Bullet.h"

class AcidLemonBullet :public Bullet
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
	 *子弹初始化
	 */
	virtual void bulletInit() override;

	/**
	 *创建子弹影子
	 */
	virtual void createShadow() override;

CC_CONSTRUCTOR_ACCESS:
	AcidLemonBullet(Node* node = nullptr);
	~AcidLemonBullet();

private:
	void createAcidLemonBulletExplode();
	void setAttackForShield(Zombies* zombie);
};