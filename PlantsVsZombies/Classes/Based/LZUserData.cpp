/**
 *Copyright (c) 2020 LZ.All Right Reserved
 *Author : LZ
 *Date: 2020.4.20
 *Email: 2117610943@qq.com
 */

#include "base/base64.h"
#include "LZUserData.h"
#include "LZGlobalVariable.h"
#include "LZCar.h"
#include "LZCoin.h"
#include "Scenes/GameScenes/Adventure/GameScene/LZAGSData.h"
#include "Scenes/GameScenes/Adventure/GameScene/LZAGSAnimationLayer.h"
#include "Scenes/GameScenes/Adventure/GameScene/LZAGSButtonLayer.h"
#include "Scenes/GameScenes/Adventure/GameScene/LZAGSControlLayer.h"
#include "Scenes/GameScenes/Adventure/GameScene/LZAGSZombiesAppearControl.h"
#include "Scenes/GameScenes/Adventure/GameScene/LZAGSInformationLayer.h"
#include "Scenes/GameScenes/Adventure/GameScene/LZAGSBackgroundLayer.h"
#include "Scenes/GameScenes/Adventure/SelectPlantsScene/LZASelectPlantsScene.h"
#include "Scenes/GameScenes/BigMap/GameScene/LZBigMapGameScene.h"
#include "Plants/LZPlants.h"
#include "Plants/LZPlants-Files.h"
#include "Plants/DefensePlants/LZSunFlower.h"
#include "Plants/EmissionPlants/Bullet/LZBullet.h"
#include "Plants/EmissionPlants/Bullet/LZBullet-File.h"
#include "Zombies/LZZombies.h"

#ifdef DLLTEST
#include "libpvz.h"
#pragma comment (lib,"libpvz")
#endif // !DLLTEST

UserData* UserData::_instance = nullptr;
int UserData::_levelDataVersion = 1310;

UserData::UserData() :
  _global(Global::getInstance())
, _fileUtils(FileUtils::getInstance())
, _userDataDocument(nullptr)
, _levelDataDocument(nullptr)
, _isAnewReadData(false)
{
}

UserData::~UserData()
{
	if (_userDataDocument) delete _userDataDocument, _userDataDocument = nullptr;
	if (_levelDataDocument) delete _levelDataDocument, _levelDataDocument = nullptr;
	_isAnewReadData = false;
}

UserData* UserData::getInstance()
{
	if (_instance == nullptr)
	{
		_instance = new (std::nothrow)UserData;
	}
	return _instance;
}

void UserData::flushUserData()
{
	StringBuffer buffer;
	rapidjson::Writer<StringBuffer> Writer(buffer);
	_userDataDocument->Accept(Writer);

	string str = string(buffer.GetString());

#ifdef DEBUG
	_fileUtils->writeStringToFile(str, getUserDataFileName());
#else
#   ifndef DLLTEST
	_fileUtils->writeStringToFile(encryption(str), getUserDataFileName());
#   else
	char* buf = new char[str.length() * 3];
	encryption(str.c_str(), buf);
	_fileUtils->writeStringToFile(buf, getUserDataFileName());
	CC_SAFE_DELETE(buf);
#   endif
#endif
	_isAnewReadData = false; // if flush must anew read data
}

void UserData::flushLevelData()
{
	StringBuffer buffer;
	rapidjson::Writer<StringBuffer> Writer(buffer);
	_levelDataDocument->Accept(Writer);

	string str = string(buffer.GetString());

#ifdef DEBUG
	_fileUtils->writeStringToFile(str, getLevelDataFileName());
#else
#   ifndef DLLTEST
	_fileUtils->writeStringToFile(encryption(str), getLevelDataFileName());
#   else
	char* buf = new char[str.length() * 3];
	encryption(str.c_str(), buf);
	_fileUtils->writeStringToFile(buf, getLevelDataFileName());
	CC_SAFE_DELETE(buf);
#   endif
#endif
}

void UserData::flushSurvivalData()
{
	StringBuffer buffer;
	rapidjson::Writer<StringBuffer> Writer(buffer);
	_levelDataDocument->Accept(Writer);

	string str = string(buffer.GetString());

#ifdef DEBUG
	_fileUtils->writeStringToFile(str, getSurvivalLevelDataFileName());
#else
#   ifndef DLLTEST
	_fileUtils->writeStringToFile(encryption(str), getSurvivalDataFileName());
#   else
	char* buf = new char[str.length() * 3];
	encryption(str.c_str(), buf);
	_fileUtils->writeStringToFile(buf, getSurvivalDataFileName());
	CC_SAFE_DELETE(buf);
#   endif
#endif
}

openUserDataReturnType UserData::openUserData()
{
	// 如果有这个存档
	if (_fileUtils->isFileExist(getUserDataFileName()))
	{
		if (_userData.empty()|| !_isAnewReadData)
		{
			_isAnewReadData = true;
#ifdef DEBUG
			_userData = _fileUtils->getStringFromFile(getUserDataFileName());
			_userDataDocument->Parse<rapidjson::kParseDefaultFlags>(_userData.c_str());
#else
#   ifndef DLLTEST
			_userData = _fileUtils->getStringFromFile(getUserDataFileName());
			_userDataDocument->Parse<rapidjson::kParseDefaultFlags>(decryption(_userData).c_str());
#   else
			_userData = _fileUtils->getStringFromFile(getUserDataFileName());
			char* buf = new char[_userData.length()];
			if (decryption(_userData.c_str(), buf)) {
				_userDataDocument->Parse<rapidjson::kParseDefaultFlags>(buf);
			}
			else {
				CC_SAFE_DELETE(buf);
				return openUserDataReturnType::FileExistError;
		}
			CC_SAFE_DELETE(buf);
#   endif
#endif
			if (_userDataDocument->HasParseError()) {
				return openUserDataReturnType::FileExistError;
			}
		}
		return openUserDataReturnType::FileExistCorrect;
	}
	else
	{
		if (!_userDataDocument->IsObject())
		{
			_userDataDocument->SetObject();
			rapidjson::Value _object(rapidjson::kObjectType);
			_userDataDocument->AddMember("UserData", _object, _userDataDocument->GetAllocator());
		}
		return openUserDataReturnType::FileNotExist;
	}
}

