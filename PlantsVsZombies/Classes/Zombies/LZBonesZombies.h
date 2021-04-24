/**
 *Copyright (c) 2021 LZ.All Right Reserved
 *Author : LZ
 *Date: 2021.4.21
 *Email: 2117610943@qq.com
 */

#pragma once
#include "LZZombies.h"

class BonesZombies :public Zombies
{
public:
	static BonesZombies* create(Node* node = nullptr);
	virtual void createZombie() override;
	virtual void createPreviewZombie() override;
	virtual void playZombieSoundEffect() override;

CC_CONSTRUCTOR_ACCESS:
	BonesZombies(Node* node = nullptr);

protected:
	virtual void zombieInjuredEventUpdate() override;
	virtual void setZombieHeadShieldThirdInjure(const string& oldName, const string& newName) override;
	virtual void setZombieSecondaryInjure() override;
};