/**
 *Copyright (c) 2021 LZ.All Right Reserved
 *Author : LZ
 *Date: 2021.7.21
 *Email: 2117610943@qq.com
 */

#include "LZITemporaryReplaceScene.h"
#include "LZIZombiesScene.h"

TemporaryReplaceScene::TemporaryReplaceScene()
{
}

TemporaryReplaceScene::~TemporaryReplaceScene()
{
}

void TemporaryReplaceScene::onEnterTransitionDidFinish()
{
	this->runAction(Sequence::create(DelayTime::create(0.3f), CallFunc::create([]() 
		{
			Director::getInstance()->replaceScene(IZombiesScene::create()); 
		}), nullptr));
}
