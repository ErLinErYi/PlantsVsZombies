/**
 *Copyright (c) 2020 LZ.All Right Reserved
 *Author : LZ
 *Date: 2020.2.5
 *Email: 2117610943@qq.com
 */

#include "LZPlants.h"

#include "Zombies/LZZombies.h"
#include "Scenes/GameScenes/Adventure/GameScene/LZAGSData.h"
#include "Scenes/GameScenes/Adventure/GameScene/LZAGSBackgroundLayer.h"
#include "Based/LZPlayMusic.h"

int Plants::plantNumber = -1;
GLProgram* Plants::_normalGLProgram = nullptr;
GLProgram* Plants::_highLightGLProgram = nullptr;

#ifdef _DEBUG
PlantsInformation::PlantsCardInformation plantsCardInformation[] =
{
	{"SunFlower",              "PlantsIcon4",     "B",    3,    0,          0,         0,      0, 0,     false,   PlantsType::SunFlower,           Color3B::YELLOW                         },  /* 向日葵 */
	{"PeaShooter",             "PlantsIcon5",     "C",    3,    1,          0,         0,      0, 0,     false,   PlantsType::PeaShooter,          Color3B::GREEN                          },  /* 豌豆射手 */
	{"Wallnut_body",           "PlantsIcon6",     "C",    3,    2,          0,         0,      0, 0,     false,   PlantsType::WallNut,             Color3B(140,80,10)                      },  /* 坚果墙 */
	{"CherryBomb",             "PlantsIcon7",     "A",    2,    3,          0,         0,      0, 0,     false,   PlantsType::CherryBomb,          Color3B::RED                            },  /* 樱桃炸弹 */
	{"PotatoMine",             "PlantsIcon7",     "B",    3,    4,          0,         0,      0, 0,     false,   PlantsType::PotatoMine,          Color3B(250,75,10)                      },  /* 土豆雷 */
	{"Cabbage",                "PlantsIcon3",     "B",    3,    5,          0,         0,      0, 0,     false,   PlantsType::CabbagePult,         Color3B(23,79,50)                       },  /* 卷心菜投手 */
	{"Torchwood",              "PlantsIcon2",     "C",    3,    6,          0,         0,      0, 0,     false,   PlantsType::Torchwood,           Color3B::RED                            },  /* 火炬树桩 */
	{"Spikeweed",              "PlantsIcon12",    "B",    3,    7,          0,         0,      0, 0,     false,   PlantsType::Spikeweed,           Color3B(0,64,0)                         },  /* 地刺 */
	{"Garlic",                 "PlantsIcon10",    "B",    3,    8,          0,         0,      0, 0,     false,   PlantsType::Garlic,              Color3B(218,205,182)                    },  /* 大蒜 */
	{"ScaredyShroom",          "PlantsIcon14",    "B",    3,    9,          0,         0,      0, 0,     false,   PlantsType::ScaredyShroom,       Color3B(163,73,164),  { 200,   200   }  },  /* 胆小菇 */
	{"IceBergLettuce",         "PlantsIcon15",    "B",    3,    10,         0,         0,      0, 0,     false,   PlantsType::IceBergLettuce,      Color3B(99,202,178),  { 500,   500   }  },  /* 冰莴苣 */
	{"Marigold",               "PlantsIcon9",     "B",    3,    11,         0,         0,      0, 0,     false,   PlantsType::Marigold,            Color3B::ORANGE,      { 1000,  1000  }  },  /* 金盏花 */
	{"Imitater",               "PlantsIcon9",     "B",    3,    12,         0,         0,      0, 0,     false,   PlantsType::Imitater,            Color3B::ORANGE,      { 2000,  2000  }  },  /* 模仿者 */
	{"Chomper",                "PlantsIcon8",     "B",    3,    13,         0,         0,      0, 0,     false,   PlantsType::Chomper,             Color3B(152,66,184),  { 3000,  3000  }  },  /* 大嘴花 */
	{"Squash",                 "PlantsIcon8",     "B",    3,    14,         0,         0,      0, 0,     false,   PlantsType::Squash,              Color3B(181,230,29),  { 4000,  4000  }  },  /* 倭瓜 */
	{"IcePeaShooter",          "PlantsIcon15",    "A",    2,    15,         0,         0,      0, 0,     false,   PlantsType::IcePeaShooter,       Color3B(47,202,207),  { 5500,  5500  }  },  /* 寒冰豌豆射手 */
	{"FirePeaShooter",         "PlantsIcon2",     "A",    2,    16,         0,         0,      0, 0,     false,   PlantsType::FirePeaShooter,      Color3B::RED,         { 7000,  7000  }  },  /* 火焰豌豆射手 */
	{"LemonShooter",           "PlantsIcon10",    "A",    2,    17,         0,         0,      0, 0,     false,   PlantsType::AcidLemonShooter,    Color3B(232,199,23),  { 10000, 10000 }  },  /* 柠檬射手 */
	{"Blover",                 "PlantsIcon11",    "B",    3,    18,         0,         0,      0, 0,     false,   PlantsType::Blover,              Color3B::GREEN,       { 12000, 12000 }  },  /* 三叶草 */
	{"SunFlowerTwin",          "PlantsIcon4",     "A",    2,    19,         0,         0,      0, 0,     false,   PlantsType::SunFlowerTwin,       Color3B::YELLOW,      { 14000, 14000 }  },  /* 双胞向日葵 */
	{"WaterMelonPult",         "PlantsIcon3",     "A",    2,    20,         0,         0,      0, 0,     false,   PlantsType::WaterMelonPult,      Color3B(23,79,50),    { 18000, 18000 }  },  /* 西瓜投手 */
	{"Pumpkin",                "PlantsIcon6",     "A",    2,    21,         0,         0,      0, 0,     false,   PlantsType::Pumpkin,             Color3B::ORANGE,      { 23000, 23000 }  },  /* 南瓜头 */
    {"Jalapeno",               "PlantsIcon2",     "A",    2,    22,         0,         0,      0, 0,     false,   PlantsType::Jalapeno,            Color3B::RED,         { 28000, 28000 }  },  /* 火爆辣椒 */
	{"MagnetShroom",           "PlantsIcon",      "A",    2,    23,         0,         0,      0, 0,     false,   PlantsType::MagnetShroom,        Color3B(64,0,64),     { 33000, 33000 }  },  /* 磁力菇 */
	{"JalapenoVariation",      "PlantsIcon2",     "S",    1,    24,         0,         0,      0, 0,     true,    PlantsType::JalapenoVariation,   Color3B(100,0,0),     { 38000, 38000 }  },  /* 火爆辣椒变异 */
	{"FumeShroom",             "PlantsIcon11",    "S",    1,    25,         0,         0,      0, 0,     false,   PlantsType::FumeShroom,          Color3B(163,73,164),  { 42000, 42000 }  },  /* 大喷菇 */
	{"ThreePeaShooter",        "PlantsIcon5",     "S",    1,    26,         0,         0,      0, 0,     false,   PlantsType::ThreePeaShooter,     Color3B::GREEN,       { 46000, 46000 }  },  /* 三头豌豆射手 */
	{"StarFruit",              "PlantsIcon9",     "S",    1,    27,         0,         0,      0, 0,     false,   PlantsType::StarFruit,           Color3B::ORANGE,      { 50000, 50000 }  },  /* 杨桃 */
	{"WinterMelonPult",        "PlantsIcon15",    "SSS",  1,    28,         0,         0,      0, 0,     false,   PlantsType::WinterMelonPult,     Color3B(47,202,207),  { 57000, 57000 }  },  /* 冰瓜投手 */
	{"Citron",                 "PlantsIcon",      "SS",   1,    29,         0,         0,      0, 0,     false,   PlantsType::Citron,              Color3B(0,255,255),   { 64000, 64000 }  },  /* 离子缘 */
	{"CatTail",                "PlantsIcon11",    "SSS",  1,    30,         0,         0,      0, 0,     false,   PlantsType::CatTail,             Color3B::MAGENTA,     { 72000, 72000 }  },  /* 香蒲 */
	{"GloomShroom",            "PlantsIcon13",    "SSS",  1,    31,         0,         0,      0, 0,     false,   PlantsType::GloomShroom,         Color3B(163,73,164),  { 82000, 82000 }  },  /* 忧郁菇 */
	/* 植物图片名称 */         /* 标志名称 */    /*品质*/    /*优先级*/  /*阳光数*/  /*时间*/           /*反转*/       /*植物类型*/                   /* 颜色 */            /* 要求 */
};
#else
PlantsInformation::PlantsCardInformation plantsCardInformation[] =
{
	{"SunFlower",             "PlantsIcon4",    "B",    3,   0,          50,          7.5f,      0, 0,     false,   PlantsType::SunFlower,           Color3B::YELLOW                         },  /* 向日葵 */
	{"PeaShooter",            "PlantsIcon5",    "C",    3,   1,          100,         7.5f,      0, 0,     false,   PlantsType::PeaShooter,          Color3B::GREEN                          },  /* 豌豆射手 */
	{"Wallnut_body",          "PlantsIcon6",    "C",    3,   2,          50,          30,        0, 0,     false,   PlantsType::WallNut,             Color3B(140,80,10)                      },  /* 坚果墙 */
	{"CherryBomb",            "PlantsIcon7",    "A",    2,   3,          150,         37.5f,     0, 0,     false,   PlantsType::CherryBomb,          Color3B::RED                            },  /* 樱桃炸弹 */
	{"PotatoMine",            "PlantsIcon7",    "C",    3,   4,          25,          30,        0, 0,     false,   PlantsType::PotatoMine,          Color3B(250,75,10)                      },  /* 土豆雷 */
	{"Cabbage",               "PlantsIcon3",    "B",    3,   5,          100,         7.5f,      0, 0,     false,   PlantsType::CabbagePult,         Color3B(23,79,50)                       },  /* 卷心菜投手 */
	{"Torchwood",             "PlantsIcon2",    "C",    3,   6,          175,         7.5f,      0, 0,     false,   PlantsType::Torchwood,           Color3B::RED                            },  /* 火炬树桩 */
	{"Spikeweed",             "PlantsIcon12",   "B",    3,   7,          100,         7.5f,      0, 0,     false,   PlantsType::Spikeweed,           Color3B(0,64,0)                         },  /* 地刺 */
	{"Garlic",                "PlantsIcon10",   "B",    3,   8,          50,          10,        0, 0,     false,   PlantsType::Garlic,              Color3B(218,205,182)                    },  /* 大蒜 */
	{"ScaredyShroom",         "PlantsIcon13",   "B",    3,   9,          75,          7.5f,      0, 0,     false,   PlantsType::ScaredyShroom,       Color3B(163,73,164),  { 200,   200   }  },  /* 胆小菇 */
	{"IceBergLettuce",        "PlantsIcon15",   "B",    3,   10,         0,           20,        0, 0,     false,   PlantsType::IceBergLettuce,      Color3B(99,202,178),  { 500,   500   }  },  /* 冰莴苣 */
	{"Marigold",              "PlantsIcon9",    "B",    3,   11,         50,          10,        0, 0,     false,   PlantsType::Marigold,            Color3B::ORANGE,      { 1000,  1000  }  },  /* 金盏花 */
	{"Imitater",              "PlantsIcon9",    "B",    3,   12,         0,           0,         0, 0,     false,   PlantsType::Imitater,            Color3B::ORANGE,      { 2000,  2000  }  },  /* 模仿者 */
	{"Chomper",               "PlantsIcon8",    "B",    3,   13,         150,         7.5f,      0, 0,     false,   PlantsType::Chomper,             Color3B(152,66,184),  { 3000,  3000  }  },  /* 大嘴花 */
	{"Squash",                "PlantsIcon8",    "B",    3,   14,         50,          30,        0, 0,     false,   PlantsType::Squash,              Color3B(152,66,184),  { 4000,  4000  }  },  /* 倭瓜 */
	{"IcePeaShooter",         "PlantsIcon15",   "B",    2,   15,         175,         10,        0, 0,     false,   PlantsType::IcePeaShooter,       Color3B(47,202,207),  { 5500,  5500  }  },  /* 寒冰豌豆射手 */
	{"FirePeaShooter",        "PlantsIcon2",    "B",    2,   16,         200,         10,        0, 0,     false,   PlantsType::FirePeaShooter,      Color3B::RED,         { 7000,  7000  }  },  /* 火焰豌豆射手 */
	{"LemonShooter",          "PlantsIcon10",   "A",    2,   17,         175,         7.5f,      0, 0,     false,   PlantsType::AcidLemonShooter,    Color3B(232,199,23),  { 10000, 10000 }  },  /* 柠檬射手 */
	{"Blover",                "PlantsIcon11",   "B",    3,   18,         50,          7.5f,      0, 0,     false,   PlantsType::Blover,              Color3B::GREEN,       { 12000, 12000 }  },  /* 三叶草 */
	{"SunFlowerTwin",         "PlantsIcon4",    "A",    2,   19,         100,         15,        0, 0,     false,   PlantsType::SunFlowerTwin,       Color3B::YELLOW,      { 14000, 14000 }  },  /* 双胞向日葵 */
	{"WaterMelonPult",        "PlantsIcon3",    "A",    2,   20,         225,         10,        0, 0,     false,   PlantsType::WaterMelonPult,      Color3B(23,79,50),    { 18000, 18000 }  },  /* 西瓜投手 */
	{"Pumpkin",               "PlantsIcon6",    "A",    2,   21,         150,         30,        0, 0,     false,   PlantsType::Pumpkin,             Color3B::ORANGE,      { 23000, 23000 }  },  /* 南瓜头 */
	{"Jalapeno",              "PlantsIcon2",    "A",    2,   22,         150,         37.5f,     0, 0,     false,   PlantsType::Jalapeno,            Color3B::RED,         { 28000, 28000 }  },  /* 火爆辣椒 */
	{"MagnetShroom",          "PlantsIcon",     "A",    2,   23,         100,         10,        0, 0,     false,   PlantsType::MagnetShroom,        Color3B(64,0,64),     { 33000, 33000 }  },  /* 磁力菇 */
	{"JalapenoVariation",     "PlantsIcon2",    "S",    1,   24,         175,         37.5f,     0, 0,     true,    PlantsType::JalapenoVariation,   Color3B(100,0,0),     { 38000, 38000 }  },  /* 火爆辣椒变异 */
	{"FumeShroom",            "PlantsIcon13",   "S",    1,   25,         150,         7.5f,      0, 0,     false,   PlantsType::FumeShroom,          Color3B(163,73,164),  { 42000, 42000 }  },  /* 大喷菇 */
	{"ThreePeaShooter",       "PlantsIcon5",    "S",    1,   26,         225,         7.5f,      0, 0,     false,   PlantsType::ThreePeaShooter,     Color3B::GREEN,       { 46000, 46000 }  },  /* 三头豌豆射手 */
	{"StarFruit",             "PlantsIcon9",    "S",    1,   27,         125,         7.5f,      0, 0,     false,   PlantsType::StarFruit,           Color3B::ORANGE,      { 50000, 50000 }  },  /* 杨桃 */
	{"WinterMelonPult",       "PlantsIcon15",   "SSS",  1,   28,         300,         20,        0, 0,     false,   PlantsType::WinterMelonPult,     Color3B(47,202,207),  { 57000, 57000 }  },  /* 冰瓜投手 */
	{"Citron",                "PlantsIcon",     "SS",   1,   29,         275,         15,        0, 0,     false,   PlantsType::Citron,              Color3B(0,255,255),   { 64000, 64000 }  },  /* 离子缘 */
	{"CatTail",               "PlantsIcon11",   "SSS",  1,   30,         225,         10,        0, 0,     false,   PlantsType::CatTail,             Color3B::MAGENTA,     { 72000, 72000 }  },  /* 香蒲 */
	{"GloomShroom",           "PlantsIcon13",   "SSS",  1,   31,         275,         40,        0, 0,     false,   PlantsType::GloomShroom,         Color3B(163,73,164),  { 82000, 82000 }  },  /* 忧郁菇 */
	/* 植物图片名称 */       /* 标志名称 */    /*品质*/   /*优先级*/  /*阳光数*/   /*时间*/              /*反转*/       /*植物类型*/                   /* 颜色 */            /* 要求 */
};
#endif // _DEBUG

