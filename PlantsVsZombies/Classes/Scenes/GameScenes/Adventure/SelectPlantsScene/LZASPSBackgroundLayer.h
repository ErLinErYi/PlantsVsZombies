/**
 *Copyright (c) 2019 LZ.All Right Reserved
 *Author : LZ
 *Date: 2019.8.12
 *Modify: 2020.2.3
 *Email: 2117610943@qq.com
 */

#pragma once
#include "cocos2d.h"
#include "spine/spine.h"
#include "spine/spine-cocos2dx.h"
#include "Based/LZGlobalVariable.h"

using namespace spine;
using namespace cocos2d;

class Zombies;
class OpenLevelData;

class SPSBackgroundLayer :public Layer
{
public:
	CREATE_FUNC(SPSBackgroundLayer);

CC_CONSTRUCTOR_ACCESS:
	SPSBackgroundLayer();
	~SPSBackgroundLayer();
	virtual bool init();

protected:
	/* ‘§¿¿Ω© ¨ */
	typedef struct
	{
		SkeletonAnimation* animation;
		float position_y;
		float position_x;
	}PreviewZombies;
	std::vector<PreviewZombies> _previewZombies;

	using UID = uniform_int_distribution<unsigned>;

protected:
	virtual void createBackGroundEffect();
	virtual void createMordernBackgroundImage();
	virtual void createFireEffect(Sprite* name);
	virtual void showPreviewZombies();
	virtual void createMordernPreviewZombies();
	virtual void setBackgroundImagePosition();
	virtual void createSnowParticle();
	virtual void sortZombiesPosition();
	virtual Vec2 getPreviewZombiesPosition(UID& n1, UID& n2);
	static bool cmp(PreviewZombies& a, PreviewZombies& b);

protected:
	int _previewZombiesTag;      // ‘§¿¿Ω© ¨±‡∫≈
	Global* _global;
	OpenLevelData* _openLevelData;
	default_random_engine _random;
	random_device _device;
};