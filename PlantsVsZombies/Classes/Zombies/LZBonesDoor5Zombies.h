/**
 *Copyright (c) 2021 LZ.All Right Reserved
 *Author : LZ
 *Date: 2021.4.23
 *Email: 2117610943@qq.com
 */

#pragma once
#include "LZBonesZombies.h"

class BonesDoor5Zombies :public BonesZombies
{
public:
	static BonesDoor5Zombies* create(Node* node = nullptr);
	virtual void createZombie() override;
	virtual void createPreviewZombie() override;
	
CC_CONSTRUCTOR_ACCESS:
	BonesDoor5Zombies(Node* node = nullptr);

protected:
	virtual void zombieInjuredEventUpdate() override;
	virtual void setZombieBodyShieldThirdInjure(const string& oldName, const string& newName) override;
	virtual void setZombieHeadShieldThirdInjure(const string& oldName, const string& newName) override;
	virtual void setZombieSecondaryInjure() override;
};