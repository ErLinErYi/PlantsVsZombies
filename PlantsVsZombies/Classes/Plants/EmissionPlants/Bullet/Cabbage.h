/**
 *Copyright (c) 2020 LZ.All Right Reserved
 *Author : LZ
 *Date: 2020.2.18
 *Email: 2117610943@qq.com
 */

#pragma once
#include "Bullet.h"

class Cabbage :public Bullet
{
public:
	void createBullet() override;
	void bulletAndZombiesCollision() override;
	void bulletInit() override;
	void createShadow() override;
	void createCabbageExplode();
	void setZombiePosition(const Vec2& position);
	void setZombieSpeed(const float speed);
	void setIsFileData(const bool isFileData);
	float getZombieSpeed() const;
	void setCabbageCurrentPosition(const Vec2& position);
	Vec2 getCabbageInitialPosition();
	Vec2 calculateZombiePosition();

CC_CONSTRUCTOR_ACCESS:
	Cabbage(Node* node);
	~Cabbage() {}

private:
	bool getBulletIsSameLineWithZombie(Zombies* zombie);
	bool getBulletIsEncounterWithZombie(Zombies* zombie);
	void bulletAttackHurtZombies(Zombies* zombie) override;

private:
	Vec2 _zombiePosition;
	Vec2 _currentPosition;
	float _zombieSpeed;
	bool _isFileData;
};