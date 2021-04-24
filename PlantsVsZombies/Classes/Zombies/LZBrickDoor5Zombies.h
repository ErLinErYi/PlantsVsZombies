/**
 *Copyright (c) 2021 LZ.All Right Reserved
 *Author : LZ
 *Date: 2021.4.24
 *Email: 2117610943@qq.com
 */

#pragma once
#include "LZBrickZombies.h"

class BrickDoor5Zombies :public BrickZombies
{
public:
	static BrickDoor5Zombies* create(Node* node = nullptr);
	virtual void createZombie() override;
	virtual void createPreviewZombie() override;

CC_CONSTRUCTOR_ACCESS:
	BrickDoor5Zombies(Node* node = nullptr);

protected:
	virtual void zombieInjuredEventUpdate() override;
	virtual void setZombieBodyShieldThirdInjure(const string& oldName, const string& newName) override;
	virtual void setZombieHeadShieldThirdInjure(const string& oldName, const string& newName) override;
};
