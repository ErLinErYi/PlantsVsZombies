/**
 *Copyright (c) 2021 LZ.All Right Reserved
 *Author : LZ
 *Date: 2021.4.22
 *Email: 2117610943@qq.com
 */

#pragma once
#include "LZNewspaperZombies.h"

class NewspaperBrickZombies :public NewspaperZombies
{
public:
	static NewspaperBrickZombies* create(Node* node = nullptr);
	virtual void createZombie() override;
	virtual void createPreviewZombie() override;

CC_CONSTRUCTOR_ACCESS:
	NewspaperBrickZombies(Node* node = nullptr);

protected:
	virtual void zombieInjuredEventUpdate() override;
	virtual void setZombieBodyShieldThirdInjure(const string& oldName, const string& newName) override;
	virtual void setZombieHeadShieldThirdInjure(const string& oldName, const string& newName) override;
};
