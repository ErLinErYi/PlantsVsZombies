/**
 *Copyright (c) 2020 LZ.All Right Reserved
 *Author : LZ
 *Date: 2020.2.4
 *Email: 2117610943@qq.com
 */

#include "GSAnimationLayer.h"
#include "GSDefine.h"
#include "GSData.h"
#include "GSControlLayer.h"
#include "GSButtonLayer.h"

#include "Plants/Plants-Files.h"
#include "Zombies/Zombies-Files.h"
#include "Plants/EmissionPlants/Bullet/Bullet.h"

#include "Based/Car.h"

GSAnimationLayer::GSAnimationLayer(Node* node) :
	_gameScene(node)
,	_global(Global::getInstance())
,   _openLevelData(OpenLevelData::getInstance())
,   _randomSuns(nullptr)
,   _sunLayer(nullptr)
{
	srand(time(nullptr));
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

void GSAnimationLayer::stopRandomSun()
{
	_sunLayer->stopAllActions();
}

bool GSAnimationLayer::init()
{
	if(!Layer::init())return false;

	createSunLayer();
	createRandomSuns();
	showCars();

	schedule([&](float delta) { gameMainLoop(delta); }, "gameMainLoop");
	schedule([&](float) {sunsDeleteUpdate();}, 2.0f, "sunDeleteUpdate");
	
	return true;
}

void GSAnimationLayer::plantPlants()
{
	AudioEngine::setVolume(AudioEngine::play2d(_global->userInformation->getMusicPath().find(rand() % 2 == 0 ? "plant2" : "plant")->second), _global->userInformation->getSoundEffectVolume());

	auto plants = createDifferentPlants();
	plants->setPlantPosition(ROW_COLUMN_TO_POSITION(controlLayerInformation->_plantsPosition));
	plants->setPlantLocalZOrder(SET_ANIMATION_Z_ORDER(controlLayerInformation->_plantsPosition));
	plants->setPlantRowAndColumn(controlLayerInformation->_plantsPosition);
	plants->setPlantTag(SET_TAG(controlLayerInformation->_plantsPosition));
	plants->createPlantAnimation();
	
	PlantsGroup.insert(pair<int, Plants*>(SET_TAG(controlLayerInformation->_plantsPosition), plants));
}

Plants* GSAnimationLayer::createDifferentPlants()
{
	Plants* plants;
	switch (buttonLayerInformation->mouseSelectImage->selectPlantsId)
	{
	case PlantsType::SunFlower:        plants = new SunFlower(this, _sunLayer);    break;
	case PlantsType::PeaShooter:       plants = new PeaShooter(this);              break;
    case PlantsType::WallNut:          plants = new WallNut(this);                 break;
	case PlantsType::CherryBomb:       plants = new CherryBomb(this);              break;
	case PlantsType::PotatoMine:       plants = new PotatoMine(this);              break;
	case PlantsType::CabbagePult:      plants = new CabbagePult(this);             break;
    case PlantsType::Torchwood:        plants = new Torchwood(this);               break;
	case PlantsType::Spikeweed:        plants = new Spikeweed(this);               break;
	case PlantsType::Garlic:           plants = new Garlic(this);                  break;
	case PlantsType::FirePeaShooter:   plants = new FirePeaShooter(this);          break;
	case PlantsType::Jalapeno:         plants = new Jalapeno(this);                break;
	case PlantsType::AcidLemonShooter: plants = new AcidLemonShooter(this);        break;
	case PlantsType::Citron:           plants = new Citron(this);                  break;
	default: break;
	}
	return plants;
}

void GSAnimationLayer::deletePlants()
{
	auto multimap_iter = PlantsGroup.equal_range(SET_TAG(controlLayerInformation->_plantsPosition));
	for (auto plant = multimap_iter.first; plant != multimap_iter.second; ++plant)
	{
		plant->second->setPlantHealthPoint(0);
		plant->second->setPlantVisible(false);
	}

	controlLayerInformation->_gameMapInformation->plantsMap[static_cast<int>(controlLayerInformation->_plantsPosition.y)][static_cast<int>(controlLayerInformation->_plantsPosition.x)] = NO_PLANTS;
}

void GSAnimationLayer::createZombies()
{
	Zombies* zombies;
	switch (static_cast<ZombiesType>(controlLayerInformation->_zombiesAppearControl->createDifferentTypeZombies(controlLayerInformation->_zombiesAppearControl->getZombiesAppearFrequency())))
	{
	case ZombiesType::CommonZombies:      zombies = new CommonZombies(this);      break;
	case ZombiesType::ConeZombies:        zombies = new ConeZombies(this);        break;
	case ZombiesType::BucketZombies:      zombies = new BucketZombies(this);      break;
	case ZombiesType::CommonDoorZombies:  zombies = new CommonDoorZombies(this);  break;
	case ZombiesType::ConeDoorZombies:    zombies = new ConeDoorZombies(this);    break;
	case ZombiesType::BucketDoorZombies:  zombies = new BucketDoorZombies(this);  break;
	case ZombiesType::LmpZombies:         zombies = new LmpZombies(this);         break;
	default: break;
	}
	zombies->setZombiePosition(Vec2(1780 + rand() % 500, controlLayerInformation->_zombiesAppearControl->getEqualProbabilityForRow()));
	zombies->createZombie();
	zombies->setZombieAttributeForGameType();
	ZombiesGroup.push_back(zombies);
	Zombies::setZombiesNumbers(ZombiesGroup.size());
}

void GSAnimationLayer::createSunLayer()
{
	_sunLayer = Layer::create();
	_gameScene->addChild(_sunLayer, 6, "sunLayer");
}

void GSAnimationLayer::createRandomSuns()
{
	// ? 条件......
	auto level = _global->userInformation->getCurrentPlayLevels();
	if (level != 36 && level != 50 && level != 52)
	{
		_randomSuns = new SunFlower(this, _sunLayer);
		_randomSuns->createRandomSuns();
	}
}

void GSAnimationLayer::showCars()
{
	const int carpositions[5] = { 180,318,456,594,732 };
	for (int i = 0; i < 5; i++)
	{
		this->runAction(Sequence::create(DelayTime::create(0.5f + 0.1 * i), CallFunc::create([=]()
			{
				AudioEngine::setVolume(AudioEngine::play2d(_global->userInformation->getMusicPath().find("plastichit2")->second), _global->userInformation->getSoundEffectVolume());
				auto car = new Car(this);
				car->setPosition(Vec2(455, carpositions[i]));
				car->setScale(0.1f);
				car->showCar();

				CarsGroup.push_back(car);
			}), nullptr));
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
			controlLayerInformation->_gameMapInformation->plantsMap[plant->second->getPlantColumn()][plant->second->getPlantRow()] = NO_PLANTS;

			auto plants = plant;
			plant->second->getPlantAnimation()->runAction(Sequence::create(DelayTime::create(2.0f),
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
void GSAnimationLayer::carsEventUpdate()
{
	for (auto car = CarsGroup.begin(); car != CarsGroup.end();)
	{
		(*car)->createCarListener();
		(*car)->deleteCar(car);
	}
}