openUserDataReturnType UserData::openLevelData()
{
	// 如果有这个存档
	if (_fileUtils->isFileExist(getLevelDataFileName()))
	{
#ifdef DEBUG
		_levelData = _fileUtils->getStringFromFile(getLevelDataFileName());
		_levelDataDocument->Parse<rapidjson::kParseDefaultFlags>(_levelData.c_str());
#else
#   ifndef DLLTEST
		_levelData = _fileUtils->getStringFromFile(getLevelDataFileName());
		_levelDataDocument->Parse<rapidjson::kParseDefaultFlags>(decryption(_levelData).c_str());
#   else
		_levelData = _fileUtils->getStringFromFile(getLevelDataFileName());
		char* buf = new char[_levelData.length()];
		if (decryption(_levelData.c_str(), buf)) {
			_levelDataDocument->Parse<rapidjson::kParseDefaultFlags>(buf);
		}
		else {
			CC_SAFE_DELETE(buf);
			return openUserDataReturnType::FileExistError;
		}
		CC_SAFE_DELETE(buf);
#   endif
#endif
		if (_levelDataDocument->HasParseError()|| checkLevelDataVersion()) {
			return openUserDataReturnType::FileExistError;
		}
		return openUserDataReturnType::FileExistCorrect;
	}
	else{
		return openUserDataReturnType::FileNotExist;
	}
}

openUserDataReturnType UserData::openSurvivalData()
{
	// 如果有这个存档
	if (_fileUtils->isFileExist(getSurvivalDataFileName()))
	{
#ifdef DEBUG
		_levelData = _fileUtils->getStringFromFile(getSurvivalDataFileName());
		_levelDataDocument->Parse<rapidjson::kParseDefaultFlags>(_levelData.c_str());
#else
#   ifndef DLLTEST
		_levelData = _fileUtils->getStringFromFile(getSurvivalDataFileName());
		_levelDataDocument->Parse<rapidjson::kParseDefaultFlags>(decryption(_levelData).c_str());
#   else
		_levelData = _fileUtils->getStringFromFile(getSurvivalDataFileName());
		char* buf = new char[_levelData.length()];
		if (decryption(_levelData.c_str(), buf)) {
			_levelDataDocument->Parse<rapidjson::kParseDefaultFlags>(buf);
		}
		else {
			CC_SAFE_DELETE(buf);
			return openUserDataReturnType::FileExistError;
		}
		CC_SAFE_DELETE(buf);
#   endif
#endif
		if (_levelDataDocument->HasParseError()) {
			return openUserDataReturnType::FileExistError;
		}
		return openUserDataReturnType::FileExistCorrect;
	}
	else {
		return openUserDataReturnType::FileNotExist;
	}
}

bool UserData::isHaveMember(char* key)
{
	if ((*_userDataDocument)["UserData"].HasMember(key))
		return true;
	return false;
}

string UserData::getUserDataFileName()
{
	return _fileUtils->getWritablePath() + "UserDataFile_" + to_string(_global->userInformation->getUserCaveFileNumber()) + ".dat";
}

string UserData::getLevelDataFileName()
{
	return _fileUtils->getWritablePath() + "UserDataFile_" + to_string(_global->userInformation->getUserCaveFileNumber()) + ".sav";
}

string UserData::getSurvivalDataFileName()
{
	return _fileUtils->getWritablePath() + "UserSurvivalDataFile_" + to_string(_global->userInformation->getUserCaveFileNumber()) + ".sav";
}

void UserData::caveUserData(char* key, double value)
{
	switch (openUserData())
	{
	case openUserDataReturnType::FileExistCorrect:
		if (isHaveMember(key))
			(*_userDataDocument)["UserData"][key].SetDouble(value);
		else
			(*_userDataDocument)["UserData"].AddMember(rapidjson::StringRef(key), value, _userDataDocument->GetAllocator());
		break;
	case openUserDataReturnType::FileNotExist:
		(*_userDataDocument)["UserData"].AddMember(rapidjson::StringRef(key), value, _userDataDocument->GetAllocator());
		break;
	case openUserDataReturnType::FileExistError:
		remove(getUserDataFileName().c_str());
		break;
	}
	flushUserData();
}

void UserData::caveUserData(char* key, bool value)
{
	switch (openUserData())
	{
	case openUserDataReturnType::FileExistCorrect:
		if (isHaveMember(key))
			(*_userDataDocument)["UserData"][key].SetBool(value);
		else
			(*_userDataDocument)["UserData"].AddMember(rapidjson::StringRef(key), value, _userDataDocument->GetAllocator());
		break;
	case openUserDataReturnType::FileNotExist:
		(*_userDataDocument)["UserData"].AddMember(rapidjson::StringRef(key), value, _userDataDocument->GetAllocator());
		break;
	case openUserDataReturnType::FileExistError:
		remove(getUserDataFileName().c_str());
		break;
	}
	flushUserData();
}

void UserData::caveUserData(char* key, char* value)
{
	switch (openUserData())
	{
	case openUserDataReturnType::FileExistCorrect:
		if (isHaveMember(key))
			(*_userDataDocument)["UserData"][key].SetString(rapidjson::StringRef(value));
		else
			(*_userDataDocument)["UserData"].AddMember(rapidjson::StringRef(key), rapidjson::StringRef(value), _userDataDocument->GetAllocator());
		break;
	case openUserDataReturnType::FileNotExist:
		(*_userDataDocument)["UserData"].AddMember(rapidjson::StringRef(key), rapidjson::StringRef(value), _userDataDocument->GetAllocator());
		break;
	case openUserDataReturnType::FileExistError:
		remove(getUserDataFileName().c_str());
		break;
	}
	flushUserData();
}

void UserData::caveUserData(char* key, int value)
{
	switch (openUserData())
	{
	case openUserDataReturnType::FileExistCorrect:
		if (isHaveMember(key))
			(*_userDataDocument)["UserData"][key].SetInt(value);
		else
			(*_userDataDocument)["UserData"].AddMember(rapidjson::StringRef(key), value, _userDataDocument->GetAllocator());
		break;
	case openUserDataReturnType::FileNotExist:
		(*_userDataDocument)["UserData"].AddMember(rapidjson::StringRef(key), value, _userDataDocument->GetAllocator());
		break;
	case openUserDataReturnType::FileExistError:
		remove(getUserDataFileName().c_str());
		break;
	}
	flushUserData();
}