/*添加植物需要修改的函数
 * GameScene->AnimationLayer:需要修改：
 * 1. createDifferentPlants(PlantsType plantsType)
 *
 * plants->plants需要修改：
 * 1. 添加plantsType
 */

Plants::Plants(Node* node, const Vec2& position) :
	_node(node)
,	_position(position)
,	_plantShadow(nullptr)
,	_plantImage(nullptr)
,	_plantAnimation(nullptr)
,	_soilSplashAnimation(nullptr)
,   _highLightGLProgramState(nullptr)
,	_sunNeed(NOINITIALIZATION)
,	_coolDownTime(NOINITIALIZATION)
,	_costGold(NOINITIALIZATION)
,	_costMasonry(NOINITIALIZATION)
,	_healthPoint(NOINITIALIZATION)
,	_plantTag(NOINITIALIZATION)
,	_isLoop(true)
,   _isCanDelete{ false,false }
,   _zOrder(0)
,   _highLightIntensity(0.6f)
,   _plantNumber(++plantNumber)
,   _combatEffecttiveness(0)
,   _plantsType(PlantsType::None)
,	_global(Global::getInstance())
{
}

Plants::~Plants()
{
}

SkeletonAnimation* Plants::plantInit(const std::string& plantname, const std::string& animaionname)
{
	auto iter = _global->userInformation->getAnimationData().find(plantname);
	if (iter != _global->userInformation->getAnimationData().end())/* 如果可以找到 */
	{
		_plantAnimation = SkeletonAnimation::createWithData(iter->second);
		_plantAnimation->setPosition(_position);
		_plantAnimation->setAnimation(0, animaionname, _isLoop);
		_plantAnimation->setLocalZOrder(_zOrder);
		_plantAnimation->setTag(_plantTag);
		_plantAnimation->setTimeScale(0.7f + rand() % 11 / 100.f);
		_plantAnimation->update(0);
		setPlantGLProgram();
		return _plantAnimation;
	}
	return nullptr;
}

