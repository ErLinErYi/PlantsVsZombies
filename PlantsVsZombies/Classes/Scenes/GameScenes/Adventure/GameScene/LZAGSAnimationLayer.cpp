/**
 *Copyright (c) 2020 LZ.All Right Reserved
 *Author : LZ
 *Date: 2020.2.4
 *Email: 2117610943@qq.com
 */
#include <random>

#include "LZAGSAnimationLayer.h"
#include "LZAGSData.h"
#include "LZAGSControlLayer.h"
#include "LZAGSButtonLayer.h"
#include "LZAGSZombiesAppearControl.h"

#include "Zombies/LZZombies-Files.h"
#include "Plants/LZPlants-Files.h"
#include "Plants/EmissionPlants/Bullet/LZBullet-File.h"

#include "Based/LZCar.h"
#include "Based/LZCoin.h"
#include "Based/LZPlayMusic.h"

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

Bullet* GSAnimationLayer::createDifferentBullet(BulletType bulletType, Node* node)
{
	Bullet* bullet = nullptr;
	switch (bulletType)
	{
	case BulletType::Pea:                 bullet = new Pea(node);                 break;
	case BulletType::IcePea:              bullet = new IcePea(node);              break;
	case BulletType::FirePea:             bullet = new FirePea(node);             break;
	case BulletType::Cabbage:             bullet = new Cabbage(node);             break;
	case BulletType::AcidLemonBullet:     bullet = new AcidLemonBullet(node);     break;
	case BulletType::CitronBullet:        bullet = new CitronBullet(node, 0);     break;
	case BulletType::StarFruitBullet:     bullet = new StarFruitBullet(node);     break;
	case BulletType::WaterMelonBullet:    bullet = new WaterMelonBullet(node);    break;
	case BulletType::WinterMelonBullet:   bullet = new WinterMelonBullet(node);   break;
	case BulletType::CatTailBullet:       bullet = new CatTailBullet(node);       break;
	case BulletType::FumeShroomBullet:    bullet = new FumeShroomBullet(node);    break;
	case BulletType::ScaredyShroomBullet: bullet = new ScaredyShroomBullet(node); break;
#ifdef _DEBUG
	default:                                                                      break;
#else
	default:                           bullet = new Pea(node);                    break;
#endif
	}
	return bullet;
}

Plants* GSAnimationLayer::createDifferentPlants(PlantsType plantsType, Node* node)
{
	Plants* plants = nullptr;
	switch (plantsType)
	{
	case PlantsType::SunFlower:           plants = new SunFlower(node);               break;
	case PlantsType::PeaShooter:          plants = new PeaShooter(node);              break;
	case PlantsType::WallNut:             plants = new WallNut(node);                 break;
	case PlantsType::CherryBomb:          plants = new CherryBomb(node);              break;
	case PlantsType::PotatoMine:          plants = new PotatoMine(node);              break;
	case PlantsType::CabbagePult:         plants = new CabbagePult(node);             break;
	case PlantsType::Torchwood:           plants = new Torchwood(node);               break;
	case PlantsType::Spikeweed:           plants = new Spikeweed(node);               break;
	case PlantsType::Garlic:              plants = new Garlic(node);                  break;
	case PlantsType::ScaredyShroom:       plants = new ScaredyShroom(node);           break;
	case PlantsType::IceBergLettuce:      plants = new IceBergLettuce(node);          break;
	case PlantsType::Marigold:            plants = new Marigold(node);                break;
	case PlantsType::Imitater:            plants = new Imitater(node);                break;
	case PlantsType::Chomper:             plants = new Chomper(node);                 break;
	case PlantsType::Squash:              plants = new Squash(node);                  break;
	case PlantsType::IcePeaShooter:       plants = new IcePeaShooter(node);           break;
	case PlantsType::FirePeaShooter:      plants = new FirePeaShooter(node);          break;
	case PlantsType::AcidLemonShooter:    plants = new AcidLemonShooter(node);        break;
	case PlantsType::Blover:              plants = new Blover(node);                  break;
	case PlantsType::SunFlowerTwin:       plants = new SunFlowerTwin(node);           break;
	case PlantsType::WaterMelonPult:      plants = new WaterMelonPult(node);          break;
	case PlantsType::Pumpkin:             plants = new Pumpkin(node);                 break;
	case PlantsType::Jalapeno:            plants = new Jalapeno(node);                break;
	case PlantsType::MagnetShroom:        plants = new MagnetShroom(node);            break;
	case PlantsType::JalapenoVariation:   plants = new JalapenoVariation(node);       break;
	case PlantsType::FumeShroom:          plants = new FumeShroom(node);              break;
	case PlantsType::ThreePeaShooter:     plants = new ThreePeaShooter(node);         break;
	case PlantsType::StarFruit:           plants = new StarFruit(node);               break;
	case PlantsType::WinterMelonPult:     plants = new WinterMelonPult(node);         break;
	case PlantsType::Citron:              plants = new Citron(node);                  break;
	case PlantsType::CatTail:             plants = new CatTail(node);                 break;
	case PlantsType::GloomShroom:         plants = new GloomShroom(node);             break;
#ifdef _DEBUG
	default:                                                                          break;
#else
	default:                              plants = new PeaShooter(node);              break;
#endif
	}
	return plants;
}