int UserData::openIntUserData(char* key)
{
	switch (openUserData())
	{
	case openUserDataReturnType::FileExistCorrect:
		if (isHaveMember(key))
			return (*_userDataDocument)["UserData"][key].GetInt();
		break;
	case openUserDataReturnType::FileExistError:
		remove(getUserDataFileName().c_str());
		break;
	default: break;
	}
	return 0;
}

double UserData::openDoubleUserData(char* key)
{
	switch (openUserData())
	{
	case openUserDataReturnType::FileExistCorrect:
		if (isHaveMember(key))
			return (*_userDataDocument)["UserData"][key].GetDouble();
		break;
	case openUserDataReturnType::FileExistError:
		remove(getUserDataFileName().c_str());
		break;
	default: break;
	}
	return 0.0;
}

bool UserData::openBoolUserData(char* key)
{
	switch (openUserData())
	{
	case openUserDataReturnType::FileExistCorrect:
		if (isHaveMember(key))
			return (*_userDataDocument)["UserData"][key].GetBool();
		break;
	case openUserDataReturnType::FileExistError:
		remove(getUserDataFileName().c_str());
		break;
	default: break;
	}
	return false;
}

const char* UserData::openStringUserData(char* key)
{
	switch (openUserData())
	{
	case openUserDataReturnType::FileExistCorrect:
		if (isHaveMember(key))
			return (*_userDataDocument)["UserData"][key].GetString();
		break;
	case openUserDataReturnType::FileExistError:
		remove(getUserDataFileName().c_str());
		break;
	default: break;
	}
	return nullptr;
}

void UserData::createNewUserDataDocument()
{
	if (_userDataDocument)
	{
		delete _userDataDocument;
		_userDataDocument = nullptr;
	}
	_userDataDocument = new Document();
	_userData.clear();
	_isAnewReadData = false;
}

void UserData::createNewLevelDataDocument()
{
	if (_levelDataDocument)
	{
		delete _levelDataDocument;
		_levelDataDocument = nullptr;
	}
	_levelDataDocument = new Document();
	_levelData.clear();
}

void UserData::setAnewReadData(const bool newRead)
{
	_isAnewReadData = newRead;
}

void UserData::caveLevelData(char* key)
{
	switch (openLevelData())
	{
	case openUserDataReturnType::FileExistCorrect:
		if ((*_levelDataDocument).HasMember(key))
			(*_levelDataDocument).RemoveMember(key);
		break;
	case openUserDataReturnType::FileExistError:
		remove(getLevelDataFileName().c_str());
		return;
		break;
	}

	if (!_levelDataDocument->IsObject())_levelDataDocument->SetObject();

	rapidjson::Value object(rapidjson::kObjectType);
	_levelDataDocument->AddMember(rapidjson::StringRef(key), object, _levelDataDocument->GetAllocator());

	caveLevelDataVersion();
	caveLevelPlantsData(key);
	caveLevelZombiesData(key);
	caveLevelSelectPlantsData(key);
	caveLevelOtherData(key);
	caveLevelSunData(key);
	caveLevelCoinData(key);
	caveLevelCarData(key);
	caveLevelBulletData(key);

	flushLevelData();
}

void UserData::caveSurvivalData(char* key)
{
	switch (openSurvivalData())
	{
	case openUserDataReturnType::FileExistCorrect:
		if ((*_levelDataDocument).HasMember(key))
			(*_levelDataDocument).RemoveMember(key);
		break;
	case openUserDataReturnType::FileExistError:
		return;
		break;
	}

	if (!_levelDataDocument->IsObject())_levelDataDocument->SetObject();

	rapidjson::Value object(rapidjson::kObjectType);
	_levelDataDocument->AddMember(rapidjson::StringRef(key), object, _levelDataDocument->GetAllocator());

	caveLevelPlantsData(key);
	caveLevelZombiesData(key);
	caveLevelSelectPlantsData(key);
	caveLevelOtherData(key);
	caveLevelSunData(key);
	caveLevelCarData(key);
	caveLevelBulletData(key);

	flushSurvivalData();
}

bool UserData::checkLevelDataVersion()
{
	return _levelDataDocument->HasMember("LevelDataVersion") ? _levelDataVersion == (*_levelDataDocument)["LevelDataVersion"].GetInt() ? false : true : true;
}

void UserData::caveLevelDataVersion()
{
	if (!_levelDataDocument->HasMember("LevelDataVersion"))
	{
		_levelDataDocument->AddMember(rapidjson::StringRef("LevelDataVersion"), _levelDataVersion, _levelDataDocument->GetAllocator());
	}
	else
	{
		if (_levelDataVersion != (*_levelDataDocument)["LevelDataVersion"].GetInt())
		{
			_levelDataDocument->RemoveMember("LevelDataVersion");
			_levelDataDocument->AddMember(rapidjson::StringRef("LevelDataVersion"), _levelDataVersion, _levelDataDocument->GetAllocator());
		}
	}
	
}

void UserData::caveLevelPlantsData(char* key)
{
	unsigned int plantsNumber = 0;
	rapidjson::Value _object(rapidjson::kObjectType);
	rapidjson::Document::AllocatorType& allocator = _levelDataDocument->GetAllocator();
	(*_levelDataDocument)[key].AddMember("Plants", _object, allocator);
	
	for (auto &plant : PlantsGroup)
	{
		rapidjson::Value object(rapidjson::kObjectType);

		if (plant.second->getPlantAnimation()&& plant.second->getPlantAnimation()->isVisible())
		{
			object.AddMember("PlantsTag", plant.second->getPlantTag(), allocator);
			object.AddMember("PlantsHealthPoint", plant.second->getPlantHealthPoint(), allocator);
			object.AddMember("PlantsAnimationTime", plant.second->getPlantAnimation()->getCurrent()->animationLast, allocator);
			object.AddMember("PlantsAnimationTimeScale", plant.second->getPlantAnimation()->getTimeScale(), allocator);
			object.AddMember("PlantsPositionX", plant.second->getPlantAnimation()->getPositionX(), allocator);
			object.AddMember("PlantsPositionY", plant.second->getPlantAnimation()->getPositionY(), allocator);
			object.AddMember("PlantsRow", plant.second->getPlantRow(), allocator);
			object.AddMember("PlantsColumn", plant.second->getPlantColumn(), allocator);
			object.AddMember("PlantsLocalZOrder", plant.second->getPlantAnimation()->getLocalZOrder(), allocator);
			object.AddMember("PlantsType", static_cast<int>(plant.second->getPlantType()), allocator);
			
			caveLevelPlantsAnimationData(plant.second, object);

			plant.second->cavePlantInformation(object, allocator);

			(*_levelDataDocument)[key]["Plants"].AddMember(numberToString(++plantsNumber, allocator), object, _levelDataDocument->GetAllocator());
		}
	}

	(*_levelDataDocument)[key]["Plants"].AddMember("PlantsNumber", plantsNumber, allocator);
}

