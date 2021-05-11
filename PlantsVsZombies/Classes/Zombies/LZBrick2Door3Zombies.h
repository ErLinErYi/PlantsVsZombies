/**
 *Copyright (c) 2021 LZ.All Right Reserved
 *Author : LZ
 *Date: 2021.4.23
 *Email: 2117610943@qq.com
 */

#pragma once
#include "LZDoorZombies3.h"

class Brick2Door3Zombies :public DoorZombies3
{
public:
	static Brick2Door3Zombies* create(Node* node = nullptr);
	virtual void createZombie() override;
	virtual void createPreviewZombie() override;

CC_CONSTRUCTOR_ACCESS:
	Brick2Door3Zombies(Node* node = nullptr);

protected:
	virtual void zombieInjuredEventUpdate() override;
	virtual void setZombieBodyShieldThirdInjure(const string& oldName, const string& newName) override;
	virtual void setZombieHeadShieldThirdInjure(const string& oldName, const string& newName) override;
};
