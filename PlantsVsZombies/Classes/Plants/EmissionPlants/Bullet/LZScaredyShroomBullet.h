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
	 *�����ӵ�
	 */
	virtual void createBullet() override;

	/**
	 *�ӵ��뽩ʬ��ײ���
	 */
	virtual void bulletAndZombiesCollision() override;

	/**
	 *�����ӵ�Ӱ��
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