void Plants::imageInit(const std::string& name, const Vec2& position)
{
	_plantImage = Sprite::createWithSpriteFrameName(name + ".png");
	_plantImage->setPosition(position);
	_plantImage->setName("Preview");
	_plantImage->setOpacity(150);
	_node->addChild(_plantImage, 99);
}

void Plants::determineRelativePositionPlantsAndZombies()
{
	for (auto zombie : ZombiesGroup)
	{
		zombieEatPlant(zombie);      /* 僵尸吃植物 */

		zombieRecoveryMove(zombie);  /* 僵尸恢复移动 */
	}
}

void Plants::stopPlantsAllAction()
{
	for (auto& sunFlower : PlantsGroup)
	{
		sunFlower.second->getPlantAnimation()->stopAllActions();
	}
}

void Plants::setPlantSoilSplashAnimation(const float& scale)
{
	/* 创建种植泥土飞溅动画 */
	auto iter = _global->userInformation->getAnimationData().find("SplashOfSoil");
	if (iter != _global->userInformation->getAnimationData().end())
	{
		_soilSplashAnimation = SkeletonAnimation::createWithData(iter->second);
		_soilSplashAnimation->setPosition(Vec2(0, -50));
		_soilSplashAnimation->setAnimation(0, "SplashOfSoil", false);
		_soilSplashAnimation->setScale(scale);
		_soilSplashAnimation->setName("SplashOfSoil");
		_soilSplashAnimation->setGlobalZOrder(_plantAnimation->getGlobalZOrder());
		_plantAnimation->addChild(_soilSplashAnimation);

		_soilSplashAnimation->runAction(Sequence::create(DelayTime::create(1.f), 
			CallFunc::create([&]() 
				{
					_soilSplashAnimation->removeFromParent();
				}), nullptr));
	}

	iter = _global->userInformation->getAnimationData().find("BufEffect");
	if (iter != _global->userInformation->getAnimationData().end())
	{
		auto bufEffect = SkeletonAnimation::createWithData(iter->second);
		bufEffect->setAnimation(0, "animation", false);
		bufEffect->setPosition(Vec2(0, -50));
		bufEffect->setScale(scale);
		bufEffect->setName("BufEffect");
		bufEffect->update(0);
		_plantAnimation->addChild(bufEffect);

		bufEffect->runAction(Sequence::create(FadeOut::create(0.5f),DelayTime::create(0.5f),
			CallFunc::create([=]()
				{
					bufEffect->removeFromParent();
				}), nullptr));
	}
}

