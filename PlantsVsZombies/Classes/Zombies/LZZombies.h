/**
 *Copyright (c) 2020 LZ.All Right Reserved
 *Author : LZ
 *Date: 2020.1.26
 *Email: 2117610943@qq.com
 */

#pragma once
#include <random>

#include "cocos2d.h"
#include "spine/spine.h"
#include "spine/spine-cocos2dx.h"

#include "Based/LZLevelData.h"
#include "Based/LZGlobalVariable.h"

#define ZOMBIESNUMBERS 43

using namespace spine;
using namespace cocos2d;
using namespace cocos2d::experimental;

class Plants;
class GSGameResultJudgement;
class GSGameEndLayer;

/* 僵尸类型 */
enum class ZombiesType
{
	None = 0,
	CommonZombies,                  /* 200  1*/       
	CommonFlagZombies,              /* 200  2*/
	LmpZombies,                     /* 200  3*/
	BalloonZombies,                 /* 200  4*/

	ConeZombies,                    /* 600+200 800 5*/
	ConeZombies3,                   /* 600+200 800 6*/
	ConeFlagZombies,                /* 600+200 800 7*/
	DoorZombies,                    /* 600+200 800 8*/
	NewspaperZombies,               /* 600+200 800 9*/

	ConeZombies4,                   /* 700+200 900 10*/

	ConeZombies2,                   /* 700+300 1000 11*/
	WoodZombies3,                   /* 800+200 1000 12*/
	DoorZombies2,                   /* 800+200 1000 13*/

	WoodZombies2,                   /* 900+200 1100 14*/

	StrongNewspaperZombies,         /* 1000+200 1200 15*/
	WoodZombies,                    /* 1000+200 1200 16*/
	DoorZombies3,                   /* 1000+200 1200 17*/

	CommonDoorZombies,              /* 1200+200 1400 18*/
	CommonDoorFlagZombies,          /* 1200+200 1400 19*/
	BucketZombies,                  /* 1200+200 1400 20*/
	BucketFlagZombies,              /* 1200+200 1400 21*/

	BucketZombies2,                 /* 1300+200 1500 22*/
	BucketZombies3,                 /* 1300+200 1500 23*/
	BucketZombies4,                 /* 1300+200 1500 24*/

	BucketZombies5,                 /* 1400+200 1600 25*/
	BonesZombies2,                  /* 1400+200 1600 26*/

	DoorZombies4,                   /* 1600+200 1800 27*/
	BucketZombies6,                 /* 1600+200 1800 28*/

	ConeDoorZombies,                /* 600+1200+200 2000 29*/
	ConeDoorFlagZombies,            /* 600+1200+200 2000 30*/
	BonesZombies,                   /* 1800+200 2000 31*/
	SnowZombies,                    /* 2000 32*/

	DoorZombies5,                   /* 2000+200 2200 33*/
	BrickZombies,                   /* 2000+200 2200 34*/
	BrickZombies2,                  /* 2000+200 2200 35*/

	BucketDoorZombies,              /* 1200+1200+200 2600 36*/
	BucketDoorFlagZombies,          /* 1200+1200+200 2600 37*/

	Bucket4Door4Zombies,            /* 1300+1600+200 3100 38*/

	NewspaperBrickZombies,          /* 1000+2000+200 3200 39*/
	Brick2Door3Zombies,             /* 2000+1000+200 3200 40*/

	BonesDoor5Zombies,              /* 1800+2000+200 4000 41*/

	BrickDoor5Zombies,              /* 2000+2000+200 4200 42*/

	GargantuarZombies               /* 6000  43*/
};

enum class ShieldType
{
	none = 0,
	IronHeadShield,
	WoodHeadShiled,
	PlasticsHeadShield,
	BrickHeadShield,
	BonesHeadShield,
	PaperBodyShield,
	ClothBodyShield,
	BrickBodyShield,
	IronBodyShield,
	StoneBodyShield
};

class Zombies :public Node
{
public:
	static bool compare (Zombies* a, Zombies* b) {
		return a->_zombieRow < b->_zombieRow;
	}
	/**
	 *获取各种僵尸种类动画名称
	 */
	static string getZombieAniamtionName(ZombiesType zombiestype);

