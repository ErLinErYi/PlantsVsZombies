/**
 *Copyright (c) 2019 LZ.All Right Reserved
 *Author : LZ
 *Email: 2117610943@qq.com
 */

#include "LZcar.h"

#include "Zombies/LZZombies.h"
#include "Scenes/GameScenes/Adventure/GameScene/LZAGSData.h"
#include "Scenes/GameScenes/Adventure/GameScene/LZAGSControlLayer.h"
#include "Based/LZPlayMusic.h"

Car::Car(Node* node) :
	_carImage(nullptr),
	_position(Vec2::ZERO),
	_isLive(false),
	_tag(-1),
	_row(-1),
	_carState(1),
	_scale(0),
	_node(node),
	_carType(CarType::null),
	_global(Global::getInstance())
{
}

Car::~Car()
{
}

Car* Car::create(Node* node)
{
	Car* car = new (std::nothrow) Car(node);
	if (car && car->init())
	{
		car->autorelease();
		return car;
	}
	CC_SAFE_DELETE(car);
	return nullptr;
}

void Car::showCar(CarType type)
{
	string name;
	_carType = type;
	switch (_carType)
	{
	case CarType::ModernCar:   name = "Car.png";         break;
	case CarType::BigMapCar:   name = "BigMapCar.png";   break;
	case CarType::FutureCar:   name = "FutureCar.png";   break;
	case CarType::WildWestCar: name = "WildWestCar.png"; break;
	default:                   name = "Car.png";         break;
	}

	_carImage = Sprite::createWithSpriteFrameName(name);
	_carImage->setPosition(_position);
	_carImage->setLocalZOrder(getZOrder());
	_carImage->setScale(_scale);
	_node->addChild(_carImage);

	createCarShadow();
	createAppearSpecialEffect();

	if (!_global->userInformation->getIsReadFileData())
		_carImage->runAction(ScaleTo::create(0.2f, 0.8f));
}

void Car::setPosition(const Vec2& position)
{
	_position = position;
}

void Car::setCarTag(const int tag)
{
	_tag = tag;
}

void Car::setParent(Node* node)
{
	_node = node;
}

void Car::setScale(const float& scale)
{
	_scale = scale;
}

void Car::setLive(bool isLive)
{
	_isLive = isLive;
}

void Car::setInRow(int row)
{
	_row = row;
}

void Car::releaseFunction()
{
	_carImage->removeFromParent();
}

int Car::getInRow()
{
	return _row;
}

bool Car::getLive() const
{
	return _isLive;
}

int Car::getCarTag() const
{
	return _carImage->getTag();
}

Sprite* Car::getCar()
{
	return _carImage;
}

CarType Car::getCarType()
{
	return _carType;
}

void Car::createCarListener()
{
	for (auto& zombie : ZombiesGroup)
	{
		if (zombie->getZombieIsEnterMap() && zombie->getZombieIsSurvive() &&         /* 僵尸进入地图 && 僵尸没有死亡 */       
			getZombieIsSameLineWithCar(zombie) && getzombieIsEncounterCar(zombie))   /* 僵尸与小车在同一行 && 僵尸遇到小车 */
		{
			_isLive = true;

			zombie->setZombieDeath(true);
			if (zombie->getZombieType() != ZombiesType::GargantuarZombies)
			{
				zombie->setZombieVisible(false);
			}
		}
	}

	carStartUp();
}

void Car::carStartUp()
{
	if (_isLive)
	{
		if (_carState == 1)
		{
			_carState = 2;
			PlayMusic::playMusic("lawnmower");
			_carImage->runAction(MoveBy::create(10.f, Vec2(7600, 0)));
		}
	}
}

void Car::deleteCar(list<Car*>::iterator& car)
{
	if ((*car)->getCar()->getPositionX() > 
		controlLayerInformation->gameMapInformation->mapRight + 500)
	{
		(*car)->releaseFunction();
		delete* car;
		*car = nullptr;
		CarsGroup.erase(car++);
	}
	else
	{
		++car;
	}
}

void Car::createCarShadow()
{
	auto shadow = Sprite::createWithSpriteFrameName("plantshadow.png");
	shadow->setScaleX(4.0f);
	shadow->setScaleY(3.0f);
	shadow->setPosition(Vec2(120, 30));
	_carImage->addChild(shadow, -1);
}

void Car::createAppearSpecialEffect()
{
	auto fog = Sprite::createWithSpriteFrameName("AwardPickupGlow.png");
	fog->setScale(4.0f);
	fog->setName("fog");
	fog->setPosition(Vec2(90, 30));
	_carImage->addChild(fog);
	fog->runAction(Sequence::create(DelayTime::create(0.5f), FadeOut::create(0.5f),
		CallFunc::create([fog]() {fog->removeFromParent(); }), nullptr));
}

bool Car::getZombieIsSameLineWithCar(Zombies* zombie) const
{
	return zombie->getZombieInRow() == _row;
}

bool Car::getzombieIsEncounterCar(Zombies* zombie) const
{
	return zombie->getZombieAnimation()->getPositionX() - 80 < _carImage->getPositionX() ? true : false;
}

int Car::getZOrder() const
{
	return (50 - _row) * 100 + 90;
}

