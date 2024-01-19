/**
 *Copyright (c) 2024 LZ.All Right Reserved
 *Author : LZ
 *Date: 2024.1.15
 *Email: 2117610943@qq.com
 */

#include "LZMirrorWorld.h"

MirrorWorld::MirrorWorld()
{
}

MirrorWorld::~MirrorWorld()
{
}

bool MirrorWorld::init()
{
    if (!Scene::init())return false;

    readWorldLevel();
    createBackground();
    createScrollView();
    createLevelModeText();
    createGoBack();
    createMouseListener();

    createMirrorWorld();

    return true;
}

void MirrorWorld::createMirrorWorld()
{
   this->runAction(Sequence::create(DelayTime::create(0.4f),
        CallFunc::create([this]()
            {
                this->setRotation3D(Vec3(0, 180, 0));
            }), nullptr));


   auto listener = EventListenerTouchOneByOne::create();

   listener->onTouchBegan = [](Touch* touch, Event* event) 
       {

           Point pos = touch->getLocationInView();
           touch->setTouchInfo(touch->getID(), pos.x, pos.y);

           return true;
       };

   listener->onTouchMoved = [](Touch* touch, Event* event) {
       log("HelloWorldScene onTouchMoved");
       };

   listener->onTouchEnded = [=](Touch* touch, Event* event) {
       log("HelloWorldScene onTouchEnded");
       };

   _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);

}