void Plants::setPlantShadow(const float& scale)
{
	/* 创建植物影子 */
	_plantShadow = Sprite::createWithSpriteFrameName("plantshadow.png");
	_plantShadow->setScale(scale);
	_plantShadow->setPosition(Vec2(-5, 15));
	_plantShadow->setName("plantshadow");
	_plantAnimation->addChild(_plantShadow, -1);
}

void Plants::setPlantHurtBlink()
{
	auto action = Repeat::create(Sequence::create(
		CallFunc::create([this]()
			{
				_highLightIntensity -= 0.01f;
				_highLightGLProgramState->setUniformFloat("intensity", _highLightIntensity);
			}), DelayTime::create(0.005f), nullptr), 50);

	if (_plantAnimation)
	{
		_plantAnimation->runAction(Sequence::create(
			CallFunc::create([this]()
				{
					_plantAnimation->setGLProgram(_highLightGLProgram);
					_highLightGLProgramState = _plantAnimation->getGLProgramState();
					_highLightGLProgramState->setUniformFloat("intensity", 0.6f);
				}), DelayTime::create(0.15f),
			CallFunc::create([this]()
				{
					_plantAnimation->setGLProgram(_normalGLProgram);
					_highLightIntensity = 0.6f;
				}), nullptr));
	}
}

