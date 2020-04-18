/**
 *Copyright (c) 2020 LZ.All Right Reserved
 *Author : LZ
 *Date: 2020.2.18
 *Email: 2117610943@qq.com
 */

#pragma once
#include "Bullet.h"

class Pea :public Bullet
{
public:
	/**
	 *设置是否燃烧
	 */
	virtual void setIsFire(const bool isFire);

	/**
	 *设置燃烧次数
	 */
	virtual void setPeaFireNumbers(const int fireNumbers);

	/**
	 *增加子弹燃烧次数
	 */
	virtual void addPeaFireNumbers();

	/**
	 *设置火炬树桩编号
	 */
	virtual void setTorchwoodTag(const int torchwoodTag);

	/**
	 *获取是否燃烧
	 */
	virtual bool getIsFire() const;

	/**
	 *获取燃烧次数
	 */
	virtual int getPeaFireNumbers() const;

	/**
	 *获取火炬树桩编号
	 */
	virtual int getTorchwoodTag() const;

protected:
	/**
	 *创建豌豆
	 */
	virtual void createBullet() override;

	/**
	 *豌豆与僵尸碰撞和检测
	 */
	virtual void bulletAndZombiesCollision() override;

	/**
	 *创建豌豆爆炸动画
	 */
	virtual void  createPeaExplode();

	/**
	 *子弹初始化
	 */
	virtual void bulletInit() override;

	/**
	 *创建子弹影子
	 */
	virtual void createShadow() override;

	/**
	 *子弹攻击僵尸
	 */
	virtual void bulletAttackZombies();

	/**
	 *火子弹攻击僵尸
	 */
	virtual void fireBulletAttackZombies();

CC_CONSTRUCTOR_ACCESS:
	Pea(Node* node = nullptr);
	~Pea();

private:
	void attackZombies();
	float getZombieInExplodeRange(Zombies* zombie);

protected:
	bool _isFire;                         // 是否燃烧 
	int _fireNumbers;                     // 燃烧次数
	int _torchwoodTag;                    // 火炬树桩标记
	string _peaAnimationName;             // 豌豆动画名称
};
