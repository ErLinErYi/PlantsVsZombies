/**
 *Copyright (c) 2019 LZ.All Right Reserved
 *Author : LZ
 *Date: 2020.4.17
 *Email: 2117610943@qq.com
 */

#include "MirrorGameScene.h"

Scene* MirrorGameScene::createScene()
{
    return MirrorGameScene::create();
}

bool MirrorGameScene::init()
{
	if (!Scene::init())return false;

	controlPlayMusic();
	backgroundLayer();   // ±³¾°²ã
	informationLayer();  // ÐÅÏ¢²ã
	buttonLayer();       // °´Å¥²ã
	controlLayer();      // ¿ØÖÆ²ã
	animationLayer();    // ¶¯»­²ã

    pauseGame();
    createMirrorWorld();
	return true;
}

void MirrorGameScene::createMirrorWorld()
{
    auto camControlNode = Node::create();
    camControlNode->setPositionNormalized(Vec2(.5, .5));
    this->addChild(camControlNode);

    auto camNode = Node::create();
    camNode->setPositionZ(Camera::getDefaultCamera()->getPosition3D().z);
    camControlNode->addChild(camNode);

    this->runAction(Sequence::create(DelayTime::create(0.05f),
        CallFunc::create([=]()
            {
                Vec3 rot = camControlNode->getRotation3D();
                rot.y += 180;
                camControlNode->setRotation3D(rot);

                Vec3 worldPos;
                camNode->getNodeToWorldTransform().getTranslation(&worldPos);

                Camera::getDefaultCamera()->setPosition3D(worldPos);
                Camera::getDefaultCamera()->lookAt(camControlNode->getPosition3D());
            }), nullptr));
}