void Plants::setPlantHurtBlink(PlantsType type) const
{
	_plantAnimation->runAction(Sequence::create(TintTo::create(0.15f, Color3B(70, 70, 70)), TintTo::create(0.15f, Color3B::WHITE), nullptr));
}

void Plants::setPlantVisible(const bool visible)
{
	_plantAnimation->setVisible(visible);
}

void Plants::setPlantScale()
{
	_plantAnimation->setScale(_plantAnimation->getScale() + (4 - _plantTag / 10) / 20.f);
}

void Plants::setPlantRemoveFromMap()
{
	setPlantHealthPoint(0);
	setPlantVisible(false);
	_plantAnimation->setTimeScale(0);
	_plantAnimation->stopAllActions();
}

bool Plants::getPlantIsSurvive() const
{
	if (_healthPoint > 0 && _plantAnimation->isVisible())
		return true;
	return false;
}

bool Plants::getPlantVisible()
{
	return _plantAnimation->isVisible();
}

bool* Plants::getPlantIsCanDelete()
{
	return _isCanDelete;
}

bool Plants::getZombieIsTheFrontOfPlant(Zombies* zombie)
{
	return zombie->getZombiePositionX() >= _plantAnimation->getPositionX() ? true : false;
}

bool Plants::getZombieIsSameColumnWithPlant(Zombies* zombie)
{
	return fabs(_plantAnimation->getPositionX() - zombie->getZombieAnimation()->getPositionX()) < 70 ? true : false;;
}

