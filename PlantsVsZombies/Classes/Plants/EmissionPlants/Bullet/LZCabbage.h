/**
 *Copyright (c) 2020 LZ.All Right Reserved
 *Author : LZ
 *Date: 2020.2.18
 *Email: 2117610943@qq.com
 */

#pragma once
#include "LZBullet.h"

class Cabbage :public Bullet
{
public:
	virtual void createBullet() override;
	virtual void bulletAndZombiesCollision() override;
	virtual void createShadow(float scale) override;
	virtual void setZombiePosition(const Vec2& position);
	virtual void setZombieSpeed(const float speed);
	virtual void setZombieHeight(const float height);
	virtual void setIsFileData(const bool isFileData);
	virtual float getZombieSpeed() const;
	virtual void setBulletCurrentPosition(const Vec2& position);
	virtual Vec2 getBulletInitialPosition();
	virtual Vec2 calculateZombiePosition();

CC_CONSTRUCTOR_ACCESS:
	Cabbage(Node* node = nullptr);
	~Cabbage();

protected:
	virtual void caveBulletInformation(rapidjson::Value& object, rapidjson::Document::AllocatorType& allocator) override;
	virtual void readBulletInformation(rapidjson::Document* levelDataDocument, char* key, int i) override;
	virtual void bulletAttackHurtZombies(Zombies* zombie) override;
	virtual void createListener(const string& actionName, float scale = 0.8f);
	virtual void createExplodeAnimation(const string& animationName, const string& actionName, const float scale = 0.6f);
	virtual void calculateInformationForReadFile();
	virtual bool getBulletIsEncounterWithZombie(Zombies* zombie);

protected:
	Vec2 _zombiePosition;
	Vec2 _currentPosition;
	Vec2 _initPosition;
	Vec2 _endPosition;
	float _zombieSpeed;
	float _zombieHeight;
	float _actionTime;
	float _acxtionHeight;
	bool _isFileData;      // 是否读取文件数据标志
};