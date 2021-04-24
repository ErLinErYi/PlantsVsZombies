/**
 *Copyright (c) 2021 LZ.All Right Reserved
 *Author : LZ
 *Date: 2021.4.21
 *Email: 2117610943@qq.com
 */

#pragma once
#include "LZConeZombies.h"

class ConeZombies4 :public ConeZombies
{
public:
	static ConeZombies4* create(Node* node = nullptr);
	virtual void createZombie() override;
	virtual void createPreviewZombie() override;

CC_CONSTRUCTOR_ACCESS:
	ConeZombies4(Node* node = nullptr);

protected:
	virtual void zombieInjuredEventUpdate() override;
	virtual void setZombieHeadShieldThirdInjure(const string& oldName, const string& newName) override;
};