bool Plants::getZombieIsSameLineWithPlant(Zombies* zombie)
{
	return zombie->getZombieInRow() == getPlantRow() ? true : false;
}

bool Plants::getZombieIsEncounterPlant(Zombies* zombie)
{
	return fabs(zombie->getZombiePositionX() - _plantAnimation->getPositionX()) <= 70 ? true : false;
}

void Plants::zombieEatPlant(Zombies* zombie)
{
	if (getPlantIsSurvive() && Plants::getZombieIsSameLineWithPlant(zombie) &&                  /* 植物存活 && 植物与僵尸在同一行 */
		Plants::getZombieIsEncounterPlant(zombie) && zombie->getZombieIsEatPlants())            /* 僵尸遇到植物 && 僵尸是吃植物的僵尸 */
	{
		setZombieEatPlantControl(zombie);
	}
}

void Plants::setZombieEatPlantControl(Zombies* zombie)
{
	if (zombie->getZombieIsSurvive() && !zombie->getZombieIsEat() && zombie->getZombieIsFrozen() != 2)
	{
		zombie->setZombieEatPlantNumber(_plantNumber);
		zombie->setZombieStop();
		zombie->setZombieIsEat(true);

		if (zombie->getZombieType() == ZombiesType::GargantuarZombies)
		{
			zombieAttackPlant(zombie);
		}
		else
		{
			const string eateffect[3] = { "chomp","chomp2","chompsoft" };
			auto track = zombie->getZombieAnimation()->setAnimation(0,
				zombie->getZombieType() == ZombiesType::LmpZombies ? "Zombies_Eat" : rand() % 4 ? "Zombies_Eat" : "Zombies_Eat1", true);
			zombie->getZombieAnimation()->setTrackEventListener(track, [=](spTrackEntry* entry, spEvent* event)
				{
					if (!strcmp(event->data->name, "eat") && zombie->getZombieIsSurvive())
					{
						if (event->intValue == 1)
						{
							PlayMusic::playMusic(eateffect[rand() % 3]);
							reducePlantHealthPoint(100 - 30 * zombie->getZombieCurrentBloodProportionBloodPrecent());
							setPlantHurtBlink();
						}
					}
				});
		}
	}
}

