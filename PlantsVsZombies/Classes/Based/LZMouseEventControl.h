/**
 *Copyright (c) 2021 LZ.All Right Reserved
 *Author : LZ
 *Date: 2021.1.24
 *Email: 2117610943@qq.com
 */

#pragma once
#include "cocos2d.h"
#include "ui/UIScrollView.h"
#include "spine/spine.h"
#include "spine/spine-cocos2dx.h"

using namespace cocos2d;
using namespace spine;

class MouseEventControl
{
public :
	static void mouseScrollControlListener(ui::ScrollView* scrollview, 
		float move, ui::ScrollView::Direction direction, float time = 0.5f);
	static void goodsRecovery(Node* node, SkeletonAnimation* animation = nullptr);

private:
	static void goodsRecovery(Point point, SkeletonAnimation* animation);
};