/**
 *Copyright (c) 2019 LZ.All Right Reserved
 *Author : LZ
 *Date: 2021.7.04
 *Emal: 2117610943@qq.com
 */
#include "LZIZombiesData.h"
#include "LZIZombiesScene.h"
#include "LZIAnimationLayer.h"
#include "Scenes/GameScenes/Adventure/GameScene/LZAGSData.h"
#include "Scenes/GameScenes/Adventure/GameScene/LZAGSInformationLayer.h"
#include "Scenes/GameScenes/Adventure/GameScene/LZAGSAnimationLayer.h"
#include "Scenes/GameScenes/Adventure/GameScene/LZAGSControlLayer.h"
#include "Plants/LZPlants.h"
#include "Plants/LZPlants-Files.h"
#include "Zombies/LZZombies.h"
#include "Based/LZGlobalVariable.h"

IZombiesData* IZombiesData::_instance = nullptr;

IZombiesData::IZombiesData()
{
}

IZombiesData::~IZombiesData()
{
}

IZombiesData* IZombiesData::getInstance()
{
	if (_instance == nullptr)
	{
		_instance = new (std::nothrow)IZombiesData;
	}
	return _instance;
}

void IZombiesData::caveLevelData(char* key)
{
	switch (UserData::openLevelData())
	{
	case openUserDataReturnType::FileExistCorrect:
		if ((*_levelDataDocument).HasMember(key))
			(*_levelDataDocument).RemoveMember(key);
		break;
	case openUserDataReturnType::FileExistError:
		_fileUtils->removeFile(getLevelDataFileName());
		return;
		break;
	}

	if (!_levelDataDocument->IsObject())_levelDataDocument->SetObject();

	rapidjson::Value object(rapidjson::kObjectType);
	_levelDataDocument->AddMember(rapidjson::StringRef(key), object, _levelDataDocument->GetAllocator());

	caveLevelDataVersion();
	caveLevelPlantsData(key);
	caveLevelZombiesData(key);
	caveLevelOtherData(key);
	caveLevelSunData(key);
	caveLevelCoinData(key);
	caveLevelBulletData(key);
	caveLevelBrainData(key);

	flushLevelData();
}

void IZombiesData::openLevelData(char* key)
{
	if (readLevelData())
	{
		openLevelOtherData(key);
		openLevelPlantsData(key);
		openLevelZombiesData(key);
		openLevelSunData(key);
		openLevelCoinData(key);
		openLevelBulletData(key);
		openLevelBrainData(key);
	}
	else
	{
		replaceScene();
	}
}

string IZombiesData::getLevelDataFileName()
{
	return _fileUtils->getWritablePath() + "IZombiesDataFile_" + to_string(_global->userInformation->getUserCaveFileNumber()) + ".sav";
}

void IZombiesData::caveLevelOtherData(char* key)
{
	rapidjson::Document::AllocatorType& allocator = _levelDataDocument->GetAllocator();
	rapidjson::Value object(rapidjson::kObjectType);

	object.AddMember("SunNumbers", _global->userInformation->getSunNumbers(), allocator);

	(*_levelDataDocument)[key].AddMember("OtherData", object, _levelDataDocument->GetAllocator());
}

void IZombiesData::caveLevelBrainData(char* key)
{
	unsigned int brainNumber = 0;
	rapidjson::Value _object(rapidjson::kObjectType);
	rapidjson::Document::AllocatorType& allocator = _levelDataDocument->GetAllocator();
	(*_levelDataDocument)[key].AddMember("Brain", _object, allocator);
	(*_levelDataDocument)[key]["Brain"].AddMember("BrainNumbers", IAnimationLayer::BrainGroup.size(), allocator);

	for (auto brain : IAnimationLayer::BrainGroup)
	{
		rapidjson::Value object(rapidjson::kObjectType);

		object.AddMember("PositionY", brain.brain->getPositionY(), allocator);
		object.AddMember("LocalZOrder", brain.brain->getLocalZOrder(), allocator);
		object.AddMember("Row", brain.row, allocator);
		object.AddMember("Enable", brain.enable, allocator);
		
		(*_levelDataDocument)[key]["Brain"].AddMember(numberToString(++brainNumber, allocator), object, _levelDataDocument->GetAllocator());
	}
}

