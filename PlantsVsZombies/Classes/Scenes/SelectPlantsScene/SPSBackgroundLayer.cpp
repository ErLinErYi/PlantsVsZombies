/**
 *Copyright (c) 2019 LZ.All Right Reserved
 *Author : LZ
 *Date: 2019.8.12
 *Modify: 2020.2.3
 *Email: 2117610943@qq.com
 */

#include "SPSBackgroundLayer.h"

#include "Zombies/Zombies-Files.h"
#include "Zombies/Zombies.h"
#include "Based/LevelData.h"

SPSBackgroundLayer::SPSBackgroundLayer() :
	_global(Global::getInstance()),
	_openLevelData(OpenLevelData::getInstance()),
	_previewZombiesTag(0)
{
	_random.seed(_device());
}

SPSBackgroundLayer::~SPSBackgroundLayer()
{
}

bool SPSBackgroundLayer::init()
{
	if (!Layer::init()) return false;

	/* 创建背景 */
	createBackGroundEffect();

	setBackgroundImagePosition();

	/* 创建预览僵尸 */
	createPreviewZombie();

	return true;
}

void SPSBackgroundLayer::createBackGroundEffect()
{
	switch (_global->userInformation->getSelectWorldName())
	{
	case WorldName::Mordern:
		createMordernBackgroundImage();
		break;
	case WorldName::DarkAges:
		createFireEffect(_global->userInformation->getBackground());
		break;
	case WorldName::NeonMixtapeTour:
		break;
	case WorldName::FrostibiteCaves:
		break;
	default:
		break;
	}
}

void SPSBackgroundLayer::createMordernBackgroundImage()
{
	switch (_global->userInformation->getCurrentPlayLevels())
	{
	case 35:
	{
		_global->userInformation->setBackground(Sprite::createWithSpriteFrameName("background6.png"));
		auto snow = ParticleSnow::create();
		_global->userInformation->getBackground()->addChild(snow);
	}
		break;
	case 36:
		_global->userInformation->setBackground(Sprite::createWithSpriteFrameName("background1.png"));
		break;
	case 37:
		_global->userInformation->setBackground(Sprite::createWithSpriteFrameName("background4.png"));
		break;
	case 50:
		_global->userInformation->setBackground(Sprite::createWithSpriteFrameName("background5.png"));
		break;
	case 51:
		_global->userInformation->setBackground(Sprite::createWithSpriteFrameName("background3.png"));
		break;
	case 52:
		_global->userInformation->setBackground(Sprite::createWithSpriteFrameName("background2.png"));
		break;
	default:
		_global->userInformation->setBackground(Sprite::createWithSpriteFrameName("background.png"));
		break;
	}
}

void SPSBackgroundLayer::createFireEffect(Sprite* name)
{
	auto fire = ParticleFire::create();
	fire->setPosition(Vec2(1900, 30));
	name->addChild(fire);

	auto fire1 = ParticleFire::create();
	fire1->setPosition(Vec2(1820, 1000));
	name->addChild(fire1);
}

void SPSBackgroundLayer::createPreviewZombie()
{
	srand(time(nullptr));
	switch (_global->userInformation->getSelectWorldName())
	{
	case WorldName::Mordern:
		createMordernPreviewZombies();
		break;
	case WorldName::DarkAges:
		break;
	case WorldName::NeonMixtapeTour:
		break;
	case WorldName::FrostibiteCaves:
		break;
	default:
		break;
	}

	sortZombiesPosition();
}