Zombies* GSAnimationLayer::createDifferentZombies(ZombiesType zombiesType, Node* node)
{
	Zombies* zombies = nullptr;
	switch (zombiesType)
	{
	case ZombiesType::CommonZombies:          zombies = new CommonZombies(node);          break;
	case ZombiesType::ConeZombies:            zombies = new ConeZombies(node);            break;
	case ZombiesType::BucketZombies:          zombies = new BucketZombies(node);          break;
	case ZombiesType::CommonDoorZombies:      zombies = new CommonDoorZombies(node);      break;
	case ZombiesType::ConeDoorZombies:        zombies = new ConeDoorZombies(node);        break;
	case ZombiesType::BucketDoorZombies:      zombies = new BucketDoorZombies(node);      break;
	case ZombiesType::LmpZombies:             zombies = new LmpZombies(node);             break;
	case ZombiesType::CommonFlagZombies:      zombies = new CommonFlagZombies(node);      break;
	case ZombiesType::ConeFlagZombies:        zombies = new ConeFlagZombies(node);        break;
	case ZombiesType::BucketFlagZombies:      zombies = new BucketFlagZombies(node);      break;
	case ZombiesType::CommonDoorFlagZombies:  zombies = new CommonDoorFlagZombies(node);  break;
	case ZombiesType::ConeDoorFlagZombies:    zombies = new ConeDoorFlagZombies(node);    break;
	case ZombiesType::BucketDoorFlagZombies:  zombies = new BucketDoorFlagZombies(node);  break;
	case ZombiesType::SnowZombies:            zombies = new SnowZombies(node);            break;
	case ZombiesType::GargantuarZombies:      zombies = new GargantuarZombies(node);      break;
	case ZombiesType::BalloonZombies:         zombies = new BalloonZombies(node);         break;
	case ZombiesType::NewspaperZombies:       zombies = new NewspaperZombies(node);       break;
	case ZombiesType::StrongNewspaperZombies: zombies = new StrongNewspaperZombies(node); break;
	case ZombiesType::BrickZombies:           zombies = new BrickZombies(node);           break;
	case ZombiesType::BrickZombies2:          zombies = new BrickZombies2(node);          break;
	case ZombiesType::BucketZombies2:         zombies = new BucketZombies2(node);         break;
	case ZombiesType::BucketZombies3:         zombies = new BucketZombies3(node);         break;
	case ZombiesType::BucketZombies4:         zombies = new BucketZombies4(node);         break;
	case ZombiesType::BucketZombies5:         zombies = new BucketZombies5(node);         break;
	case ZombiesType::BucketZombies6:         zombies = new BucketZombies6(node);         break;
	case ZombiesType::ConeZombies2:           zombies = new ConeZombies2(node);           break;
	case ZombiesType::ConeZombies3:           zombies = new ConeZombies3(node);           break;
	case ZombiesType::ConeZombies4:           zombies = new ConeZombies4(node);           break;
	case ZombiesType::WoodZombies:            zombies = new WoodZombies(node);            break;
	case ZombiesType::WoodZombies2:           zombies = new WoodZombies2(node);           break;
	case ZombiesType::WoodZombies3:           zombies = new WoodZombies3(node);           break;
	case ZombiesType::BonesZombies:           zombies = new BonesZombies(node);           break;
	case ZombiesType::BonesZombies2:          zombies = new BonesZombies2(node);          break;
	case ZombiesType::DoorZombies:            zombies = new DoorZombies(node);            break;
	case ZombiesType::DoorZombies2:           zombies = new DoorZombies2(node);           break;
	case ZombiesType::DoorZombies3:           zombies = new DoorZombies3(node);           break;
	case ZombiesType::DoorZombies4:           zombies = new DoorZombies4(node);           break;
	case ZombiesType::DoorZombies5:           zombies = new DoorZombies5(node);           break;
	case ZombiesType::NewspaperBrickZombies:  zombies = new NewspaperBrickZombies(node);  break;
	case ZombiesType::Bucket4Door4Zombies:    zombies = new Bucket4Door4Zombies(node);    break;
	case ZombiesType::BonesDoor5Zombies:      zombies = new BonesDoor5Zombies(node);      break;
	case ZombiesType::Brick2Door3Zombies:     zombies = new Brick2Door3Zombies(node);     break;
	case ZombiesType::BrickDoor5Zombies:      zombies = new BrickDoor5Zombies(node);      break;
#ifdef _DEBUG
	default:                                                                              break;
#else
	default:                                  zombies = new GargantuarZombies(node);      break;
#endif
	}
	return zombies;
}

