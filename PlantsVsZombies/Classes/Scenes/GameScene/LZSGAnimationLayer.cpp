/**
 *Copyright (c) 2020 LZ.All Right Reserved
 *Author : LZ
 *Date: 2020.2.4
 *Email: 2117610943@qq.com
 */
#include <random>

#include "LZSGAnimationLayer.h"
#include "LZSGDefine.h"
#include "LZSGData.h"
#include "LZSGControlLayer.h"
#include "LZSGButtonLayer.h"
#include "LZSGZombiesAppearControl.h"

#include "Zombies/LZZZombies-Files.h"
#include "Plants/LZPPlants-Files.h"
#include "Plants/EmissionPlants/Bullet/LZPEBBullet-File.h"

#include "Based/LZBCar.h"
#include "Based/LZBCoin.h"
#include "Based/LZBPlayMusic.h"

GSAnimationLayer::GSAnimationLayer(Node* node) :
	_gameScene(node)
,	_global(Global::getInstance())
,   _openLevelData(OpenLevelData::getInstance())
,   _randomSuns(nullptr)
{
	_random.seed(_device());
}

GSAnimationLayer::~GSAnimationLayer()
{
	if (_randomSuns)delete _randomSuns;
}

GSAnimationLayer* GSAnimationLayer::create(Node* node)
{
	GSAnimationLayer* object = new (std::nothrow) GSAnimationLayer(node);
	if (object && object->init())
	{
		object->autorelease();
		return object;
	}
	CC_SAFE_DELETE(object);
	return nullptr;
}

Bullet* GSAnimationLayer::createDifferentBullet(BulletType bulletType)
{
	Bullet* bullet;
	switch (bulletType)
	{
	case BulletType::Pea:              bullet = new Pea(this);              break;
	case BulletType::IcePea:           bullet = new IcePea(this);           break;
	case BulletType::FirePea:          bullet = new FirePea(this);          break;
	case BulletType::Cabbage:          bullet = new Cabbage(this);          break;
	case BulletType::AcidLemonBullet:  bullet = new AcidLemonBullet(this);  break;
	case BulletType::CitronBullet:     bullet = new CitronBullet(this, 0);  break;
	case BulletType::StarFruitBullet:  bullet = new StarFruitBullet(this);  break;
	case BulletType::WaterMelonBullet: bullet = new WaterMelonBullet(this); break;
	case BulletType::WinterMelonBullet:bullet = new WinterMelonBullet(this);break;
	default:break;
	}
	return bullet;
}

Plants* GSAnimationLayer::createDifferentPlants(PlantsType plantsType)
{
	Plants* plants;
	switch (plantsType)
	{
	case PlantsType::SunFlower:           plants = new SunFlower(this);               break;
	case PlantsType::PeaShooter:          plants = new PeaShooter(this);              break;
	case PlantsType::WallNut:             plants = new WallNut(this);                 break;
	case PlantsType::CherryBomb:          plants = new CherryBomb(this);              break;
	case PlantsType::PotatoMine:          plants = new PotatoMine(this);              break;
	case PlantsType::CabbagePult:         plants = new CabbagePult(this);             break;
	case PlantsType::Torchwood:           plants = new Torchwood(this);               break;
	case PlantsType::Spikeweed:           plants = new Spikeweed(this);               break;
	case PlantsType::Garlic:              plants = new Garlic(this);                  break;
	case PlantsType::IceBergLettuce:      plants = new IceBergLettuce(this);          break;
	case PlantsType::Chomper:             plants = new Chomper(this);                 break;
	case PlantsType::IcePeaShooter:       plants = new IcePeaShooter(this);           break;
	case PlantsType::FirePeaShooter:      plants = new FirePeaShooter(this);          break;
	case PlantsType::AcidLemonShooter:    plants = new AcidLemonShooter(this);        break;
	case PlantsType::SunFlowerTwin:       plants = new SunFlowerTwin(this);           break;
	case PlantsType::WaterMelonPult:      plants = new WaterMelonPult(this);          break;
	case PlantsType::Jalapeno:            plants = new Jalapeno(this);                break;
	case PlantsType::JalapenoVariation:   plants = new JalapenoVariation(this);       break;
	case PlantsType::ThreePeaShooter:     plants = new ThreePeaShooter(this);         break;
	case PlantsType::StarFruit:           plants = new StarFruit(this);               break;
	case PlantsType::WinterMelonPult:     plants = new WinterMelonPult(this);         break;
	case PlantsType::Citron:              plants = new Citron(this);                  break;
	case PlantsType::GloomShroom:         plants = new GloomShroom(this);             break;
	default: break;
	}
	return plants;
}