void UserData::caveLevelPlantsAnimationData(Plants* plant, rapidjson::Value& object)
{
	rapidjson::Document::AllocatorType& allocator = _levelDataDocument->GetAllocator();

	auto spTE = plant->getPlantAnimation()->getCurrent();
	int animationNumber = 0;
	do
	{
		rapidjson::Value objectPA(rapidjson::kObjectType);
		objectPA.AddMember("PlantsAnimationName", rapidjson::StringRef(spTE->animation->name), allocator);
		objectPA.AddMember("PlantsAnimationLoop", spTE->loop, allocator);

		object.AddMember(numberToString(++animationNumber, allocator), objectPA, allocator);
		spTE = spTE->next;
	} while (spTE);
	object.AddMember("AnimationNumber", animationNumber, allocator);
}

void UserData::caveLevelZombiesData(char* key)
{
	unsigned int zombiesNumber = 0;
	rapidjson::Value _object(rapidjson::kObjectType);
	rapidjson::Document::AllocatorType& allocator = _levelDataDocument->GetAllocator();
	(*_levelDataDocument)[key].AddMember("Zombies", _object, allocator);

	for (auto zombie : ZombiesGroup)
	{
		if (zombie->getZombieAnimation()&& zombie->getZombieAnimation()->isVisible())
		{
			rapidjson::Value object(rapidjson::kObjectType);

			object.AddMember("ZombieCurrentBloodVolume", zombie->getZombieCurrentBloodVolume(), allocator);
			object.AddMember("ZombieCurrentBodyShieldVolume", zombie->getZombieCurrentBodyShieldVolume(), allocator);
			object.AddMember("ZombieCurrentHeadShieldVolume", zombie->getZombieCurrentHeadShieldVolume(), allocator);
			object.AddMember("ZombieAnimationTime", zombie->getZombieAnimation()->getCurrent()->animationLast, allocator);
			object.AddMember("ZombieAnimationTimeScale", zombie->getZombieAnimation()->getTimeScale(), allocator);
			object.AddMember("ZombiePositionX", zombie->getZombieAnimation()->getPositionX(), allocator);
			object.AddMember("ZombiePositionY", zombie->getZombieAnimation()->getPositionY(), allocator);
			object.AddMember("ZombieCurrentSpeed", zombie->getZombieCurrentSpeed(), allocator);
			object.AddMember("ZombieTimerTimeSlow", zombie->getZombieTimerTime(true), allocator);
			object.AddMember("ZombieTimerTimeStop", zombie->getZombieTimerTime(), allocator);
			object.AddMember("ZombieInRow", zombie->getZombieInRow(), allocator);
			object.AddMember("ZombieIsFrozen", zombie->getZombieIsFrozen(), allocator);
			object.AddMember("ZombieLocalZOrder", zombie->getZombieAnimation()->getLocalZOrder(), allocator);
			object.AddMember("ZombieType", static_cast<int>(zombie->getZombieType()), allocator);
			object.AddMember("ZombieOpacity", static_cast<int>(zombie->getZombieAnimation()->getOpacity()), allocator);
			
			caveLevelZombiesAnimationData(zombie, object);

			(*_levelDataDocument)[key]["Zombies"].AddMember(numberToString(++zombiesNumber, allocator), object, _levelDataDocument->GetAllocator());
		}
	}

	(*_levelDataDocument)[key]["Zombies"].AddMember("ZombiesNumber", zombiesNumber, allocator);
}

void UserData::caveLevelZombiesAnimationData(Zombies* zombie, rapidjson::Value& object)
{
	rapidjson::Document::AllocatorType& allocator = _levelDataDocument->GetAllocator();

	auto spTE = zombie->getZombieAnimation()->getCurrent();
	int animationNumber = 0;
	do
	{
		rapidjson::Value objectPA(rapidjson::kObjectType);
		objectPA.AddMember("ZombieAnimationName", rapidjson::StringRef(spTE->animation->name), allocator);
		objectPA.AddMember("ZombieAnimationLoop", spTE->loop, allocator);

		object.AddMember(numberToString(++animationNumber, allocator), objectPA, allocator);
		spTE = spTE->next;
	} while (spTE);
	object.AddMember("AnimationNumber", animationNumber, allocator);
}

void UserData::caveLevelSunData(char* key)
{
	unsigned int sunsNumber = 0;
	rapidjson::Value _object(rapidjson::kObjectType);
	rapidjson::Document::AllocatorType& allocator = _levelDataDocument->GetAllocator();
	(*_levelDataDocument)[key].AddMember("Sun", _object, allocator);

	for (auto sun : SunsGroup)
	{
		if (sun->getSun() && sun->getSun()->isVisible() && sun->getEnable())
		{
			rapidjson::Value object(rapidjson::kObjectType);

			object.AddMember("SunTag", sun->getSunTag(), allocator);
			object.AddMember("PositionX", sun->getSun()->getPositionX(), allocator);
			object.AddMember("PositionY", sun->getSun()->getPositionY(), allocator);

			(*_levelDataDocument)[key]["Sun"].AddMember(numberToString(++sunsNumber, allocator), object, _levelDataDocument->GetAllocator());
		}
	}
	(*_levelDataDocument)[key]["Sun"].AddMember("SunNumbers", sunsNumber, allocator);
}

