/**
 *Copyright (c) 2020 LZ.All Right Reserved
 *Author : LZ
 *Date: 2020.2.5
 *Email: 2117610943@qq.com
 */

#pragma once
#include "cocos2d.h"
#include "spine/spine.h"
#include "Based/GlobalVariable.h"
#include "spine/spine-cocos2dx.h"

#define INIT Vec2(-1000,-1000)
#define NOINITIALIZATION  -1

using namespace spine;
using namespace cocos2d;
using namespace cocos2d::experimental;

class Zombies;

enum class PlantsType
{
	None = -1,
	SunFlower,          /* 向日葵 */
	PeaShooter,         /* 豌豆射手 */
	WallNut,            /* 坚果墙 */
	CherryBomb,         /* 樱桃炸弹 */
	PotatoMine,         /* 土豆雷 */
	CabbagePult,        /* 卷心菜投手 */
	Torchwood,          /* 火炬树桩 */
	Spikeweed,          /* 地刺 */
	Garlic,             /* 大蒜 */
	FirePeaShooter,     /* 火焰豌豆射手 */
	Jalapeno,           /* 火爆辣椒 */
	AcidLemonShooter,   /* 强酸柠檬射手 */
	Citron              /* 离子缘 */
	//...
};  

class  Plants :public Node
{
public:
	/**
	 *暂停植物所有动作
	 */
	static void stopPlantsAllAction();

	/**
	 *创建图片
	*/
	virtual Sprite* createPlantImage() = 0;

	/**
	 *创建植物动画
	 */
	virtual void createPlantAnimation() = 0;

	/**
	 *判断僵尸与植物之间的相对位置
	 */
	virtual void determineRelativePositionPlantsAndZombies();

	/**
	 *检测植物生命情况
	 */
	virtual void checkPlantHealthPoint() {}

	/**
	 *设置节点
	 */
	virtual void setPlantNode(Node* node) { _node = node; }

	/**
	 *设置位置
	 */
	virtual void setPlantPosition(const Vec2& position) { _position = position; }

	/**
	 *设置绘制顺序
	 */
	virtual void setPlantLocalZOrder(const int& order) { _zOrder = order; }

	/**
	 *设置植物所在行列
	 */
	virtual void setPlantRowAndColumn(const Vec2& rowAndColumn) { _rowAndColumn = rowAndColumn; }

	/**
	 *设置植物是否可见
	 */
	virtual void setPlantVisible(const bool visible);

	/**
	 *获取植物类型
	 */
	PlantsType getPlantType() const { return _plantsType; }

	/**
	 *设置植物标记
	 */
	virtual void setPlantTag(const int& tag) { _plantTag = tag; }

	/**
	 *设置植物生命值
	 */
	virtual void setPlantHealthPoint(int healthPoint) { _healthPoint = healthPoint; }

	/**
	 *设置植物大小
	 */
	virtual void setPlantScale();

	/**
	 *获取植物标记
	 */
	virtual int getPlantTag() const { return _plantTag; }

	/**
	 *获取动画
	 */
	SkeletonAnimation* getPlantAnimation() { return _plantAnimation; }

	/**
	 *判断植物是否存活
	 */
	virtual bool getPlantIsSurvive() const;

	/**
	 *获取植物生命值
	 */
	virtual float getPlantHealthPoint() const { return _healthPoint; }

	/**
	 *@ 1.获取植物所在行列
	 */
	virtual Vec2 getPlantRowAndColumn() const { return _rowAndColumn; }

	/**
	 *@ 2.获取植物所在行
	 */
	virtual int getPlantRow() const { return _rowAndColumn.x; }

	/**
	 *@ 3.获取植物所在列
	 */
	virtual int getPlantColumn() const { return _rowAndColumn.y; }

	/**
	 *获取是否显示
	 */
	virtual bool getPlantVisible();

	/**
	 *获取植物是否可以删除
	 */
	virtual bool* getPlantIsCanDelete();

CC_CONSTRUCTOR_ACCESS:
	Plants(Node* node = nullptr, const Vec2& position = Vec2::ZERO);
	~Plants();
	
protected:
	/**
	 *种植植物
	 */
	virtual SkeletonAnimation* plantInit(const std::string& plantname, const std::string& animaionname);

	/**
	 *创建植物图片
	 */
	virtual void imageInit(const std::string& name, const Vec2& position);

	/**
	 *减少植物生命值
	 */
	virtual void reducePlantHealthPoint(int number) { _healthPoint -= number; }

	/**
	 *泥土飞溅动画
	 */
	virtual void setPlantSoilSplashAnimation(const float& scale);

	/**
	 *设置植物影子
	 */
	virtual void setPlantShadow(const float& scale);

	/**
	 *设置植物受到伤害闪烁
	 */
	virtual void setPlantHurtBlink();
	virtual void setPlantHurtBlink(PlantsType type) const;

	/**
	 *获取僵尸是否在植物前方
	 */
	virtual bool getZombieIsTheFrontOfPlant(Zombies* zombie) const;

	/**
	 *获取僵尸是否于植物在同一行
	 */
	virtual bool getZombieIsSameLineWithPlant(Zombies* zombie) const;

	/**
	 *获取僵尸是否遇到植物
	 */
	virtual bool getzombieIsEncounterPlant(Zombies* zombie) const;

	/**
	 *僵尸吃植物
	 */
	virtual void zombieEatPlant(Zombies* zombie);

	/**
	 *僵尸从吃植物中恢复
	 */
	virtual void zombieRecoveryMove(Zombies* zombie);

private:
	void setPlantGLProgram();

protected:
	int _sunNeed;                               // 阳光需求
	int _costGold;                              // 花费金币
	int _costMasonry;                           // 花费砖石
	int _combatEffecttiveness;                  // 战斗力
	int _plantTag;                              // 植物标签
	int _zOrder;                                // 绘制顺序
	int _plantNumber;                           // 存储植物编号（唯一性 ）
	bool _isLoop;                               // 是否循环
	bool _isCanDelete[2];                       // 是否可以删除
	float _coolDownTime;                        // 冷却时间
	float _healthPoint;                         // 生命值
	float _totalHealthPoint;                    // 生命总值
	Vec2 _position;                             // 位置
	Vec2 _rowAndColumn;                         // 详细地址,所在行列
	SkeletonAnimation* _plantAnimation;         // 植物动画
	Node* _node;                                // 节点
	Global* _global;                            // 全局变量
	Sprite* _plantImage;                        // 植物图片
	Sprite* _plantShadow;                       // 植物影子
	PlantsType _plantsType;                     // 植物类型
	SkeletonAnimation* _soilSplashAnimation;    // 泥土飞溅动画
	static int plantNumber;                     // 植物编号（唯一性）

private:
	static GLProgram* _normalGLProgram;
	static GLProgram* _highLightGLProgram;
	GLProgramState* _highLightGLProgramState;
	float _highLightIntensity;
};