	/**
	 *删除僵尸
	 */
	static void zombiesDeleteUpdate(list<Zombies*>::iterator& zombie);
	static void zombiesDeleteUpdateNotRecordDieNumbers(list<Zombies*>::iterator& zombie);

	/**
	 *获取僵尸数量
	 */
	static unsigned int getZombiesNumbers();

	/**
	 *设置僵尸数量
	 */
	static void setZombiesNumbers(const unsigned int numbers);

	/**
	 *僵尸数量自增/自减一
	 *@ ++ 自增1
	 *@ -- 自减1
	 */
	static void zombiesNumbersChange(const string&);

	/**
	 *判断僵尸赢
	 */
	static void judgeZombieWin(list<Zombies*>::iterator zombie);

	/**
	 *僵尸输赢初始化
	 */
	static void zombiesWinOrLoseInit();

	/**
	 *获取高亮
	 */
	static GLProgram* getHighLight();

	/**
	 *奖励金币
	 */
	static void rewardCoin(SkeletonAnimation* zombies);

	/**
	 *创建僵尸
	 */
	virtual void createZombie() = 0;

	/**
	 *创建预览僵尸
	 */
	virtual void createPreviewZombie() = 0;

	/**
	 *僵尸受伤事件更新
	 */
	virtual void zombieInjuredEventUpdate() = 0;

	/**
	 * 读取僵尸特有信息
	 * 子类可以重写该方法
	 */
	virtual void readZombieInformation();

	/**
	 *创建僵尸影子
	 */
	virtual void createZombieShadow();

	/**
	 * 创建僵尸定时器  
	 */
	virtual void createZombieTimer();

	/**
	 * 设置僵尸动作减缓二分之一 
	 */
	void setZombieActionSlow();

	/**
	 * 设置僵尸恢复运动速度 
	 */
	void setZombieActionRecovery(bool slow = false);

	/**
	 * 设置僵尸动作停止
	 */
	void setZombieActionStop();

	/**
	 *设置僵尸大小
	 */
	virtual void setZombieScale(const Size& scale = Size(40, 40)) const;

	/**
	 *设置僵尸大小
	 */
	virtual void setZombieScale(const int& scale) const;

	/**
	 * 设置僵尸所在行 
	 */
	virtual void setZombieInRow(const int row);

	/**
	 *设置动画播放速度
	 */
	virtual void setZombieTimeScale(const float& timeScale) const;

	/**
	 *设置僵尸动画编号
	 */
	virtual void setZombieAnimation(const int tag, const bool isLoop = true) const;

	/**
	 *设置僵尸动画
	 */
	virtual void setZombieAnimation(const string& name, bool isLoop = true) const;

	/**
	 *设置僵尸的大小
	 */
	virtual void setZombieAttributeForGameType();

	/**
	 *设置是否显示僵尸掉落肢体动画
	 */
	virtual void setZombieIsShowLoseLimbsAnimation(const bool isShow);

	/**
	 *设置是否显示僵尸掉落护盾动画
	 */
	virtual void setZombieIsShowLoseShieldAnimation(const bool isShow);

	/**
	 *设置僵尸所吃植物编号
	 */
	virtual void setZombieEatPlantNumber(const int& number);

	/**
	 *设置僵尸移动
	 */
	virtual void setZombieMove(const float delta);

	/**
	 * 设置僵尸移动信息
	 */
	virtual void setZombieMoveInformation();

	/**
	 *设置僵尸透明度
	 */
	virtual void setZombieOpacity(GLubyte opacity);

	/**
	 *设置位置
	 */
	virtual void setZombiePosition(const Vec2& position);

	/**
	 *设置僵尸动画名字
	 */
	virtual void setZombieAnimationName(const string& name);

	/**
	 *设置僵尸编号
	 */
	virtual void setZombieTag(const int& tag);

	/**
	 *设置僵尸当前速度
	 */
	virtual void setZombieCurrentSpeed(const int currentSpeed);

	/**
	 *设置僵尸停止移动
	 */
	virtual void setZombieStop();

	/**
	 *设置僵尸是否显示
	 */
	virtual void setZombieIsShow(const bool isShow);

	/**
	 *设置僵尸是否吃植物
	 */
	virtual void setZombieIsEat(const bool isEat);

	/**
	 *设置僵尸死亡
	 */
	virtual void setZombieDeath(const bool death);

	/**设置僵僵尸是否显示
	 */
	virtual void setZombieVisible(const bool isVisible);

