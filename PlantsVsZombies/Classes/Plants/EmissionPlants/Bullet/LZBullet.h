/**
 *Copyright (c) 2020 LZ.All Right Reserved
 *Author : LZ
 *Date: 2020.2.11
 *Email: 2117610943@qq.com
 */

#pragma once
#include "cocos2d.h"
#include "spine/spine-cocos2dx.h"
#include "json/document.h"
#include "Based/LZGlobalVariable.h"
#include "Scenes/GameScenes/Adventure/GameScene/LZAGSAnimationLayer.h"

using namespace spine;
using namespace cocos2d;

class Zombies;

enum class BulletType
{
	None = 0,
	Pea,
	IcePea,
	FirePea,
	Cabbage,
	AcidLemonBullet,
	CitronBullet,
	StarFruitBullet,
	WaterMelonBullet,
	WinterMelonBullet,
	CatTailBullet,
	FumeShroomBullet,
	ScaredyShroomBullet
};

enum class SoundEffectType
{
	kernelpult = 0,
	plastichit,
	shieldhit,
	paper,
	Brick,
	firepea,
	watermelon
};

class Bullet
{
public:
	/**
	 *创建子弹
	 */
	virtual void createBullet() = 0;

	/**
	 *子弹与僵尸碰撞检测
	 */
	virtual void bulletAndZombiesCollision() = 0;

	/**
	 * 存储子弹特有信息
	 * 子类中实现
	 */
	virtual void caveBulletInformation(rapidjson::Value& object, rapidjson::Document::AllocatorType& allocator) {};

	/**
	 * 读取子弹特有信息 
	 * 子类中实现
	 */
	virtual void readBulletInformation(rapidjson::Document* levelDataDocument, char* key, int i) {};
	virtual void readBulletAnimationInformation(rapidjson::Document* levelDataDocument, char* key, int i) {};

	/**
	 *获取子弹动画
	 */
	virtual SkeletonAnimation* getBullet() const;

	/**
	 *设置子弹的位置
	 */
	virtual void setBulletPosition(const Vec2& position);
	
	/**
	 * 设置子弹所在行  
	 */
	virtual void setBulletInRow(const int row);

	/**
	 *设置子弹的名字
	 */
	virtual void setBulletName(const string& name);

	/**
	 *设置子弹是否被使用
	 */
	virtual void setBulletIsUsed(const bool isUsed);

	/**
	 *设置子弹攻击力
	 */
	virtual void setBulletAttack(const float attack);

	/**
	 *设置子弹透明度
	 */
	virtual void setBulletOpacity(GLubyte opacity);
	virtual void setBulletOpacity();

	/**
	 *获取子弹是否被使用
	 */
	virtual bool getBulletIsUsed() const;

	/**
	 *获取子弹攻击力
	 */
	virtual int getBulletAttack() const;

	/**
	 *获取子弹位置
	 */
	virtual Vec2 getBulletPosition() const;

	/**
	 * 获取子弹初始位置 
	 */
	virtual Vec2 getBulletInitPosition() const;

	/**
	 *获取子所在行 
	 */
	virtual int getBulletInRow() const;

	/**
	 *获取子弹X位置
	 */
	virtual float getBulletPositionX() const;

	/**
	 *获取子弹Y位置
	 */
	virtual float getBulletPositionY() const;

	/**
	 *获取子弹类型
	 */
	virtual BulletType getBulletType() const;

	/**
	 *获取子弹是否与僵尸在同一行
	 */
	virtual bool getBulletIsSameLineWithZombie(Zombies* zombie);

	/**
	 *获取子弹是否与僵尸相遇
	 */
	virtual bool getBulletIsEncounterWithZombie(Zombies* zombie);

	/*
	 *获取子弹的名字
	 */
	virtual string& getBulletName();

	/**
	 *获取子弹是否显示
	 */
	virtual bool getBulletVisible() const;

	/**
	 *子弹攻击僵尸
	 */
	virtual void bulletAttackHurtZombies(Zombies* zombie);

	/**
	 * 子弹离开地图设置不可见
	 */
	virtual void bulletOutMapSetInvisible();

	/**
	 *静态方法删除子弹
	 */
	static void bulletDeleteUpdate(list<Bullet*>::iterator& bullet);

	/**
	 *播放子弹碰撞声音
	 */
	static void playSoundEffect(SoundEffectType soundEffect);

	/**
	 *选择播放音效
	 */
	static void selectSoundEffect(const int body, const int head);
	static void selectSoundEffect(const int head);

	virtual void releaseFunction();

CC_CONSTRUCTOR_ACCESS:
	Bullet();
	~Bullet();

protected:
	virtual SkeletonAnimation* bulletInit(const std::string& plantname, const std::string& animaionname);
	virtual void createShadow() {};
	virtual void createShadow(Vec2 position) {};
	virtual void createShadow(float scale) {};
	virtual float getZOrder() const;

protected:
	SkeletonAnimation* _bulletAnimation;  // 动画
	float _attack;                        // 攻击力
	bool _isUsed;                         // 是否使用
	bool _animationLoop;                  // 动画循环播放
	int _bulletRow;                       // 所在行
	int _zombieInExplodeRangeNumbers;     // 溅射伤害僵尸数
	Node* _node;                          // 父节点
	Vec2 _position;                       // 位置
	string _bulletName;                   // 豌豆名字
	BulletType _bulletType;               // 子弹类型
	Global* _global;                      // 全局单例
};