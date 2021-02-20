/**
 *Copyright (c) 2021 LZ.All Right Reserved
 *Author : LZ
 *Date: 2020.2.18
 *Email: 2117610943@qq.com
 */

#pragma once
#include "LZZombies.h"

class GargantuarZombies :public Zombies
{
public:
	static GargantuarZombies* create(Node* node = nullptr);
	virtual void createZombie() override;
	virtual void createPreviewZombie() override;
	virtual void playZombieSoundEffect() override;
	virtual void playZombiesAshesAnimation() override;
	virtual void zombieLoseHeadAnimation(const std::string& name) override;
	virtual void playZombiesFillDownAnimation() override;

CC_CONSTRUCTOR_ACCESS:
	GargantuarZombies(Node* node = nullptr);

private:
	void zombieInjuredEventUpdate() override;
	void setZombiePrimaryInjure() override;
};