void UserData::caveLevelCoinData(char* key)
{
	unsigned int coinNumber = 0;
	rapidjson::Value _object(rapidjson::kObjectType);
	rapidjson::Document::AllocatorType& allocator = _levelDataDocument->GetAllocator();
	(*_levelDataDocument)[key].AddMember("Coin", _object, allocator);

	for (auto coin : CoinsGroup)
	{
		if (coin->getCoin() && coin->getCoin()->isVisible() && coin->getEnable())
		{
			rapidjson::Value object(rapidjson::kObjectType);

			object.AddMember("PositionX", coin->getCoin()->getPositionX(), allocator);
			object.AddMember("PositionY", coin->getCoin()->getPositionY(), allocator);
			object.AddMember("LocalZOrder", coin->getCoin()->getLocalZOrder(), allocator);

			(*_levelDataDocument)[key]["Coin"].AddMember(numberToString(++coinNumber, allocator), object, _levelDataDocument->GetAllocator());
		}
	}
	(*_levelDataDocument)[key]["Coin"].AddMember("CoinNumbers", coinNumber, allocator);
}

void UserData::caveLevelCarData(char* key)
{
	unsigned int carsNumber = 0;
	rapidjson::Value _object(rapidjson::kObjectType);
	rapidjson::Document::AllocatorType& allocator = _levelDataDocument->GetAllocator();
	(*_levelDataDocument)[key].AddMember("Car", _object, allocator);
	(*_levelDataDocument)[key]["Car"].AddMember("CarNumbers", CarsGroup.size(), allocator);

	for (auto car : CarsGroup)
	{
		rapidjson::Value object(rapidjson::kObjectType);

		object.AddMember("Live", car->getLive(), allocator);
		object.AddMember("PositionX", car->getCar()->getPositionX(), allocator);
		object.AddMember("PositionY", car->getCar()->getPositionY(), allocator);
		object.AddMember("CarRow", car->getInRow(), allocator);
		object.AddMember("CarType", static_cast<int>(car->getCarType()), allocator);
		
		(*_levelDataDocument)[key]["Car"].AddMember(numberToString(++carsNumber, allocator), object, _levelDataDocument->GetAllocator());
	}
}

void UserData::caveLevelBulletData(char* key)
{
	unsigned int bulletNumber = 0;
	rapidjson::Value _object(rapidjson::kObjectType);
	rapidjson::Document::AllocatorType& allocator = _levelDataDocument->GetAllocator();
	(*_levelDataDocument)[key].AddMember("Bullet", _object, allocator);

	for (auto bullet : BulletGroup)
	{
		if (!bullet->getBulletIsUsed() && bullet->getBulletVisible() && bullet->getBullet()->getOpacity() >= 255 &&
			bullet->getBulletPosition().x < controlLayerInformation->gameMapInformation->mapRight + 300)
		{
			rapidjson::Value object(rapidjson::kObjectType);

			object.AddMember("BulletType", static_cast<int>(bullet->getBulletType()), allocator);
			object.AddMember("PositionX", bullet->getBullet()->getPositionX(), allocator);
			object.AddMember("PositionY", bullet->getBullet()->getPositionY(), allocator);
			//object.AddMember("BulletOpcity", static_cast<int>(bullet->getBullet()->getOpacity()), allocator);
			object.AddMember("BulletAnimationTime", bullet->getBullet()->getCurrent()->animationLast, allocator);
			object.AddMember("BulletAnimationTimeScale", bullet->getBullet()->getTimeScale(), allocator);
			object.AddMember("bulletInRow", bullet->getBulletInRow(), allocator);
			object.AddMember("LocalZOrder", bullet->getBullet()->getLocalZOrder(), allocator);

			caveLevelBulletAnimationData(bullet, object);

			bullet->caveBulletInformation(object, allocator);

			(*_levelDataDocument)[key]["Bullet"].AddMember(numberToString(++bulletNumber, allocator), object, _levelDataDocument->GetAllocator());
		}
	}
	(*_levelDataDocument)[key]["Bullet"].AddMember("BulletNumbers", bulletNumber, allocator);
}

void UserData::caveLevelBulletAnimationData(Bullet* bullet, rapidjson::Value& object)
{
	rapidjson::Document::AllocatorType& allocator = _levelDataDocument->GetAllocator();

	auto spTE = bullet->getBullet()->getCurrent();
	int animationNumber = 0;
	do
	{
		rapidjson::Value objectPA(rapidjson::kObjectType);
		objectPA.AddMember("BulletAnimationName", rapidjson::StringRef(spTE->animation->name), allocator);
		objectPA.AddMember("BulletAnimationLoop", spTE->loop, allocator);

		object.AddMember(numberToString(++animationNumber, allocator), objectPA, allocator);
		spTE = spTE->next;
	} while (spTE);
	object.AddMember("AnimationNumber", animationNumber, allocator);
}

void UserData::caveLevelSelectPlantsData(char* key)
{
	unsigned int plantsNumber = 0;
	rapidjson::Value _object(rapidjson::kObjectType);
	rapidjson::Document::AllocatorType& allocator = _levelDataDocument->GetAllocator();
	(*_levelDataDocument)[key].AddMember("SelectPlants", _object, allocator);
	(*_levelDataDocument)[key]["SelectPlants"].AddMember("SelectNumber", _global->userInformation->getUserSelectCrads().size(), allocator);

	for (auto& card : _global->userInformation->getUserSelectCrads())
	{
		rapidjson::Value object(rapidjson::kObjectType);

		object.AddMember("CardTag", card.cardTag, allocator);
		object.AddMember("Percent", buttonLayerInformation->plantsCards[card.cardTag].progressTimer->getPercentage(), allocator);
		object.AddMember("LastTime", buttonLayerInformation->plantsCards[card.cardTag].progressTimer->getPercentage() / 100.f * plantsCardInformation[card.cardTag].plantsCoolTime, allocator);

		(*_levelDataDocument)[key]["SelectPlants"].AddMember(numberToString(++plantsNumber, allocator), object, _levelDataDocument->GetAllocator());
	}
}

