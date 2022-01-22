/**
 *Copyright (c) 2021 LZ.All Right Reserved
 *Author : LZ
 *Date: 2021.7.02
 *Email: 2117610943@qq.com
 */

#pragma once
#include "Scenes/GameScenes/Adventure/GameScene/LZAGSControlLayer.h"
#include "LZIButtonLayer.h"

using namespace cocos2d;

class Zombies;
class IControlLayer :public GSControlLayer
{
public:
	CREATE_FUNC(IControlLayer);
	void addLayer(Node* node, const int order, const string& name) { node->addChild(this, order, name); }
	static void beginNewGame();
	static void onSetMostLevel(unsigned int level);
	static unsigned int onGetMostLevel();
	static void onSetCurrentLevel(unsigned int level);
	static unsigned int onGetCurrentLevel();

CC_CONSTRUCTOR_ACCESS:
	IControlLayer();
	~IControlLayer();
	virtual bool init() override;

protected:
	virtual void initData() override;
	virtual void createSchedule() override;
	virtual void createMouseListener() override;
	virtual void createZombies() override;
	virtual void judgeLevelIsFinished() override;
	virtual void controlCardEnabled() override;
	virtual void mouseMoveControl() override;
	virtual void mouseDownControl(EventMouse* eventmouse = nullptr) override;
	virtual void mouseRightButtonDownControl() override;
	virtual void mouseLeftButtonDownControl() override;
	virtual bool judgeMousePositionIsInMap() override;
	virtual void createZombiesCardListener();
	virtual void selectZombiesPreviewImage();
	virtual void createPreviewZombie();
	virtual void judgeZombiesFadeOut();
	virtual void autoCreatePlants(PlantsType type, Vec2 vec2);
	virtual void createSelectPlantsType(PlantsType *type, int plantsNumber, uniform_int_distribution<int> &rand_engine);
	virtual void selectPlantsType();
	virtual void calculatePlantsNumbers();

private:
	void showSelectedButtonHoverEffect();
	void judgeUserLose();
	void showBlackFadeOutAnimation();
	void coinRecovery(const Vec2& position, const int id);
	
protected:
	IButtonLayer::ZombiesCardButton _selectZombiesCard;                  // 所选僵尸
	default_random_engine _random;                                       // 随机数引擎
	random_device _device;
	bool _levelFinished;
	int _sunFlowerNumbers;
	int _totalNumber;
	int _pultPlantNumbers;
	int _pultPlantRange;
	int _otherPlantRange;
	static unsigned int currentLevelNumber;
	static unsigned int mostLevelNumber;
	static unsigned int _encryptMKey;
	static unsigned int _encryptCKey;
};