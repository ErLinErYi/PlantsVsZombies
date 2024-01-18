﻿/**
 *Copyright (c) 2019 LZ.All Right Reserved
 *Author : LZ
 *Date: 2019.8.12
 *Modify: 2020.2.3
 *Email: 2117610943@qq.com
 */

#include "LZASPSBackgroundLayer.h"
#include "Scenes/GameScenes/Adventure/GameScene/LZAGSAnimationLayer.h"
#include "Scenes/GameScenes/Adventure/GameScene/LZAGSData.h"
#include "Zombies/LZZombies-Files.h"
#include "Zombies/LZZombies.h"
#include "Based/LZLevelData.h"

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
	showPreviewZombies();

	setBackgroundImagePosition();

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

void SPSBackgroundLayer::showPreviewZombies()
{
	switch (_global->userInformation->getSelectWorldName())
	{
	case WorldName::Mordern:
		createMordernPreviewZombies();
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
	sortZombiesPosition();
}

void SPSBackgroundLayer::createMordernBackgroundImage()
{
	auto world = _global->userInformation->getGameDifficulty();
	auto level = _global->userInformation->getCurrentPlayLevels();
	if (!world)
	{
		if (level >= 0 && level <= 10)
		{
			_global->userInformation->setBackground(Sprite::createWithSpriteFrameName("background.png"));
		}
		else if (level >= 11 && level <= 20)
		{
			_global->userInformation->setBackground(Sprite::createWithSpriteFrameName("background6.png"));
			createSnowParticle();
		}
		else if (level >= 21 && level <= 30)
		{
			_global->userInformation->setBackground(Sprite::createWithSpriteFrameName("background1.png"));
		}
		else if (level >= 31 && level <= 40)
		{
			_global->userInformation->setBackground(Sprite::createWithSpriteFrameName("background4.png"));
		}
		else if (level >= 41)
		{
			_global->userInformation->setBackground(Sprite::createWithSpriteFrameName("background5.png"));
		}
	}
	else
	{
		if (level >= 0 && level <= 10)
		{
			_global->userInformation->setBackground(Sprite::createWithSpriteFrameName("background.png"));
		}
		else if (level >= 11 && level <= 20)
		{
			_global->userInformation->setBackground(Sprite::createWithSpriteFrameName("background3.png"));
		}
		else if (level >= 21 && level <= 30)
		{
			_global->userInformation->setBackground(Sprite::createWithSpriteFrameName("background2.png"));
		}
		else if (level >= 31 && level <= 40)
		{
			_global->userInformation->setBackground(Sprite::createWithSpriteFrameName("background7.png"));
		}
		else if (level >= 41)
		{
			_global->userInformation->setBackground(Sprite::createWithSpriteFrameName("background9.png"));
		}
	}
}

void SPSBackgroundLayer::createSnowParticle()
{
	auto sonw = ParticleSnow::create();
	sonw->retain();
	sonw->setTotalParticles(2000);
	sonw->setLife(6);
	sonw->setLifeVar(1);
	sonw->setGravity(Vec2(0, -10));
	sonw->setSpeed(130);
	sonw->setSpeedVar(30);
	sonw->setEmissionRate(sonw->getTotalParticles() / sonw->getLife());
	_global->userInformation->getBackground()->addChild(sonw, 10);
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

void SPSBackgroundLayer::createMordernPreviewZombies()
{
	const uniform_int_distribution<unsigned>n(0, 2);
	uniform_int_distribution<unsigned>n1(0, 550);
	uniform_int_distribution<unsigned>n2(0, 650);
	auto &type=_openLevelData->readLevelData(_openLevelData->getLevelNumber())->getZombiesType();

	int k = roundf(15.f / type.size());
	k = k <= 0 ? 1 : k;
	for (unsigned int i = 0; i < type.size(); ++i)
	{
		for (int j = 0; j < k; ++j)
		{
			auto zombies = GSAnimationLayer::createDifferentZombies(static_cast<ZombiesType>(type.at(i)), this);
			zombies->setZombiePosition(getPreviewZombiesPosition(n1, n2));
			zombies->setZombieTag(_previewZombiesTag++);
			zombies->createPreviewZombie();
			zombies->setZombieAttributeForGameType();
			zombies->setZombieOpacity(255);

			PreviewZombies previewzombies{};
			previewzombies.animation = zombies->getZombieAnimation();
			previewzombies.position_y = previewzombies.animation->getPositionY();
			previewzombies.position_x = previewzombies.animation->getPositionX();
			_previewZombies.emplace_back(previewzombies);
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
	bool can = false;
	int x, y;
	do
	{
		can = false;
		x = 2050 + n1(_random);
		y = n2(_random) + 40;
		for (const auto& p : _previewZombies)
		{
			if (sqrt(powf(p.position_x - x, 2) + powf(p.position_y - y, 2)) < 100)
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