void UserData::caveLevelOtherData(char* key)
{
	rapidjson::Document::AllocatorType& allocator = _levelDataDocument->GetAllocator();

	rapidjson::Value object(rapidjson::kObjectType);

	object.AddMember("SunNumbers", _global->userInformation->getSunNumbers(), allocator);
	object.AddMember("SurSunNumbers", informationLayerInformation->gameType->getSunNumberRequriement()->allSunNumbers, allocator);
	object.AddMember("SurPlusPlantsNumbers", informationLayerInformation->gameType->getPlantsRequriement()->surPlusPlantsNumbers, allocator);
	object.AddMember("ZombiesAppearFrequency", controlLayerInformation->_zombiesAppearControl->getZombiesAppearFrequency(), allocator);
	object.AddMember("ZombiesAppearTime", controlLayerInformation->_zombiesAppearControl->getTime(), allocator);
	object.AddMember("IsBegin", controlLayerInformation->_zombiesAppearControl->getIsBegin(), allocator);
	object.AddMember("IsShowWords", controlLayerInformation->_zombiesAppearControl->getIsShowWords(), allocator);
	object.AddMember("LastFrequencyZombiesWasDeath", controlLayerInformation->_zombiesAppearControl->getLastFrequencyZombiesWasDeath(), allocator);
	object.AddMember("ProgressBarPercent", informationLayerInformation->getProgressBarPercent(), allocator);
	object.AddMember("ProgressBarLastPercent", informationLayerInformation->getProgressBarLastPercent(), allocator);

	if (BigMapGameScene::scrollView)
	{
		object.AddMember("BigMapOffsetX", BigMapGameScene::scrollView->getContentOffset().x, allocator);
		object.AddMember("BigMapOffsetY", BigMapGameScene::scrollView->getContentOffset().y, allocator);
	}

	(*_levelDataDocument)[key].AddMember("OtherData", object, _levelDataDocument->GetAllocator());
}

void UserData::caveSurvivalOtherData(char* key)
{
	rapidjson::Document::AllocatorType& allocator = _levelDataDocument->GetAllocator();

	rapidjson::Value object(rapidjson::kObjectType);

	object.AddMember("SunNumbers", _global->userInformation->getSunNumbers(), allocator);

	(*_levelDataDocument)[key].AddMember("OtherData", object, _levelDataDocument->GetAllocator());
}

bool UserData::readLevelData()
{
	switch (openLevelData())
	{
	case openUserDataReturnType::FileExistCorrect:
		return true;
		break;
	case openUserDataReturnType::FileExistError:
		remove(getLevelDataFileName().c_str());
		return false;
	default:
		return false;
		break;
	}
}

void UserData::openLevelData(char* key)
{
	if (readLevelData())
	{
		openLevelPlantsData(key);
		openLevelZombiesData(key);
		openLevelOtherData(key);
		openLevelSunData(key);
		openLevelCoinData(key);
		openLevelCarData(key);
		openLevelBulletData(key);
	}
	else
	{
		replaceScene();
	}
}

bool UserData::isHaveLevelData(char* key)
{
	switch (openLevelData())
	{
	case openUserDataReturnType::FileExistCorrect:
		if ((*_levelDataDocument).HasMember(key))
			return true;
		break;
	case openUserDataReturnType::FileExistError:
		remove(getLevelDataFileName().c_str());
		break;
	default: break;
	}
	return false;
}

void UserData::openSurvivalData(char* key)
{
	switch (openSurvivalData())
	{
	case openUserDataReturnType::FileExistCorrect:
		openLevelPlantsData(key);
		openLevelZombiesData(key);
		openLevelOtherData(key);
		openLevelSunData(key);
		openLevelCarData(key);
		openLevelBulletData(key);
		break;
	case openUserDataReturnType::FileExistError:
		remove(getSurvivalDataFileName().c_str());
		break;
	}
}

bool UserData::isHaveSurvivalData(char* key)
{
	switch (openSurvivalData())
	{
	case openUserDataReturnType::FileExistCorrect:
		if ((*_levelDataDocument).HasMember(key))
			return true;
		break;
	case openUserDataReturnType::FileExistError:
		remove(getSurvivalDataFileName().c_str());
		break;
	default:break;
	}
	return false;
}

void UserData::openLevelPlantsData(char* key)
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
		plants->createPlantAnimation();

		openLevelPlantsAnimationData(key, to_string(i).c_str(), plants);
		plants->getPlantAnimation()->update((*_levelDataDocument)[key]["Plants"][to_string(i).c_str()]["PlantsAnimationTime"].GetFloat());
		plants->getPlantAnimation()->setTimeScale((*_levelDataDocument)[key]["Plants"][to_string(i).c_str()]["PlantsAnimationTimeScale"].GetFloat());
		plants->setPlantHealthPoint((*_levelDataDocument)[key]["Plants"][to_string(i).c_str()]["PlantsHealthPoint"].GetFloat());
		plants->getPlantAnimation()->getChildByName("SplashOfSoil")->setOpacity(0);
		plants->getPlantAnimation()->getChildByName("BufEffect")->setOpacity(0);

		PlantsGroup.insert(pair<int, Plants*>((*_levelDataDocument)[key]["Plants"][to_string(i).c_str()]["PlantsTag"].GetInt(), plants));
		
		controlLayerInformation->gameMapInformation->plantsMap[plants->getPlantRow()][plants->getPlantColumn()] = static_cast<unsigned int>(type);/* 地图记录种植的植物 */
	}
}

void UserData::openLevelPlantsAnimationData(char* key, const char* pl, Plants* plant)
{
	plant->getPlantAnimation()->setTimeScale(1.f);

	const int animationNumber = (*_levelDataDocument)[key]["Plants"][pl]["AnimationNumber"].GetInt();
	for (int i = 1; i <= animationNumber; ++i)
	{
		auto name = (*_levelDataDocument)[key]["Plants"][pl][to_string(i).c_str()]["PlantsAnimationName"].GetString();
		auto loop = static_cast<bool>((*_levelDataDocument)[key]["Plants"][pl][to_string(i).c_str()]["PlantsAnimationLoop"].GetInt());

		i == 1 ? plant->getPlantAnimation()->setAnimation(0, name, loop) : plant->getPlantAnimation()->addAnimation(0, name, loop);
	}
}

void UserData::openLevelZombiesData(char* key)
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
		zombies->setZombieAttributeForGameType();

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

