/**
 *Copyright (c) 2021 LZ.All Right Reserved
 *Author : LZ
 *Date: 2021.1.24
 *Email: 2117610943@qq.com
 */

#include "LZMouseEventControl.h"
#include "LZGlobalVariable.h"
#include "LZPlayMusic.h"
#include "LZCoin.h"
#include "Scenes/GameScenes/Adventure/GameScene/LZAGSData.h"
#include "Scenes/GameScenes/BigMap/GameScene/LZBigMapGameScene.h"
#include "Plants/DefensePlants/LZSunFlower.h"

void MouseEventControl::mouseScrollControlListener(ui::ScrollView* scrollview, float move, ui::ScrollView::Direction direction, float time)
{
	const auto minOffset = 0.f;
	const auto maxOffset = 100.f;

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
		Vec2 offset = Vec2::ZERO;
		auto point = t->getLocation();

		if (BigMapGameScene::scrollView)
		{
			//offset = BigMapGameScene::scrollView->getContentOffset();
			point = point * 2 + Vec2(220, 0);
		}
		//goodsRecovery(Vec2(point.x + fabs(offset.x), point.y + fabs(offset.y)), animation);
		goodsRecovery(point, animation);
		return true;
	};
	Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(linster, node);
}

void MouseEventControl::goodsRecovery(Point point, SkeletonAnimation* animation)
{
	for (auto sun : SunsGroup)
	{
		if (sun->getSun()->getBoundingBox().containsPoint(point) && sun->getEnable() && sun->getSun()->getOpacity() > 200)
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
			if (coin->getCoin()->getBoundingBox().containsPoint(point) && coin->getEnable())
			{
				PlayMusic::playMusic("coin");
				Coin::coinRecoveryAction(coin);
				break;
			}
		}
	}
}