Zombies* GSAnimationLayer::createDifferentZombies(ZombiesType zombiesType, Node* node)
{
	Zombies* zombies;
	switch (zombiesType)
	{
	case ZombiesType::CommonZombies:          zombies = new CommonZombies(node ? node : this);          break;
	case ZombiesType::ConeZombies:            zombies = new ConeZombies(node ? node : this);            break;
	case ZombiesType::BucketZombies:          zombies = new BucketZombies(node ? node : this);          break;
	case ZombiesType::CommonDoorZombies:      zombies = new CommonDoorZombies(node ? node : this);      break;
	case ZombiesType::ConeDoorZombies:        zombies = new ConeDoorZombies(node ? node : this);        break;
	case ZombiesType::BucketDoorZombies:      zombies = new BucketDoorZombies(node ? node : this);      break;
	case ZombiesType::LmpZombies:             zombies = new LmpZombies(node ? node : this);             break;
	case ZombiesType::CommonFlagZombies:      zombies = new CommonFlagZombies(node ? node : this);      break;
	case ZombiesType::ConeFlagZombies:        zombies = new ConeFlagZombies(node ? node : this);        break;
	case ZombiesType::BucketFlagZombies:      zombies = new BucketFlagZombies(node ? node : this);      break;
	case ZombiesType::CommonDoorFlagZombies:  zombies = new CommonDoorFlagZombies(node ? node : this);  break;
	case ZombiesType::ConeDoorFlagZombies:    zombies = new ConeDoorFlagZombies(node ? node : this);    break;
	case ZombiesType::BucketDoorFlagZombies:  zombies = new BucketDoorFlagZombies(node ? node : this);  break;
	case ZombiesType::SnowZombies:            zombies = new SnowZombies(node ? node : this);            break;
	default: break;
	}
	return zombies;
}

bool GSAnimationLayer::init()
{
	if(!Layer::init())return false;

	createRandomSuns();
	showCars();

	schedule([&](float delta) { gameMainLoop(delta); }, "gameMainLoop");
	schedule([&](float) {sunsDeleteUpdate(); coinDeleteUpdate(); }, 2.0f, "sunDeleteUpdate");
	
	return true;
}

void GSAnimationLayer::plantPlants()
{
	PlayMusic::playMusic(rand() % 2 == 0 ? "plant2" : "plant");

	auto plants = createDifferentPlants(buttonLayerInformation->mouseSelectImage->selectPlantsId);
	plants->setPlantPosition(ROW_COLUMN_TO_POSITION(controlLayerInformation->_plantsPosition));
	plants->setPlantLocalZOrder(SET_ANIMATION_Z_ORDER(controlLayerInformation->_plantsPosition));
	plants->setPlantRowAndColumn(controlLayerInformation->_plantsPosition);
	plants->setPlantTag(SET_TAG(controlLayerInformation->_plantsPosition));
	plants->createPlantAnimation();

	PlantsGroup.insert(pair<int, Plants*>(SET_TAG(controlLayerInformation->_plantsPosition), plants));
}

void GSAnimationLayer::deletePlants()
{
	auto multimap_iter = PlantsGroup.equal_range(SET_TAG(controlLayerInformation->_plantsPosition));
	for (auto &plant = multimap_iter.first; plant != multimap_iter.second; ++plant)
	{
		plant->second->setPlantHealthPoint(0);
		plant->second->setPlantVisible(false);
		plant->second->getPlantAnimation()->setTimeScale(0);
		plant->second->getPlantAnimation()->stopAllActions();
	}

	controlLayerInformation->_gameMapInformation->plantsMap[static_cast<int>(
		controlLayerInformation->_plantsPosition.y)][static_cast<int>(controlLayerInformation->_plantsPosition.x)] = NO_PLANTS;
}

void GSAnimationLayer::createZombies()
{
	uniform_int_distribution<unsigned>number(0, 500);
	auto zombies = createDifferentZombies(static_cast<ZombiesType>(
		controlLayerInformation->_zombiesAppearControl->createDifferentTypeZombies(
			controlLayerInformation->_zombiesAppearControl->getZombiesAppearFrequency())));
	auto row = controlLayerInformation->_zombiesAppearControl->getEqualProbabilityForRow();
	zombies->setZombiePosition(Vec2(1780 + number(_random), ZombiesAppearControl::zombiesPosition[row]));
	zombies->setZombieInRow(row);
	zombies->createZombie();
	zombies->setZombieAttributeForGameType();
	ZombiesGroup.push_back(zombies);
	Zombies::zombiesNumbersChange("++");
}