void IZombiesData::replaceScene()
{
	Director::getInstance()->replaceScene(IZombiesScene::create());
}

void IZombiesData::openLevelPlantsData(char* key)
{
	const auto plantsNumbers = (*_levelDataDocument)[key]["Plants"]["PlantsNumber"].GetInt();
	for (int i = 1; i <= plantsNumbers; ++i)
	{
		auto type = static_cast<PlantsType>((*_levelDataDocument)[key]["Plants"][to_string(i).c_str()]["PlantsType"].GetInt());
		auto plants = GSAnimationLayer::createDifferentPlants(type, animationLayerInformation);
		plants->setPlantPosition(Vec2(
			(*_levelDataDocument)[key]["Plants"][to_string(i).c_str()]["PlantsPositionX"].GetFloat(),
			(*_levelDataDocument)[key]["Plants"][to_string(i).c_str()]["PlantsPositionY"].GetFloat()));
		plants->setPlantLocalZOrder((*_levelDataDocument)[key]["Plants"][to_string(i).c_str()]["PlantsLocalZOrder"].GetInt());
		plants->setPlantRowAndColumn(Vec2(
			(*_levelDataDocument)[key]["Plants"][to_string(i).c_str()]["PlantsColumn"].GetInt(),
			(*_levelDataDocument)[key]["Plants"][to_string(i).c_str()]["PlantsRow"].GetInt()));
		plants->setPlantTag((*_levelDataDocument)[key]["Plants"][to_string(i).c_str()]["PlantsTag"].GetInt());
		plants->readPlantInforamtion(_levelDataDocument, key, i);

		if (type == PlantsType::SunFlower)
		{
			dynamic_cast<SunFlower*>(plants)->setIZombiesType(true);
		}
		plants->createPlantAnimation();

		openLevelPlantsAnimationData(key, to_string(i).c_str(), plants);
		plants->getPlantAnimation()->update((*_levelDataDocument)[key]["Plants"][to_string(i).c_str()]["PlantsAnimationTime"].GetFloat());
		plants->getPlantAnimation()->setTimeScale((*_levelDataDocument)[key]["Plants"][to_string(i).c_str()]["PlantsAnimationTimeScale"].GetFloat());
		plants->setPlantHealthPoint((*_levelDataDocument)[key]["Plants"][to_string(i).c_str()]["PlantsHealthPoint"].GetFloat());
		plants->getPlantAnimation()->getChildByName("SplashOfSoil")->setOpacity(0);
		plants->getPlantAnimation()->getChildByName("BufEffect")->setOpacity(0);

		plants->setPlantOtherInformation(_levelDataDocument, key, i);

		PlantsGroup.insert(pair<int, Plants*>((*_levelDataDocument)[key]["Plants"][to_string(i).c_str()]["PlantsTag"].GetInt(), plants));

		if (type != PlantsType::Pumpkin)
		{
			controlLayerInformation->gameMapInformation->plantsMap[plants->getPlantRow()][plants->getPlantColumn()] = static_cast<unsigned int>(type);/* 地图记录种植的植物 */
		}
	}
}

