/**
 *Copyright (c) 2021 LZ.All Right Reserved
 *Author : LZ
 *Date: 2021.4.21
 *Email: 2117610943@qq.com
 */

#pragma once
#include "LZNewspaperZombies.h"

class StrongNewspaperZombies :public NewspaperZombies
{
public:
	static StrongNewspaperZombies* create(Node* node = nullptr);
	virtual void createZombie() override;
	virtual void createPreviewZombie() override;
	virtual Sprite* createPreviewZombieImage() override;
	
CC_CONSTRUCTOR_ACCESS:
	StrongNewspaperZombies(Node* node = nullptr);

protected:
	virtual void zombieInjuredEventUpdate() override;
}; 