void GSAnimationLayer::createZombiesOnSurvival()
{
	uniform_int_distribution<unsigned>number(0, 500);
	auto zombies = createDifferentZombies(static_cast<ZombiesType>(
		controlLayerInformation->_zombiesAppearControl->createDifferentTypeZombies()));
	zombies->setZombiePosition(Vec2(1780 + number(_random), controlLayerInformation->_zombiesAppearControl->getEqualProbabilityForRow()));
	zombies->createZombie();
	zombies->setZombieAttributeForGameType();
	ZombiesGroup.push_back(zombies);
	Zombies::zombiesNumbersChange("++");
}

GSAnimationLayer* GSAnimationLayer::getAnimationLayer()
{
	return this;
}

void GSAnimationLayer::createRandomSuns()
{
	// ? 条件......
	auto level = _global->userInformation->getCurrentPlayLevels();
	if (level != 36 && level != 50 && level != 52)
	{
		_randomSuns = new SunFlower(this);
		_randomSuns->createRandomSuns();
	}
}

void GSAnimationLayer::showCars()
{
	if (!_global->userInformation->getIsReadFileData())
	{
		const int carpositions[5] = { 180,318,456,594,732 };
		for (int i = 0; i < 5; i++)
		{
			this->runAction(Sequence::create(DelayTime::create(0.5f + 0.1 * i), CallFunc::create([=]()
				{
					PlayMusic::playMusic("plastichit2");

					auto car = new Car(this);
					car->setPosition(Vec2(455, carpositions[i]));
					car->showCar();

					CarsGroup.push_back(car);
				}), nullptr));
		}
	}
}

void GSAnimationLayer::gameMainLoop(float delta)
{
	zombiesEventUpdate(delta); /* 僵尸事件更新 */
	plantsEventUpdate();       /* 植物事件更新 */
	bulletEventUpdate();       /* 子弹事件更新 */
	carsEventUpdate();         /* 小车事件更新 */
}

void GSAnimationLayer::zombiesEventUpdate(float delta)
{
	for (auto zombie = ZombiesGroup.begin(); zombie != ZombiesGroup.end();)
	{
		(*zombie)->setZombieMove(delta);
		(*zombie)->zombieInjuredEventUpdate();
		(*zombie)->playZombieSoundEffect();
		Zombies::judgeZombieWin(zombie);
		Zombies::zombiesDeleteUpdate(zombie);
	}
}

void GSAnimationLayer::plantsEventUpdate()
{
	for (auto plant = PlantsGroup.begin(); plant != PlantsGroup.end(); /*++plant*/)
	{
		plant->second->determineRelativePositionPlantsAndZombies();
		plant->second->checkPlantHealthPoint();

		plantsDeleteUpdate(plant);
	}
}

void GSAnimationLayer::plantsDeleteUpdate(map<int, Plants*>::iterator& plant)
{
	if (!plant->second->getPlantVisible()) /* 植物死亡 */
	{
		if (!plant->second->getPlantIsCanDelete()[0])
		{
			plant->second->getPlantIsCanDelete()[0] = true;
			GSControlLayer::setPlantMapCanPlant(plant->second->getPlantRow(), plant->second->getPlantColumn());

			auto plants = plant;
			plant->second->getPlantAnimation()->runAction(Sequence::create(DelayTime::create(4.0f),
				CallFunc::create([plants]()
					{
						plants->second->getPlantIsCanDelete()[1] = true;
					}), nullptr));
		}
		if (plant->second->getPlantIsCanDelete()[1])
		{
			plant->second->getPlantAnimation()->removeFromParent();
			delete plant->second;
			plant->second = nullptr;
			PlantsGroup.erase(plant++);
		}
		else
		{
			++plant;
		}
	}
	else
	{
		++plant;
	}
}

void GSAnimationLayer::bulletEventUpdate()
{
	for (auto bullet = BulletGroup.begin(); bullet != BulletGroup.end();)
	{
		(*bullet)->bulletAndZombiesCollision();
		
		Bullet::bulletDeleteUpdate(bullet);
	}
}

void GSAnimationLayer::sunsDeleteUpdate()
{
	for (auto sun = SunsGroup.begin(); sun != SunsGroup.end();)
	{
		Sun::deleteSun(sun);
	}
}

void GSAnimationLayer::coinDeleteUpdate()
{
	for (auto coin = CoinsGroup.begin(); coin != CoinsGroup.end();)
	{
		Coin::deleteCoin(coin);
	}
}

void GSAnimationLayer::carsEventUpdate()
{
	for (auto car = CarsGroup.begin(); car != CarsGroup.end();)
	{
		(*car)->createCarListener();
		(*car)->deleteCar(car);
	}
}
