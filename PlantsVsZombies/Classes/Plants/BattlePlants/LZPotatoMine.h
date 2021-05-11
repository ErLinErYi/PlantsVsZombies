/**
 *Copyright (c) 2020 LZ.All Right Reserved
 *Author : LZ
 *Date: 2020.2.6
 *Email: 2117610943@qq.com
 */

#pragma once
#include "LZBattlePlants.h"

class PotatoMine :public BattlePlants
{
public:
	static PotatoMine* create(Node* node = nullptr);
	void setBreakGround(const float time);
	float getBreakGround() const;
	Sprite* createPlantImage() override;
	void createPlantAnimation() override;
	void determineRelativePositionPlantsAndZombies() override;

CC_CONSTRUCTOR_ACCESS:
	PotatoMine(Node* node = nullptr);
	~PotatoMine();

private:
	void plantExplode() override;
	void zombieEatPlant(Zombies* zombie) override;
	void cavePlantInformation(rapidjson::Value& object, rapidjson::Document::AllocatorType& allocator) override;
	void readPlantInforamtion(rapidjson::Document* levelDataDocument, char* key, int i) override;
	SkeletonAnimation* showPlantAnimationAndText() override;
	void createListener() override;
	void explodeHurtZombies();
	bool getZombieIsInExplodeRange(Zombies* zombie) override;
	
private:
	bool _isReady;                 // 是否准备好
	bool _isExplodeFinished;       // 是否爆炸完成
	bool _isBeginExplode;          // 是否开始爆炸
	bool _isCanKillZombies;        // 是否可以杀手僵尸
	bool _isKillZombiesFinished;   // 是否杀死僵尸结束
	bool _isReadFile;              // 是否读取文件
	float _breakGround;            // 破土时间
};