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
#include "Based/GlobalVariable.h"
#include "spine\spine-cocos2dx.h"
#include "Zombies/Zombies.h"

using namespace spine;
using namespace cocos2d;

class SPSBackgroundLayer :public Layer
{
public:
	CREATE_FUNC(SPSBackgroundLayer);

	/**
	 *ÃÌº”µΩ÷˜≥°æ∞÷–
	 */
	void addLayer(Node* node) { node->addChild(this, 0, "_scrollLayer"); }

CC_CONSTRUCTOR_ACCESS:
	SPSBackgroundLayer();
	~SPSBackgroundLayer();
	virtual bool init();

protected:
	virtual void createBackGroundEffect();

private:
	/* ‘§¿¿Ω© ¨ */
	typedef struct
	{
		SkeletonAnimation* animation;
		float position_y;
	}PreviewZombies;
	std::vector<PreviewZombies> _previewZombies;

private:
	void createMordernBackgroundImage();
	void createFireEffect(Sprite* name);
	void createPreviewZombie();
	void createMordernPreviewZombies();
	void setBackgroundImagePosition();
	void sortZombiesPosition();
	Zombies* createDifferentZombies(const int& id);
	static bool cmp(PreviewZombies& a, PreviewZombies& b);

private:
	int _previewZombiesTag;      // ‘§¿¿Ω© ¨±‡∫≈
	Global* _global;
	OpenLevelData* _openLevelData;
};