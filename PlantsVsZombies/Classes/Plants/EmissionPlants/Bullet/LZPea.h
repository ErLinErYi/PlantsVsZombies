/**
 *Copyright (c) 2020 LZ.All Right Reserved
 *Author : LZ
 *Date: 2020.2.18
 *Email: 2117610943@qq.com
 */

#pragma once
#include "LZBullet.h"

class Pea :public Bullet
{
public:
	enum class PeaDirectionType
	{
		Up,         // 上方
		Normal,     // 正常
		Down        // 下方
	};

public:
	/**
	 *创建豌豆
	 */
	virtual void createBullet() override;

	/**
	 *设置火炬树桩编号
	 */
	virtual void setTorchwoodTag(const int torchwoodTag);

	/**
	 * 计算豌豆偏移方向计算 
	 */
	virtual void calculateDirectionDistance(Vec2 initPosition, Vec2 position);

	/**
	 * 设置豌豆方向类型
	 */
	virtual void setPeaDirectionType(PeaDirectionType type);

	/**
	 * 获取豌豆方向类型
	 */
	virtual PeaDirectionType getPeaDirectionType();

	/**
	 *获取火炬树桩编号
	 */
	virtual int getTorchwoodTag() const;

protected:
	/**
	 *豌豆与僵尸碰撞和检测
	 */
	virtual void bulletAndZombiesCollision() override;

	/**
	 * 攻击僵尸 
	 */
	virtual void attackZombies(Zombies* zombie);
	
	/**
	 *  僵尸是否在溅射范围判断
	 */
	virtual bool getZombieInExplodeRange(Zombies* zombie);

	/**
	 *创建豌豆爆炸动画
	 */
	virtual void  createPeaExplode();

	/**
	 *创建爆炸效果 
	 */
	virtual void createExplodeEffect();

	/**
	 *创建子弹影子
	 */
	virtual void createShadow(Vec2 position = Vec2::ZERO) override;

	/**
	 * 设置移动方式
	 */
	virtual void setBulletAction();

	/**
	 * 存储子弹特有信息
	 */
	virtual void caveBulletInformation(rapidjson::Value& object, rapidjson::Document::AllocatorType& allocator) override;

	/**
	 * 读取子弹特有信息
	 */
	virtual void readBulletInformation(rapidjson::Document* levelDataDocument, char* key, int i) override;
	virtual void readBulletAnimationInformation(rapidjson::Document* levelDataDocument, char* key, int i) override;

CC_CONSTRUCTOR_ACCESS:
	Pea(Node* node = nullptr);
	~Pea();

protected:
	int _torchwoodTag;                    // 火炬树桩标记
	float _actionTime;                    // 动作时间
	Vec2 _initPostion[2];                 // 初始时间
	string _peaAnimationName;             // 豌豆动画名称
	PeaDirectionType _peaDirectionType;   // 豌豆方向类型
};