	/**
	 *设置僵尸身体护盾血量
	 */
	virtual void setZombieCurrentBodyShieldVolume(const float currentBodyShieldVolume);

	/**
	 *设置僵尸头部护盾血量
	 */
	virtual void setZombieCurrentHeadShieldVolume(const float currentHeadShieldVolume);

	/**
	 *设置僵尸血量
	 */
	virtual void setZombieCurrentBloodVolume(const float currentBloodVolume);

	/**
	 *设置僵尸是否创建定时器 
	 */
	virtual void setZombieIsCreateTimer(const bool isCreateTimer);

	/**
	 *设置僵尸是否被击飞
	 */
	virtual void setZombieIsStrikeFly(const bool isStrikeFly);

	/**
	 *设置僵尸是否被冻住  
	 */
	virtual void setZombieIsFrozen(const int isFrozen);

	/**
	 *设置僵尸受伤闪烁
	 */
	virtual void setZombieHurtBlink();

	/**
	 *设置僵尸大小
	 */
	virtual void setZombieScale();

	/**
	 * 设置僵尸是否吃大蒜 
	 */
	virtual void setZombieIsEatGarlic(const bool isEatGarlic);

	/**
	 * 设置僵尸血量倍数 
	 */
	virtual void setZombieMultipleBloodVolume(const float multiple);

	/**
	 * 设置僵尸护盾  
	 */
	virtual void setZombieShieldType(ShieldType type, bool head = true);

	/**
	 * 获取僵尸所在行 
	 */
	virtual int getZombieInRow() const;

	/**
	 * 获取僵尸是否吃植物 
	 */
	virtual bool getZombieIsEatPlants();

	/**
	 * 获取僵尸是否可以被攻击 
	 */
	virtual bool getZombieIsCanBeAttack();

	/**
	 *设置僵尸已被某一植物预订杀死
	 */
	virtual void setZombieReserveKill(bool reserveKill);

	/**
	 *设置僵尸定时器时间  
	 */
	virtual void setZombieTimerTime(const int timerTime, bool slow = false);

	/**
	 * 设置 僵尸不同关卡的属性失效
	 */
	virtual void setZombieAttributeForGameTypeInvalid(const bool invalid);

	/**
	 *获取僵尸动画
	 */
	virtual SkeletonAnimation* getZombieAnimation() const;

	/**
	 *僵尸是否存活
	 */
	virtual bool getZombieIsSurvive() const;

	/**
	 *获取僵尸是否显示
	 */
	virtual bool getZombieIsVisible() const;

	/**
	 *获取僵尸名字
	 */
	virtual string getZombieAnimationName() const;

	/**
	 *获取僵尸编号
	 */
	virtual int getZombieTag() const;

	/**
	 *获取僵尸所吃植物编号
	 */
	virtual int getZombieEatPlantNumber() const;

	/**
	 *僵尸没有播放死亡动画
	 */
	virtual bool getZombieIsPlayDieAnimation() const;

	/**
	 *获取僵尸位置
	 */
	virtual Vec2 getZombiePosition()  const;

	/**
	 *获取僵尸x坐标
	 */
	virtual float getZombiePositionX() const;

	/**
	 *获取僵死y坐标
	 */
	virtual float getZombiePositionY() const; 
	
	/**
	 *获取僵尸是否进入地图
	 */
	virtual bool getZombieIsEnterMap() const;

	/**
	 *获取僵尸身体护盾血量
	 */
	virtual float getZombieCurrentBodyShieldVolume() const;

	/**
	 *获取僵尸头部护盾血量
	 */
	virtual float getZombieCurrentHeadShieldVolume() const;

	/**
	 *获取僵尸血量
	 */
	virtual float getZombieCurrentBloodVolume() const;

	/**
	 * 获取当前总血量是总血量的百分比 
	 */
	virtual float getZombieCurrentBloodProportionBloodPrecent() const;

	/**
	 *获取僵尸是否在吃植物
	 */
	virtual bool getZombieIsEat() const;

	/**
	 *获取僵尸是否停止
	 */
	virtual bool getZombieIsStop() const;

	/**
	 *@ 1.获取僵尸当前的速度
	 */
	virtual float getZombieCurrentSpeed() const;

	/**
	 *@ 2.获取僵尸原本速度
	 */
	virtual float getZombieSpeed() const;

