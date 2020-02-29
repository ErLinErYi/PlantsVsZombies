/**
 *Copyright (c) 2020 LZ.All Right Reserved
 *Author : LZ
 *Date: 2020.2.17
 *Email: 2117610943@qq.com
 */

#include "BattlePlants.h"

#include "Zombies/Zombies.h"

BattlePlants::BattlePlants()
{
}

BattlePlants::~BattlePlants()
{
}

void BattlePlants::hurtZombies(Zombies* zombie)
{
	if (zombie->getZombieCurrentBodyShieldVolume() < _combatEffecttiveness) /* 如果当前身体护盾血量小于爆炸伤害 */
	{
		if (zombie->getZombieCurrentBodyShieldVolume() + zombie->getZombieCurrentHeadShieldVolume() < _combatEffecttiveness) /* 如果当前身体护盾加头部护盾血量小于爆炸伤害 */
		{
			if (zombie->getZombieCurrentBodyShieldVolume() + zombie->getZombieCurrentHeadShieldVolume() + zombie->getZombieCurrentBloodVolume() <= _combatEffecttiveness) /* 如果僵尸所有血量小于爆炸伤害（僵尸死亡） */
			{
				/* 僵尸死亡 */
				zombie->setZombieCurrentBloodVolume(0);
				zombie->setZombieCurrentHeadShieldVolume(0);
				zombie->setZombieCurrentBodyShieldVolume(0);
			}
			else
			{
				/* 计算僵尸本体血量 */
				zombie->setZombieCurrentBloodVolume(zombie->getZombieCurrentBodyShieldVolume() + zombie->getZombieCurrentHeadShieldVolume() + zombie->getZombieCurrentBloodVolume() - _combatEffecttiveness);
				zombie->setZombieCurrentHeadShieldVolume(0);
				zombie->setZombieCurrentBodyShieldVolume(0);
			}
		}
		else
		{
			/* 计算僵尸护盾剩于血量 */
			zombie->setZombieCurrentHeadShieldVolume(zombie->getZombieCurrentBodyShieldVolume() + zombie->getZombieCurrentHeadShieldVolume() - _combatEffecttiveness);
			zombie->setZombieCurrentBodyShieldVolume(0);
		}
	}
	else
	{
		zombie->setZombieCurrentBodyShieldVolume(zombie->getZombieCurrentBodyShieldVolume() - _combatEffecttiveness);
	}
}