void UserData::openLevelZombiesAnimationData(char* key, const char* zo, Zombies* zombie)
{
	zombie->getZombieAnimation()->setTimeScale(1.f);

	const int animationNumber = (*_levelDataDocument)[key]["Zombies"][zo]["AnimationNumber"].GetInt();
	for (int i = 1; i <= animationNumber; ++i)
	{
		auto name = (*_levelDataDocument)[key]["Zombies"][zo][to_string(i).c_str()]["ZombieAnimationName"].GetString();
		auto loop = static_cast<bool>((*_levelDataDocument)[key]["Zombies"][zo][to_string(i).c_str()]["ZombieAnimationLoop"].GetInt());

		i == 1 ? zombie->getZombieAnimation()->setAnimation(0, name, loop), zombie->setZombieAnimationName(name) : zombie->getZombieAnimation()->addAnimation(0, name, loop);
	}
}

void UserData::openLevelSelectCardData(char* key)
{
	if (readLevelData())
	{
		UserSelectCard card{};
		vector<UserSelectCard> userSelectCard;
		const auto selectNumbers = (*_levelDataDocument)[key]["SelectPlants"]["SelectNumber"].GetInt();
		for (int i = 1; i <= selectNumbers; ++i)
		{
			card.cardTag = (*_levelDataDocument)[key]["SelectPlants"][to_string(i).c_str()]["CardTag"].GetInt();
			userSelectCard.push_back(card);
			plantsCardInformation[card.cardTag].PlantsSurPlusPrecent = (*_levelDataDocument)[key]["SelectPlants"][to_string(i).c_str()]["Percent"].GetFloat();
			plantsCardInformation[card.cardTag].PlantsSurPlusCoolTime = (*_levelDataDocument)[key]["SelectPlants"][to_string(i).c_str()]["LastTime"].GetFloat();
		}
		_global->userInformation->setUserSelectCrads(userSelectCard);
		_global->userInformation->setIsReadFileData(true);
	}
	else
	{
		replaceScene();
	}
}

void UserData::openLevelSunData(char* key)
{
	const auto sunNumbers = (*_levelDataDocument)[key]["Sun"]["SunNumbers"].GetInt();
	for (int i = 1; i <= sunNumbers; ++i)
	{
		auto sun = new Sun(goodsLayerInformation);
		sun->setSunTag((*_levelDataDocument)[key]["Sun"][to_string(i).c_str()]["SunTag"].GetInt());
		sun->setPosition(Vec2(
			(*_levelDataDocument)[key]["Sun"][to_string(i).c_str()]["PositionX"].GetFloat(),
			(*_levelDataDocument)[key]["Sun"][to_string(i).c_str()]["PositionY"].GetFloat()));
		sun->createSuns();

		SunsGroup.push_back(sun);
	}
}

void UserData::openLevelCoinData(char* key)
{
	const auto coinNumbers = (*_levelDataDocument)[key]["Coin"]["CoinNumbers"].GetInt();
	for (int i = 1; i <= coinNumbers; ++i)
	{
		auto coin = new Coin(goodsLayerInformation);
		coin->setPosition(Vec2(
			(*_levelDataDocument)[key]["Coin"][to_string(i).c_str()]["PositionX"].GetFloat(),
			(*_levelDataDocument)[key]["Coin"][to_string(i).c_str()]["PositionY"].GetFloat()));
		coin->setCoinLocalZOrder((*_levelDataDocument)[key]["Coin"][to_string(i).c_str()]["LocalZOrder"].GetInt());
		coin->createCoin();

		CoinsGroup.push_back(coin);
	}
}

void UserData::openLevelCarData(char* key)
{
	const auto carNumbers = (*_levelDataDocument)[key]["Car"]["CarNumbers"].GetInt();
	for (int i = 1; i <= carNumbers; ++i)
	{
		auto car = new Car(animationLayerInformation->getAnimationLayer());
		car->setLive((*_levelDataDocument)[key]["Car"][to_string(i).c_str()]["Live"].GetBool());
		car->setInRow((*_levelDataDocument)[key]["Car"][to_string(i).c_str()]["CarRow"].GetInt());
		car->setPosition(Vec2(
			(*_levelDataDocument)[key]["Car"][to_string(i).c_str()]["PositionX"].GetFloat(),
			(*_levelDataDocument)[key]["Car"][to_string(i).c_str()]["PositionY"].GetFloat()));
		car->showCar(static_cast<CarType>((*_levelDataDocument)[key]["Car"][to_string(i).c_str()]["CarType"].GetInt()));
		car->getCar()->setScale(0.8f);
		car->getCar()->getChildByName("fog")->setOpacity(0);
		CarsGroup.push_back(car);
	}
}

void UserData::openLevelBulletData(char* key)
{
	const auto bulletNumbers = (*_levelDataDocument)[key]["Bullet"]["BulletNumbers"].GetInt();
	for (int i = 1; i <= bulletNumbers; ++i)
	{
		auto type = static_cast<BulletType>((*_levelDataDocument)[key]["Bullet"][to_string(i).c_str()]["BulletType"].GetInt());
		auto bullet = GSAnimationLayer::createDifferentBullet(type, animationLayerInformation);
		bullet->readBulletInformation(_levelDataDocument, key, i);
		bullet->setBulletInRow((*_levelDataDocument)[key]["Bullet"][to_string(i).c_str()]["bulletInRow"].GetInt());
		bullet->createBullet();

		openLevelBulletAnimationData(key, to_string(i).c_str(), bullet);

		bullet->getBullet()->setLocalZOrder((*_levelDataDocument)[key]["Bullet"][to_string(i).c_str()]["LocalZOrder"].GetInt());
		//bullet->getBullet()->setOpacity((*_levelDataDocument)[key]["Bullet"][to_string(i).c_str()]["BulletOpcity"].GetInt());
		bullet->getBullet()->update((*_levelDataDocument)[key]["Bullet"][to_string(i).c_str()]["BulletAnimationTime"].GetFloat());
		bullet->getBullet()->setTimeScale((*_levelDataDocument)[key]["Bullet"][to_string(i).c_str()]["BulletAnimationTimeScale"].GetFloat());
		bullet->readBulletAnimationInformation(_levelDataDocument, key, i);

		BulletGroup.push_back(bullet);
	}
}