void IZombiesData::openLevelZombiesData(char* key)
{
	const auto zombiesNumbers = (*_levelDataDocument)[key]["Zombies"]["ZombiesNumber"].GetInt();
	for (int i = 1; i <= zombiesNumbers; ++i)
	{
		auto zombies = GSAnimationLayer::createDifferentZombies(
			static_cast<ZombiesType>((*_levelDataDocument)[key]["Zombies"][to_string(i).c_str()]["ZombieType"].GetInt()), animationLayerInformation);

		zombies->setZombiePosition(Vec2(
			(*_levelDataDocument)[key]["Zombies"][to_string(i).c_str()]["ZombiePositionX"].GetFloat(),
			(*_levelDataDocument)[key]["Zombies"][to_string(i).c_str()]["ZombiePositionY"].GetFloat()));
		zombies->setZombieInRow((*_levelDataDocument)[key]["Zombies"][to_string(i).c_str()]["ZombieInRow"].GetInt());
		zombies->createZombie();
		zombies->setZombieAttributeForGameTypeInvalid(true);

		openLevelZombiesAnimationData(key, to_string(i).c_str(), zombies);

		zombies->getZombieAnimation()->update((*_levelDataDocument)[key]["Zombies"][to_string(i).c_str()]["ZombieAnimationTime"].GetFloat());
		zombies->getZombieAnimation()->setTimeScale((*_levelDataDocument)[key]["Zombies"][to_string(i).c_str()]["ZombieAnimationTimeScale"].GetFloat());
		zombies->setZombieCurrentBloodVolume((*_levelDataDocument)[key]["Zombies"][to_string(i).c_str()]["ZombieCurrentBloodVolume"].GetFloat());
		zombies->setZombieCurrentBodyShieldVolume((*_levelDataDocument)[key]["Zombies"][to_string(i).c_str()]["ZombieCurrentBodyShieldVolume"].GetFloat());
		zombies->setZombieCurrentHeadShieldVolume((*_levelDataDocument)[key]["Zombies"][to_string(i).c_str()]["ZombieCurrentHeadShieldVolume"].GetFloat());
		zombies->getZombieAnimation()->setOpacity((*_levelDataDocument)[key]["Zombies"][to_string(i).c_str()]["ZombieOpacity"].GetInt());
		zombies->getZombieAnimation()->getChildByName("shadow")->setOpacity(zombies->getZombieAnimation()->getOpacity());
		zombies->setZombieIsFrozen((*_levelDataDocument)[key]["Zombies"][to_string(i).c_str()]["ZombieIsFrozen"].GetInt());
		zombies->setZombieCurrentSpeed((*_levelDataDocument)[key]["Zombies"][to_string(i).c_str()]["ZombieCurrentSpeed"].GetFloat());
		zombies->setZombieTimerTime((*_levelDataDocument)[key]["Zombies"][to_string(i).c_str()]["ZombieTimerTimeSlow"].GetInt(), true);
		zombies->setZombieTimerTime((*_levelDataDocument)[key]["Zombies"][to_string(i).c_str()]["ZombieTimerTimeStop"].GetInt());

		zombies->setZombieIsShowLoseLimbsAnimation(false);
		zombies->setZombieIsShowLoseShieldAnimation(false);
		zombies->zombieInjuredEventUpdate();
		zombies->setZombieIsShowLoseLimbsAnimation(true);
		zombies->setZombieIsShowLoseShieldAnimation(true);
		zombies->readZombieInformation();

		ZombiesGroup.push_back(zombies);
		Zombies::zombiesNumbersChange("++");
	}
}

void IZombiesData::openLevelBrainData(char* key)
{
	const auto barinNumbers = (*_levelDataDocument)[key]["Brain"]["BrainNumbers"].GetInt();
	for (int i = 1; i <= barinNumbers; ++i)
	{
		auto brain = Sprite::createWithSpriteFrameName("Brain.png");
		brain->setPosition(Vec2(500, (*_levelDataDocument)[key]["Brain"][to_string(i).c_str()]["PositionY"].GetFloat()));
		brain->setScale(0.3f);
		brain->setLocalZOrder((*_levelDataDocument)[key]["Brain"][to_string(i).c_str()]["LocalZOrder"].GetInt());
		animationLayerInformation->addChild(brain);

		auto shadow = Sprite::createWithSpriteFrameName("plantshadow.png");
		shadow->setScale(8.0f);
		shadow->setPosition(Vec2(220, 110));
		brain->addChild(shadow, -1);

		IAnimationLayer::Brain b;
		b.brain = brain;
		b.enable = (*_levelDataDocument)[key]["Brain"][to_string(i).c_str()]["Enable"].GetBool();
		b.row = (*_levelDataDocument)[key]["Brain"][to_string(i).c_str()]["Row"].GetInt();
		IAnimationLayer::BrainGroup.push_back(b);
	}
}

void IZombiesData::openLevelOtherData(char* key)
{
	_global->userInformation->setSunNumbers((*_levelDataDocument)[key]["OtherData"]["SunNumbers"].GetInt());
	informationLayerInformation->updateSunNumbers();
}
