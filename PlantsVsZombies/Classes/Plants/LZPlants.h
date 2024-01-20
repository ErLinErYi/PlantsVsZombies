/**
 *Copyright (c) 2020 LZ.All Right Reserved
 *Author : LZ
 *Date: 2020.2.5
 *Email: 2117610943@qq.com
 */

#pragma once
#include "cocos2d.h"
#include "spine/spine.h"
#include "spine/spine-cocos2dx.h"
#include "json/document.h"

#include "Based/LZGlobalVariable.h"

#define INIT Vec2(-1000,-1000)
#define NOINITIALIZATION  -1
#define PLANTSNUMBERS 32

using namespace spine;
using namespace cocos2d;
using namespace cocos2d::experimental;

class Zombies;

enum class PlantsType
{
	SunFlower = 0,      /* 向日葵 */
	PeaShooter,         /* 豌豆射手 */
	WallNut,            /* 坚果墙 */
	CherryBomb,         /* 樱桃炸弹 */
	PotatoMine,         /* 土豆雷 */
	CabbagePult,        /* 卷心菜投手 */
	Torchwood,          /* 火炬树桩 */
	Spikeweed,          /* 地刺 */
	Garlic,             /* 大蒜 */
	ScaredyShroom,      /* 胆小姑 */
	IceBergLettuce,     /* 冰莴苣 */
	Marigold,           /* 金盏花 */
	Imitater,           /* 模仿者 */
	Chomper,            /* 大嘴花 */
	Squash,             /* 倭瓜 */
	IcePeaShooter,      /* 寒冰豌豆射手 */
	FirePeaShooter,     /* 火焰豌豆射手 */
	AcidLemonShooter,   /* 强酸柠檬射手 */
	Blover,             /* 三叶草 */
	SunFlowerTwin,      /* 双胞向日葵 */
	WaterMelonPult,     /* 西瓜投手 */
	Pumpkin,            /* 南瓜头 */
	Jalapeno,           /* 火爆辣椒 */
	MagnetShroom,       /* 磁力菇 */
	JalapenoVariation,  /* 火爆辣椒变异 */
	FumeShroom,         /* 大喷菇 */
	ThreePeaShooter,    /* 三头豌豆射手 */
	StarFruit,          /* 杨桃 */
	WinterMelonPult,    /* 冰瓜投手 */
	Citron,             /* 离子缘 */
	CatTail,            /* 香蒲 */
	GloomShroom,        /* 忧郁菇 */
	//...
	None = 34
};  

/**
 *植物卡牌信息
 */
class PlantsInformation
{
public:
	struct PlantsCards
	{
		PlantsCards() :
			timeBarIsFinished(false)
		{}
		Button* plantsCards;            /* 卡牌 */
		Text* plantsCardText;           /* 卡牌文字 */
		ProgressTimer* progressTimer;   /* 倒计时 */
		float plantsCoolTime;           /* 植物冷却时间 */
		int plantsNeedSunNumbers;       /* 所需阳光 */
		bool timeBarIsFinished;         /* 倒计时是否完成 */
		int tag;                        /* 编号 */
	};

	struct point { int x, y; };
	struct PlantsCardInformation
	{
		const string information[3];              // 文字信息
		const unsigned int quality;               // 品质
		unsigned int priority;                    // 优先级
		const int plantsNeedSunNumbers;           // 植物所需阳光数
		const float plantsCoolTime;               // 植物冷却时间
		float PlantsSurPlusCoolTime;              // 植物剩余冷却时间
		float PlantsSurPlusPrecent;               // 植物剩余进度百分比
		const bool flippedX;                      // 左右反转
		const PlantsType type;                    // 植物类型
		const Color3B color;                      // 颜色
		const point requirement{ 0,0 };           // 要求
	};
};

extern PlantsInformation::PlantsCardInformation plantsCardInformation[];

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
	 * 存储植物特有信息  
	 * 子类重写该方法
	 */
	virtual void cavePlantInformation(rapidjson::Value& object, rapidjson::Document::AllocatorType& allocator) {};

	/**
	 * 读取植物特有信息  
	 * 子类重写该方法
	 */
	virtual void readPlantInforamtion(rapidjson::Document* levelDataDocument, char* key, int i) {};

	/**
	 * 设置植物其他信息 
	 */
	virtual void setPlantOtherInformation(rapidjson::Document* levelDataDocument, char* key, int i) {};

	/**
	 * 显示植物介绍信息与动画  
	 */
	virtual SkeletonAnimation* showPlantAnimationAndText() = 0;

	/**
	 * 设置植物动画
	 */
	virtual void setPlantNoramlAnimation() {};

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
	virtual void setPlantScale(float scale);

	/**
	 * 从地图上删除植物 
	 */
	virtual void setPlantRemoveFromMap();

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
	virtual int getPlantRow() const { return _rowAndColumn.y; }

	/**
	 *@ 3.获取植物所在列
	 */
	virtual int getPlantColumn() const { return _rowAndColumn.x; }

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
	virtual ~Plants();
	
protected:
	/**
	 * 创建监听 
	 */
	virtual void createListener() {};
	virtual void createListener(std::string animationName, std::string actionName = "") {};
	virtual void createListener(std::string animationName, std::string actionName = "", std::string musicName = "") {};

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
	virtual bool getZombieIsTheFrontOfPlant(Zombies* zombie);

	/**
	 * 获取僵尸是否于植物在同一列
	 */
	virtual bool getZombieIsSameColumnWithPlant(Zombies* zombie);

	/**
	 *获取僵尸是否于植物在同一行
	 */
	virtual bool getZombieIsSameLineWithPlant(Zombies* zombie);

	/**
	 *获取僵尸是否遇到植物
	 */
	virtual bool getZombieIsEncounterPlant(Zombies* zombie);

	/**
	 *获取当前位置是否存在保护类植物
	 */
	virtual bool getPositionHasProtectPlants();

	/**
	 *僵尸吃植物
	 */
	virtual void zombieEatPlant(Zombies* zombie);

	/**
	 * 僵尸吃植物处理 
	 */
	virtual void setZombieEatPlantControl(Zombies* zombie);

	/**
	 *僵尸攻击植物 
	 */
	virtual void zombieAttackPlant(Zombies* zombie);

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
	int _plantTag;                              // 植物标签
	int _zOrder;                                // 绘制顺序
	int _plantNumber;                           // 存储植物编号（唯一性 ）
	bool _isLoop;                               // 是否循环
	bool _isCanDelete[2];                       // 是否可以删除
	float _combatEffecttiveness;                // 战斗力
	float _coolDownTime;                        // 冷却时间
	float _healthPoint;                         // 生命值
	float _totalHealthPoint;                    // 生命总值
	Vec2 _position;                             // 位置
	Vec2 _rowAndColumn;                         // 详细地址,所在行列
	SkeletonAnimation* _plantAnimation;         // 植物动画
	string _plantAnimationName;                 // 植物动画名称
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