bool GSAnimationLayer::init()
{
	if(!Layer::init())return false;

	createRandomSuns();
	showCars();

	schedule([&](float delta) { gameMainLoop(delta); }, "zombiesMainLoop");
	schedule([&](float delta) { gameMainLoop(); }, 1.f / 30.f, "gameMainLoop");
	schedule([&](float) {sunsDeleteUpdate(); coinDeleteUpdate();}, 2.0f, "sunDeleteUpdate");
	
	return true;
}

void GSAnimationLayer::plantPlants()
{
	PlayMusic::playMusic(rand() % 2 == 0 ? "plant2" : "plant");

	auto plants = createDifferentPlants(buttonLayerInformation->mouseSelectImage->selectPlantsId, this);
	plants->setPlantPosition(ROW_COLUMN_TO_POSITION(controlLayerInformation->_plantsPosition));
	plants->setPlantLocalZOrder(SET_ANIMATION_Z_ORDER(controlLayerInformation->_plantsPosition));
	plants->setPlantRowAndColumn(controlLayerInformation->_plantsPosition);
	plants->setPlantTag(SET_TAG(controlLayerInformation->_plantsPosition));
	plants->createPlantAnimation();
	
	PlantsGroup.insert(pair<int, Plants*>(plants->getPlantAnimation()->getTag(), plants));
}

void GSAnimationLayer::deletePlants(const int type)
{
	if (!type)
	{
		auto multimap_iter = PlantsGroup.equal_range(SET_TAG(controlLayerInformation->_plantsPosition));
		for (auto& plant = multimap_iter.first; plant != multimap_iter.second; ++plant)
		{
			plant->second->setPlantRemoveFromMap();
		}

		controlLayerInformation->gameMapInformation->plantsMap[static_cast<int>(
			controlLayerInformation->_plantsPosition.y)][static_cast<int>(controlLayerInformation->_plantsPosition.x)] = NO_PLANTS;
	}
	else
	{
		auto multimap_iter = PlantsGroup.equal_range(SET_TAG(controlLayerInformation->_plantsPosition) + Pumpkin::tagAddition);
		for (auto& plant = multimap_iter.first; plant != multimap_iter.second; ++plant)
		{
			plant->second->setPlantRemoveFromMap();
		}

		controlLayerInformation->gameMapInformation->plantPumpkin[static_cast<int>(
			controlLayerInformation->_plantsPosition.y)][static_cast<int>(controlLayerInformation->_plantsPosition.x)] = false;
	}
}

void GSAnimationLayer::createZombies()
{
	uniform_int_distribution<unsigned>number(0, 500);
	auto zombies = createDifferentZombies(static_cast<ZombiesType>(
		controlLayerInformation->_zombiesAppearControl->createDifferentTypeZombies(
			controlLayerInformation->_zombiesAppearControl->getZombiesAppearFrequency())), this);
	const auto row = controlLayerInformation->_zombiesAppearControl->getEqualProbabilityForRow();
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
		controlLayerInformation->_zombiesAppearControl->createDifferentTypeZombies()), this);
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
	auto world = _global->userInformation->getGameDifficulty();
	const auto level = _global->userInformation->getCurrentPlayLevels();
	if (!world)
	{
		if ((level >= 0 && level <= 20) || (level >= 31 && level <= 40))
		{
			_randomSuns = new SunFlower(this);
			_randomSuns->createRandomSuns();
		}
	}
	else
	{
		if ((level >= 0 && level <= 20) || (level >= 41))
		{
			_randomSuns = new SunFlower(this);
			_randomSuns->createRandomSuns();
		}
	}
}

void GSAnimationLayer::showCars()
{
	if (!_global->userInformation->getIsReadFileData())
	{
		for (int i = 0; i < 5; ++i)
		{
			this->runAction(Sequence::create(DelayTime::create(0.5f + 0.1 * i), CallFunc::create([=]()
				{
					PlayMusic::playMusic("plastichit2");

					auto car = new Car(this);
					car->setPosition(Vec2(490, 180 + 138 * i));
					car->setInRow(i);
					car->setScale(0.8f);
					car->showCar(CarType::ModernCar);

					CarsGroup.push_back(car);
				}), nullptr));
		}
	}
}

void GSAnimationLayer::gameMainLoop()
{
	plantsEventUpdate();       /* 植物事件更新 */
	bulletEventUpdate();       /* 子弹事件更新 */
	carsEventUpdate();         /* 小车事件更新 */
}

void GSAnimationLayer::gameMainLoop(float delta)
{
	zombiesEventUpdate(delta); /* 僵尸事件更新 */
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
			
			GSControlLayer::setPlantMapCanPlant(plant->second->getPlantRow(), plant->second->getPlantColumn(), plant->second->getPlantType());

			auto plants = plant;
			plant->second->getPlantAnimation()->stopAllActions();
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
		(*bullet)->bulletOutMapSetInvisible();
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
