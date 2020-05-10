/**
 *Copyright (c) 2020 LZ.All Right Reserved
 *Author : LZ
 *Date: 2020.5.08
 *Email: 2117610943@qq.com
 */

#include "SGSSurvivalControlLayer.h"
#include "../GSZombiesAppearControl.h"
#include "../GSAnimationLayer.h"
#include "../GSInformationLayer.h"
#include "../GSData.h"
#include "../GSDefine.h"
#include "../../SelectPlantsScene/SelectPlantsScene.h"

#include "Based/PlayMusic.h"

unsigned int SGSSurvivalControlLayer::_frequencyNumbers = 0;

void SGSSurvivalControlLayer::judgeLevelIsFinished()
{
	/* 关卡结束 */
	if (ZombiesGroup.size() <= 0 && _zombiesAppearControl->getZombiesAppearFrequency() >= 10)
	{
		CURSOR_VISIBLE(true);

		_director->pushScene(SelectPlantsScene::createScene());
	}
}

void SGSSurvivalControlLayer::createZombies()
{
	/* 刷新僵尸 */
	if (_zombiesAppearControl->getLastFrequencyZombiesWasDeath())
	{
		_zombiesAppearControl->setLastFrequencyZombiesWasDeath(false);
		_zombiesAppearControl->setTimeClear(); /* 距离上一波刷新时间清零 */
		if (_zombiesAppearControl->getZombiesAppearFrequency() < 10)
		{
			unsigned int zombiesNumbers = getZombiesNumbers();
			for (unsigned int i = 0; i < zombiesNumbers; ++i)
			{
				animationLayerInformation->createZombiesOnSurvival();
			}
			/* 进攻波数自增一 */
			_zombiesAppearControl->setZombiesAppearFrequency();
			++_frequencyNumbers;

			/* 进度条更新 */
			informationLayerInformation->updateProgressBar(_zombiesAppearControl->getZombiesAppearFrequency(), 10);
		}
	}
	informationLayerInformation->updateProgressBarHead();

	/* 控制僵尸的刷新 */
	if (controlRefurbishZombies())
	{
		_zombiesAppearControl->setLastFrequencyZombiesWasDeath(true);
		_zombiesAppearControl->setIsBegin(false);
	}
}

bool SGSSurvivalControlLayer::controlRefurbishZombies()
{
	if ((Zombies::getZombiesNumbers() <= 4 && _frequencyNumbers > 3)                    /* 如果活着的僵尸数小于规定，刷新下一波 */

		|| (Zombies::getZombiesNumbers() <= 0 && _frequencyNumbers > 1                  /* 如果没有存活僵尸则立即刷新僵尸 */)

		|| (_zombiesAppearControl->getTime() >= 18 && _frequencyNumbers == 0)           /* 第一波刷新控制 */

		|| (_zombiesAppearControl->getTime() >= 30 + rand() % 10 &&
			(_frequencyNumbers == 1 || _frequencyNumbers == 2))                         /* 第二三波刷新控制 */

		|| (_zombiesAppearControl->getTime() >= 40 && _frequencyNumbers > 2)            /* 如果大于45秒刷新下一波 */
		)
	{
		return true;
	}
	return false;
}

void SGSSurvivalControlLayer::controlRefurbishMusicAndText()
{
	/* 控制大波僵尸来袭的文字与音乐 */
	if ((_zombiesAppearControl->getTime() >= 18 && _frequencyNumbers / 10 == 0) ||
		(_zombiesAppearControl->getTime() >= 5 && _frequencyNumbers / 10 > 0) &&
		_zombiesAppearControl->getZombiesAppearFrequency() == 0)
	{
		PlayMusic::playMusic("awooga");
	}

	/* 僵尸的文字显示与旗子更新 */
	if (_zombiesAppearControl->getZombiesAppearFrequency() > 0 && 
		_zombiesAppearControl->getZombiesAppearFrequency() % 5 == 0 && !_zombiesAppearControl->getIsShowWords())
	{
		if (informationLayerInformation->updateProgressBarFlag())
		{
			_zombiesAppearControl->setIsShowWords(true);
		}
	}
	if (_zombiesAppearControl->getZombiesAppearFrequency() % 5 != 0 && _zombiesAppearControl->getIsShowWords())
	{
		_zombiesAppearControl->setIsShowWords(false);
	}
}

int SGSSurvivalControlLayer::getZombiesNumbers()
{
	auto number = static_cast<int>(pow(_frequencyNumbers + 1, 2));
	if (number > 50)number = 50;
	return number + number % number / 2;
}

void SGSSurvivalControlLayer::setZombiesAppearFrequency(const unsigned int frequency)
{
	_frequencyNumbers = frequency;
}

unsigned int SGSSurvivalControlLayer::getZombiesAppearFrequency() const
{
	return _frequencyNumbers;
}
