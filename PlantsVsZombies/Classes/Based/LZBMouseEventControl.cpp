/**
 *Copyright (c) 2021 LZ.All Right Reserved
 *Author : LZ
 *Date: 2021.1.24
 *Email: 2117610943@qq.com
 */

#include "LZBMouseEventControl.h"
#include "LZBGlobalVariable.h"
#include "LZBPlayMusic.h"
#include "LZBCoin.h"
#include "Scenes/GameScene/LZSGData.h"
#include "Plants/DefensePlants/LZPDSunFlower.h"

void MouseEventControl::mouseScrollControlListener(ui::ScrollView* scrollview, float move, ui::ScrollView::Direction direction, float time)
{
	auto minOffset = 0.f;
	auto maxOffset = 100.f;

	auto offset =
		direction == ui::ScrollView::Direction::VERTICAL ?
		scrollview->getScrolledPercentVertical() :
		scrollview->getScrolledPercentHorizontal();

	offset += move;

	if (offset < minOffset) offset = minOffset;
	else if (offset > maxOffset) offset = maxOffset;

	direction == ui::ScrollView::Direction::VERTICAL ?
		scrollview->scrollToPercentVertical(offset, time, true) :
		scrollview->scrollToPercentHorizontal(offset, time, true);
}

void MouseEventControl::goodsRecovery(Node* node, SkeletonAnimation* animation)
{
	auto linster = EventListenerTouchOneByOne::create();
	linster->onTouchBegan = [=](Touch* t, Event* e)
	{
		Point p = t->getLocation();
		if (Global::getInstance()->userInformation->getIsMirrorScene())
		{
			p.x = 1920 - p.x;
		}
		for (auto sun : SunsGroup)
		{
			if (sun->getSun()->getBoundingBox().containsPoint(p) && sun->getEnable())
			{
				PlayMusic::playMusic("points");
				SunFlower::sunRecovery(sun);
				break;
			}
		}

		for (auto coin : CoinsGroup)
		{
			if (animation)
			{
				if (coin->getCoin()->getBoundingBox().intersectsRect(animation->getBoundingBox()) && coin->getEnable())
				{
					PlayMusic::playMusic("coin");
					Coin::coinRecoveryAction(coin);
					break;
				}
			}
			else
			{
				if (coin->getCoin()->getBoundingBox().containsPoint(p) && coin->getEnable())
				{
					PlayMusic::playMusic("coin");
					Coin::coinRecoveryAction(coin);
					break;
				}
			}
		}
		return true;
	};
	Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(linster, node);
}