	/**
	 *获取僵尸是否显示
	 */
	virtual bool getZombieIsShow() const;

	/**
	 *获取僵尸是否创建定时器  
	 */
	virtual bool getZombieIsCreateTimer() const;

	/**
     *获取定时器时间  
	 */
	virtual int& getZombieTimerTime(bool slow = false);

	/**
	 *获取僵尸是否被冻住  
	 */
	virtual int getZombieIsFrozen() const;

	/**
	 *获取僵尸种类
	 */
	virtual ZombiesType getZombieType() const;

	/**
	 *获取僵尸头部被攻击的音效 
	 */
	virtual int getZombieHeadAttackSoundEffect() const;

	/**
	 *获取僵尸身体被攻击音效
	 */
	virtual int getZombieBodyAttackSoundEffect() const;

	/**
	 *获取是否可删除
	 */
	virtual bool* getIsCanDelete();

	/**
	 *获取僵尸是否有护盾
	 */
	virtual bool getZombieIsHaveShield() const;

	/**
	 *获取僵尸是否被击飞
	 */
	virtual bool getZombieIsStrikeFly() const;

	/**
	 *获取僵尸是否已被某一植物预订杀死
	 */
	virtual bool getZombieReserveKill();

	/**
	 *获取僵尸身体护盾类型
	 */
	ShieldType getZombieBodyShieldType()const;

	/**
	 *获取僵尸头部护盾类型
	 */
	ShieldType getZombieHeadShieldType()const;

	/**
	 * 获取僵尸是否吃大蒜
	 */
	virtual bool getZombieIsEatGarlic() const;

	/**
	 *播放僵尸灰烬动画
	 */
	virtual void playZombiesFillDownAnimation();

	/**
	 *播放僵尸灰烬动画
	 */
	virtual void playZombiesAshesAnimation();

	/**
	 *播放僵尸音效
	 */
	virtual void playZombieSoundEffect();

	/**
	 * 删除操作 
	 */
	virtual void releaseFunction();

protected:
	/*
	 *初始化僵尸
	 */
	virtual void zombieInit(const string& animation_name = "Zombies");

	/**
	 * 设置僵尸监听 
	 */
	virtual void setZombiesListener();

	/**
     *获取僵尸绘制图层
     */
	virtual float getZombieLocalZOrder() const;

	/**
	 *设置节点
	 */
	virtual void setNode(Node* node){ _node = node; }

	/**
	 *设置僵尸一级损伤
	 */
	virtual void setZombiePrimaryInjure();

	/**
	 *设置僵尸二级损伤
	 */
	virtual void setZombieSecondaryInjure();

	/**
	 *僵尸掉胳膊动画
	 */
	virtual void zombieLoseArmAnimation(const std::string& name, const float scale = 1.f);

	/**
	 *僵尸掉头动画
	 */
	virtual void zombieLoseHeadAnimation(const std::string& name, const float scale = 1.5f);

	/**
	 *僵尸掉护盾动画
	 */
	virtual void zombieLoseShieldAnimation(const std::string& name, const float scale = 1.5f);

	/**
	 *僵尸消失动画
	 */
	virtual void zombieFadeOutAnimation();

	/**
	 *僵尸音效
	 */
	virtual void playZombieSoundEffect(const string& name);

	/**
	 *显示僵尸肢体与护盾影子
	 */
	virtual void showZombieShadow(Node* node, const int posy);

	/**
	 *设置僵尸不同关卡的属性
	 */
	virtual void setZombieAttributeForGameType(Node* sprite);

	/**
	 *设置头部护盾一级损伤
	 *参数：更换之前名字
	 *参数：要更换的名字
	 *参数：损伤等级
	 */
	virtual void setZombieHeadShieldPrimaryInjure(const string& oldName, const string& newName);

	/**
	 *设置头部护盾二级损伤
	 *参数：更换之前名字
	 *参数：要更换的名字
	 *参数：损伤等级
	 */
	virtual void setZombieHeadShieldSecondaryInjure(const string& oldName, const string& newName);

	/**
	 *设置头部护盾三级损伤
	 *参数：更换之前名字
	 *参数：要更换的名字
	 *参数：损伤等级
	 */
	virtual void setZombieHeadShieldThirdInjure(const string& oldName, const string& newName);

