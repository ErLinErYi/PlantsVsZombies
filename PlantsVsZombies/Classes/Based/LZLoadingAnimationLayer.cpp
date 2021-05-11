/**
 *Copyright (c) 2021 LZ.All Right Reserved
 *Author : LZ
 *Date: 2021.4.24
 *Email: 2117610943@qq.com
 */

#include "LZLoadingAnimationLayer.h"
#include "spine/spine.h"

using namespace spine;

bool LoadingAnimationLayer::init()
{
	if (!LayerColor::initWithColor(Color4B(0, 0, 0, 180)))return false;

	createShieldLayer(this);

	auto iter = Global::getInstance()->userInformation->getAnimationData().find("Loading");
	if (iter != Global::getInstance()->userInformation->getAnimationData().end())/* 如果可以找到 */
	{
		auto littlesprite = SkeletonAnimation::createWithData(iter->second);
		littlesprite->setPosition(Director::getInstance()->getWinSize() / 2.f);
		littlesprite->setAnimation(0, "animation", true);
		littlesprite->update(0);
		littlesprite->setScale(1.5f);
		littlesprite->setName("loadAnimation");
		this->addChild(littlesprite);
	}

	return true;
}