void Plants::zombieAttackPlant(Zombies* zombie)
{
	auto track = zombie->getZombieAnimation()->setAnimation(0, "Zombies_Attack", true);
	zombie->getZombieAnimation()->setTrackEventListener(track,[=](spTrackEntry* entry, spEvent* event)
		{
			if (!strcmp(event->data->name, "attack") && zombie->getZombieIsSurvive())
			{
				PlayMusic::playMusic("gargantuar_thump");
				GSBackgroundLayer::backgroundRunAction();
				setPlantHealthPoint(0);
			}
		});
}

void Plants::zombieRecoveryMove(Zombies* zombie)
{
	if (!getPlantIsSurvive() && zombie->getZombieEatPlantNumber() == _plantNumber &&  /* 植物死亡 && 僵尸是吃的该植物 */
		zombie->getZombieIsEat() && zombie->getZombieIsStop()) /* 僵尸正在吃植物 && 僵尸正在停止移动 */
	{
		setPlantRemoveFromMap();

		if (getPlantType() != PlantsType::PotatoMine && getPlantType() != PlantsType::CherryBomb &&
			getPlantType() != PlantsType::Jalapeno && getPlantType() != PlantsType::JalapenoVariation)
		{
			PlayMusic::playMusic("gulp");
		}

		zombie->setZombieIsEat(false);
		if (!zombie->getZombieIsPlayDieAnimation()) /* 僵尸没有播放死亡动画 */
		{
			zombie->getZombieAnimation()->setTrackEventListener(zombie->getZombieAnimation()->getCurrent(), nullptr);
			if (zombie->getZombieType() == ZombiesType::GargantuarZombies)
			{
				zombie->getZombieAnimation()->addAnimation(0, "Zombies_Walk", true);
			}
			else
			{
				zombie->getZombieAnimation()->setMix("Zombies_Eat", Zombies::getZombieAniamtionName(zombie->getZombieType()), 0.5f);
				zombie->getZombieAnimation()->setAnimation(0, Zombies::getZombieAniamtionName(zombie->getZombieType()), true);
			}

			if (zombie->getZombieIsFrozen() == 0)
			{
				zombie->setZombieCurrentSpeed(zombie->getZombieSpeed());
			}
			else if (zombie->getZombieIsFrozen() == 1)
			{
				zombie->setZombieCurrentSpeed(zombie->getZombieSpeed() / 2.f);
			}
		}
	}
}

void Plants::setPlantGLProgram()
{
	if (!_normalGLProgram || !_highLightGLProgram)
	{
		_normalGLProgram = _plantAnimation->getGLProgram();
		_highLightGLProgram = Zombies::getHighLight();
	}
}