	/**
	 *设置身体护盾一级损伤
	 *参数：更换之前名字
	 *参数：要更换的名字
	 *参数：损伤等级
	 */
	virtual void setZombieBodyShieldPrimaryInjure(const string& oldName, const string& newName);

	/**
	 *设置身体护盾二级损伤
	 *参数：更换之前名字
	 *参数：要更换的名字
	 *参数：损伤等级
	 */
	virtual void setZombieBodyShieldSecondaryInjure(const string& oldName, const string& newName);

	/**
	 *设置身体护盾三级损伤
	 *参数：更换之前名字
	 *参数：要更换的名字
	 *参数：损伤等级
	 */
	virtual void setZombieBodyShieldThirdInjure(const string& oldName, const string& newName);

CC_CONSTRUCTOR_ACCESS:
	Zombies();
	~Zombies();

private:
	void setSmallZombieAttribute();
	void setBigZombieAttribute();
	void setOpacityZombieAttribute();
	void setZombieGLProgram();
	bool getZombieWarning();

protected:
	int _attackHeadSoundEffectType;           // 攻击头部音效   
	int _attackBodySoundEffectType;           // 攻击身体音效
	int _bodyAnimationId;                     // 身体动画编号
	int _bodyShieldAnimationId;               // 身体护盾动画编号
	int _headShieldAnimationId;               // 头部护盾动画编号
	int _zombieTag;                           // 僵尸编号(暂未使用)
	int _zombieRow;                           // 僵尸所在行
	int _timerTimeSlow;                       // 记录减缓运动时间
	int _timerTimeStop;                       // 记录停止运动时间
	int _isFrozen;                            // 是否被冻住减速（冰莴苣，寒冰射手，冰瓜投手）
	float _timeScale;                         // 播放速度
	float _bloodVolume;                       // 血量
	float _headShieldVolume;                  // 护盾血量
	float _bodyShieldVolume;                  // 护盾1血量
	float _currentBloodVolume;                // 当前血量
	float _currentHeadShieldVolume;           // 当前护盾血量
	float _currentBodyShieldVolume;           // 当前护盾1血量
	float _speed;                             // 存储移动速度
	float _currentSpeed;                      // 移动速度
	bool _isHaveShield;                       // 是否有护盾
	bool _isUseForGameType;                   // 是否被使用在游戏类型
	bool _isEat;                              // 是否在吃植物
	bool _isEatGarlic;                        // 是否吃大蒜
	bool _isShow;                             // 是否显示
	bool _isPreview;                          // 是否是预览模式
	bool _isShowLoseLimbsAnimation;           // 是否显示僵尸掉落肢体动画
	bool _isShowLoseShieldAnimation;          // 是否显示僵尸护盾掉落动画
	bool _isCanDelete[2];                     // 是否可以删除
	bool _isCreateTimer;                      // 是否创建定时器
	bool _isStrikeFly;                        // 是否击飞(离子缘）
	bool _isReserveKill;                      // 是否被预定杀死（大嘴花）
	bool _isCanMove;                          // 是否可以移动
	bool _gameTypeInvalid;                    // 游戏属性是否失效
	string _zombieAnimationName;              // 僵尸动画名字
	Vec2 _position;                           // 位置
	Node* _node;                              // 节点
	Global* _global;                          // 全局变量
	SkeletonAnimation* _zombiesAnimation;     // 僵尸动画
	ShieldType _headShieldType;               // 头部护盾类型
	ShieldType _bodyShieldType;               // 身体护盾类型
	ZombiesType _zombiesType;                 // 僵尸类型
	string _animationName[7];                 // 动画名称
	default_random_engine _random;            // 随机数引擎
	random_device _device;
	static unsigned int _zombiesNumbers;      // 僵尸数量

private:
	int _zombieEatPlantNumber;
	int _zombieHowlNumbers;
	OpenLevelData* _openLevelData;
	GLProgramState* _highLightGLProgramState;
	float _highLightIntensity;                
	bool _highLightFinished;                  // 是否高亮
	bool _redWarning;                         // 是否警告

	static bool _zombieIsWin;
	static int _zombiesNewDieNumbers;
	static GLProgram* _normalGLProgram;
	static GLProgram* _highLightGLProgram;
	static GSGameEndLayer* _gameEndLayer;
	static GSGameResultJudgement* _gameResultJudgement;
};
