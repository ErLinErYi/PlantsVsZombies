#pragma once
#include "cocos2d.h"
#include "spine/spine-cocos2dx.h"
#include "Based/LevelData.h"
#include "Based/GlobalVariable.h"

using namespace spine;
using namespace cocos2d;
using namespace cocos2d::ui;

struct SunNumberRequriement
{
	SunNumberRequriement();
	Text* allSunNumbersText;                     /* 显示阳光数字 */
	int atLeastSunNumbers;                       /* 至少需要的阳光数 */
	int allSunNumbers;                           /* 阳光数 */
	bool isHaveSunNumberRequriement;             /* 是否有阳光要求 */
	bool isUpdateImage;                          /* 图片是否更新 */
};

struct PlantsRequriement
{
	PlantsRequriement();
	Text* palntsNumbersText;                     /* 显示植物数量 */
	int userPlantsNumbers;                       /* 可以使用的植物数量 */
	int surPlusPlantsNumbers;                    /* 剩余植物数量 */
	bool isHavePlantsRequriement;                /* 是否有植物要求 */
	bool isUpdateImage;                          /* 图片是否更新 */
};

enum class GameTypes
{
	None = -1,
	CreateWall,
	AtLeastSunNumbers,
	FlowerPosition,
	CarNumbers,
	UserPlantsNumbers,
	ZombiesInvisible,
	SmallZombies,
	BigZombies,
	NoPlants,
	UserLose
};

class GameType
{
public:
	void createGameType();                                            /* 创建游戏类型 */
	void updateRequirementNumbers(const std::string name);            /* 更新数量 */
	void updateRequirementNumbers();
	void waringPlantsNull();                                          /* 警告植物使用完 */
	SunNumberRequriement* getSunNumberRequriement() const;
	PlantsRequriement* getPlantsRequriement() const;

CC_CONSTRUCTOR_ACCESS:
	GameType(Node* node);
	~GameType();

private:
	void showNumbers(const int& ID);     /* 显示数量 */
	void updateNumbers(const int& ID);   /* 更新数量 */

private:
	Global* _global;
	Node* _node;
	OpenLevelData* _openlevelData;
	SunNumberRequriement* _sunNumberRequriement;
	PlantsRequriement* _plantsRequriement;
};