/**
 *Copyright (c) 2021 LZ.All Right Reserved
 *Author : LZ
 *Date: 2021.2.14
 *Email: 2117610943@qq.com
 */
#pragma once
#include "Scenes/GameScenes/Adventure/GameScene/LZAGSControlLayer.h"

class Zombies;

class BMControlLayer :public GSControlLayer
{
public:
	CREATE_FUNC(BMControlLayer);
	static void judgeZombiesWin(Zombies* zombie);
	virtual void addLayer(Node* node, const int order, const string& name) override { node->addChild(this, order, name); }

CC_CONSTRUCTOR_ACCESS:
	BMControlLayer();
	~BMControlLayer();
	virtual bool init() override;

protected:
	virtual void initData() override;                                                            /* 初始化数据 */
	virtual void judgeLevelIsFinished() override;                                                /* 判断关卡是否结束 */
	virtual void createSchedule() override;                                                      /* 创建定时器 */
	virtual void calculatePlantPosition() override;                                              /* 计算植物种植位置 */    
	virtual void mouseMoveControl() override;                                                    /* 鼠标移动控制 */
	virtual void createPreviewPlants() override;                                                 /* 创建预览植物 */
	virtual void mouseDownControl(EventMouse* eventmouse = nullptr) override;                    /* 鼠标按下控制 */
	virtual void checkPlantType(const int type = 0) override;                                    /* 检测植物类型 */
	
public:
	static bool isLose;

protected:
	bool _isFade;
	Vec2 _offset;
};