void SPSBackgroundLayer::createMordernPreviewZombies()
{
	uniform_int_distribution<unsigned>n(0, 2);
	uniform_int_distribution<unsigned>n1(0, 550);
	uniform_int_distribution<unsigned>n2(0, 650);
	int number = _openLevelData->readLevelData(_openLevelData->getLevelNumber())->getZombiesType().size();
	for (int i = 0; i < number; ++i)
	{
		int k;
		_previewZombies.size() < 15 ? k = n(_random) + 1 : k = 1;
		for (int j = 0; j < k; ++j)
		{
			auto zombies = createDifferentZombies(i);
			zombies->setZombiePosition(getPreviewZombiesPosition(n1, n2));
			zombies->setZombieTag(_previewZombiesTag++);
			zombies->createPreviewZombie();
			zombies->setZombieAttributeForGameType();

			PreviewZombies previewzombies;
			previewzombies.animation = zombies->getZombieAnimation();
			previewzombies.position_y = previewzombies.animation->getPositionY();
			previewzombies.position_x = previewzombies.animation->getPositionX();
			_previewZombies.push_back(previewzombies);
		}
	}
}

void SPSBackgroundLayer::setBackgroundImagePosition()
{
	if (_global->userInformation->getBackground() != nullptr)
	{
		_global->userInformation->getBackground()->setAnchorPoint(Point(0, 0));
		_global->userInformation->getBackground()->setContentSize(Size(2930, 1081));
		this->addChild(_global->userInformation->getBackground(), 0, "previewBackgroundImage");
	}
}

void SPSBackgroundLayer::sortZombiesPosition()
{
	std::sort(_previewZombies.begin(), _previewZombies.end(), cmp);

	int i = 0;
	for (auto& pre : _previewZombies)
	{
		pre.animation->setLocalZOrder(110 + i++);
		pre.animation->setOpacity(255);
		pre.animation->getChildByName("shadow")->setOpacity(255);
	}
}

Vec2 SPSBackgroundLayer::getPreviewZombiesPosition(UID& n1, UID& n2)
{
	Vec2 vec2;
	bool can;
	int x, y;
	do
	{
		can = false;
		x = 2050 + n1(_random);
		y = n2(_random) + 80;
		for (auto p : _previewZombies)
		{
			if (sqrt(pow(p.position_x - x, 2) + pow(p.position_y - y, 2)) < 100)
			{
				can = true;
				break;
			}
		}
	} while (can);

	vec2.x = x;
	vec2.y = y;

	return vec2;
}

bool SPSBackgroundLayer::cmp(PreviewZombies& a, PreviewZombies& b)
{
	return a.position_y > b.position_y;
}

Zombies* SPSBackgroundLayer::createDifferentZombies(const int& id)
{
	Zombies* zombies;
	switch (static_cast<ZombiesType>(_openLevelData->readLevelData(_openLevelData->getLevelNumber())->getZombiesType().at(id)))
	{
	case ZombiesType::CommonZombies:          zombies = CommonZombies::create(this);            break;
	case ZombiesType::ConeZombies:            zombies = ConeZombies::create(this);              break;
	case ZombiesType::BucketZombies:          zombies = BucketZombies::create(this);            break;
	case ZombiesType::CommonDoorZombies:      zombies = CommonDoorZombies::create(this);        break;
	case ZombiesType::ConeDoorZombies:        zombies = ConeDoorZombies::create(this);          break;
	case ZombiesType::BucketDoorZombies:      zombies = BucketDoorZombies::create(this);        break;
	case ZombiesType::LmpZombies:             zombies = LmpZombies::create(this);               break;
	case ZombiesType::CommonFlagZombies:      zombies = CommonFlagZombies::create(this);        break;
	case ZombiesType::ConeFlagZombies:        zombies = ConeFlagZombies::create(this);          break;
	case ZombiesType::BucketFlagZombies:      zombies = BucketFlagZombies::create(this);        break;
	case ZombiesType::CommonDoorFlagZombies:  zombies = CommonDoorFlagZombies::create(this);    break;
	case ZombiesType::ConeDoorFlagZombies:    zombies = ConeDoorFlagZombies::create(this);      break;
	case ZombiesType::BucketDoorFlagZombies:  zombies = BucketDoorFlagZombies::create(this);    break;
	case ZombiesType::SnowZombies:            zombies = SnowZombies::create(this);              break;
	default: break;
	}
	return zombies;
}