void UserData::openLevelBulletAnimationData(char* key, const char* bu, Bullet* bullet)
{
	bullet->getBullet()->setTimeScale(1.f);

	const int animationNumber = (*_levelDataDocument)[key]["Bullet"][bu]["AnimationNumber"].GetInt();
	for (int i = 1; i <= animationNumber; ++i)
	{
		auto name = (*_levelDataDocument)[key]["Bullet"][bu][to_string(i).c_str()]["BulletAnimationName"].GetString();
		auto loop = static_cast<bool>((*_levelDataDocument)[key]["Bullet"][bu][to_string(i).c_str()]["BulletAnimationLoop"].GetInt());

		i == 1 ? bullet->getBullet()->setAnimation(0, name, loop), bullet->getBullet()->setAnimation(0, name, loop) : bullet->getBullet()->addAnimation(0, name, loop);
	}
}

void UserData::openLevelOtherData(char* key)
{
	const auto number = (*_levelDataDocument)[key]["OtherData"]["ZombiesAppearFrequency"].GetInt();
	controlLayerInformation->_zombiesAppearControl->setZombiesAppearFrequency(number);

	informationLayerInformation->setProgressBarPercent(
		(*_levelDataDocument)[key]["OtherData"]["ProgressBarPercent"].GetFloat());

	informationLayerInformation->setProgressBarLastPercent(
		(*_levelDataDocument)[key]["OtherData"]["ProgressBarLastPercent"].GetFloat());

	controlLayerInformation->_zombiesAppearControl->setTimeAdd(
		(*_levelDataDocument)[key]["OtherData"]["ZombiesAppearTime"].GetDouble());

	controlLayerInformation->_zombiesAppearControl->setIsBegin(
		(*_levelDataDocument)[key]["OtherData"]["IsBegin"].GetBool());

	controlLayerInformation->_zombiesAppearControl->setIsShowWords(
		(*_levelDataDocument)[key]["OtherData"]["IsShowWords"].GetBool());

	controlLayerInformation->_zombiesAppearControl->setLastFrequencyZombiesWasDeath(
		(*_levelDataDocument)[key]["OtherData"]["LastFrequencyZombiesWasDeath"].GetBool());

	controlLayerInformation->updateFlag();

	informationLayerInformation->updateProgressBarHead();

	_global->userInformation->setSunNumbers(
		(*_levelDataDocument)[key]["OtherData"]["SunNumbers"].GetInt());
	informationLayerInformation->updateSunNumbers();

	informationLayerInformation->gameType->getSunNumberRequriement()->allSunNumbers =
		(*_levelDataDocument)[key]["OtherData"]["SurSunNumbers"].GetInt();
	informationLayerInformation->gameType->getPlantsRequriement()->surPlusPlantsNumbers =
		(*_levelDataDocument)[key]["OtherData"]["SurPlusPlantsNumbers"].GetInt();
	informationLayerInformation->gameType->updateRequirementNumbers();

	if (BigMapGameScene::scrollView)
	{
		BigMapGameScene::scrollView->setContentOffset(Vec2(
			(*_levelDataDocument)[key]["OtherData"]["BigMapOffsetX"].GetFloat(),
			(*_levelDataDocument)[key]["OtherData"]["BigMapOffsetY"].GetFloat()));
	}
}

void UserData::openSurvivalOtherData(char* key)
{
	_global->userInformation->setSunNumbers(
		(*_levelDataDocument)[key]["OtherData"]["SunNumbers"].GetInt());
	informationLayerInformation->updateSunNumbers();
}

void UserData::removeLevelData(char* key)
{
	switch (openLevelData())
	{
	case openUserDataReturnType::FileExistCorrect:
		if ((*_levelDataDocument).HasMember(key))
			(*_levelDataDocument).RemoveMember(key);
		break;
	default:
		return;
		break;
	}
	flushLevelData();
}

rapidjson::Value UserData::numberToString(int number, rapidjson::Document::AllocatorType& allocator)
{
	rapidjson::Value v(kStringType);
	auto str = to_string(number);
	v.SetString(str.c_str(), str.size(), allocator);
	return v;
}

rapidjson::Value UserData::numberAddString(int number, string sstr, rapidjson::Document::AllocatorType& allocator)
{
	rapidjson::Value v(kStringType);
	auto str = sstr + to_string(number);
	v.SetString(str.c_str(), str.size(), allocator);
	return v;
}

void UserData::replaceScene()
{
	Director::getInstance()->replaceScene(TransitionFade::create(1.0f, SelectPlantsScene::create()));
}

#ifndef DLLTEST
string UserData::encryption(string& str)
{
	char* encryptString, * encryptString1;
	base64Encode((unsigned char*)str.c_str(), (unsigned int)str.length(), &encryptString);

	string sss(encryptString);
	reverse(sss.begin(), sss.end());
	for (auto& s : sss)
	{
		if (s >= 'a' && s <= 'z')s = ((s - 'a') + 2) % 26 + 'a';
		if (s >= 'A' && s <= 'Z')s = ((s - 'A') + 5) % 26 + 'A';
		if (s >= '0' && s <= '9')s = ((s - '0') + 7) % 10 + '0';
		if (s == '=')s = '+';
	}

	base64Encode((unsigned char*)sss.c_str(), (unsigned int)sss.length(), &encryptString1);

	string s(encryptString1);

	CC_SAFE_FREE(encryptString);
	CC_SAFE_FREE(encryptString1);

	return s;
}

string UserData::decryption(string& str)
{
	unsigned char* decryptString = nullptr, * decryptString1 = nullptr;
	
	auto ret = base64Decode((unsigned char*)str.c_str(), (unsigned int)str.length(), &decryptString);
	
	if (ret > 0) decryptString[ret] = '\0';
	else return "";
	
	string sss(reinterpret_cast<char*>(decryptString));
	
	for (auto& s : sss)
	{
		if (s >= 'a' && s <= 'z')s = ((s - 'a') + 24) % 26 + 'a';
		if (s >= 'A' && s <= 'Z')s = ((s - 'A') + 21) % 26 + 'A';
		if (s >= '0' && s <= '9')s = ((s - '0') + 3) % 10 + '0';
		if (s == '+')s = '=';
	}
	reverse(sss.begin(), sss.end());

	ret = base64Decode((unsigned char*)sss.c_str(), (unsigned int)sss.length(), &decryptString1);

	if (ret > 0) decryptString1[ret] = '\0';
	else { CC_SAFE_FREE(decryptString); return ""; }

	string s(reinterpret_cast<char*>(decryptString1));

	CC_SAFE_FREE(decryptString);
	CC_SAFE_FREE(decryptString1);

	return s;
}
#endif // !DLLTEST