/**
 *Copyright (c) 2020 LZ.All Right Reserved
 *Author : LZ
 *Date: 2020.4.20
 *Email: 2117610943@qq.com
 */

#include "UserData.h"
#include "GlobalVariable.h"
#include "base/base64.h"
#include "Scenes/GameScene/GSData.h"
#include "Scenes/GameScene/GSAnimationLayer.h"
#include "Scenes/GameScene/GSButtonLayer.h"
#include "Scenes/GameScene/GSControlLayer.h"
#include "Scenes/GameScene/GSZombiesAppearControl.h"
#include "Scenes/GameScene/GSInformationLayer.h"
#include "Scenes/GameScene/GSBackgroundLayer.h"
#include "Scenes/SelectPlantsScene/SelectPlantsScene.h"
#include "Plants/Plants.h"
#include "Plants/Plants-Files.h"
#include "Zombies/Zombies.h"
#include "Car.h"
#include "Coin.h"
#include "Plants/EmissionPlants/Bullet/Bullet.h"
#include "Plants/EmissionPlants/Bullet/Bullet-File.h"

#ifdef DLLTEST
#include "libpvz.h"
#pragma comment (lib,"libpvz")
#endif // !DLLTEST

UserData* UserData::_instance = nullptr;

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
		if (_levelDataDocument->HasParseError()) {
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

void UserData::caveLevelPlantsData(char* key)
{
	unsigned int plantsNumber = 0;
	rapidjson::Value _object(rapidjson::kObjectType);
	rapidjson::Document::AllocatorType& allocator = _levelDataDocument->GetAllocator();
	(*_levelDataDocument)[key].AddMember("Plants", _object, allocator);
	
	for (auto plant : PlantsGroup)
	{
		rapidjson::Value object(rapidjson::kObjectType);
		
		auto visible = plant.second->getPlantAnimation()->isVisible();
		if (visible)
		{
			object.AddMember("PlantsTag", plant.second->getPlantTag(), allocator);
			object.AddMember("PlantsHealthPoint", plant.second->getPlantHealthPoint(), allocator);
			object.AddMember("PlantsPositionX", plant.second->getPlantAnimation()->getPositionX(), allocator);
			object.AddMember("PlantsPositionY", plant.second->getPlantAnimation()->getPositionY(), allocator);
			object.AddMember("PlantsRow", plant.second->getPlantRow(), allocator);
			object.AddMember("PlantsColumn", plant.second->getPlantColumn(), allocator);
			object.AddMember("PlantsLocalZOrder", plant.second->getPlantAnimation()->getLocalZOrder(), allocator);
			object.AddMember("PlantsType", static_cast<int>(plant.second->getPlantType()), allocator);
			object.AddMember("PlantVisible", visible, allocator);

			switch (plant.second->getPlantType())
			{
			case PlantsType::SunFlower:
				object.AddMember("SunShowTime.X", dynamic_cast<SunFlower*>(plant.second)->getSunShowTime().x, allocator);
				object.AddMember("SunShowTime.Y", dynamic_cast<SunFlower*>(plant.second)->getSunShowTime().y, allocator);
				break;
			case PlantsType::PotatoMine:
				object.AddMember("BreakGround", dynamic_cast<PotatoMine*>(plant.second)->getBreakGround(), allocator);
				break;
			default:
				break;
			}

			auto number = to_string(++plantsNumber);
			char* str = new char[number.size() + 1];
			strcpy(str, number.c_str());
			str[number.size()] = '\0';
			(*_levelDataDocument)[key]["Plants"].AddMember(rapidjson::StringRef(str), object, _levelDataDocument->GetAllocator());
		}
	}

	(*_levelDataDocument)[key]["Plants"].AddMember("PlantsNumber", plantsNumber, allocator);
}

void UserData::caveLevelZombiesData(char* key)
{
	unsigned int zombiesNumber = 0;
	rapidjson::Value _object(rapidjson::kObjectType);
	rapidjson::Document::AllocatorType& allocator = _levelDataDocument->GetAllocator();
	(*_levelDataDocument)[key].AddMember("Zombies", _object, allocator);
	(*_levelDataDocument)[key]["Zombies"].AddMember("ZombiesNumber", ZombiesGroup.size(), allocator);

	for (auto zombie : ZombiesGroup)
	{
		rapidjson::Value object(rapidjson::kObjectType);

		object.AddMember("ZombieCurrentBloodVolume", zombie->getZombieCurrentBloodVolume(), allocator);
		object.AddMember("ZombieCurrentBodyShieldVolume", zombie->getZombieCurrentBodyShieldVolume(), allocator);
		object.AddMember("ZombieCurrentHeadShieldVolume", zombie->getZombieCurrentHeadShieldVolume(), allocator);
		object.AddMember("ZombiePositionX", zombie->getZombieAnimation()->getPositionX(), allocator);
		object.AddMember("ZombiePositionY", zombie->getZombieAnimation()->getPositionY(), allocator);
		object.AddMember("ZombieLocalZOrder", zombie->getZombieAnimation()->getLocalZOrder(), allocator);
		object.AddMember("ZombieType", static_cast<int>(zombie->getZombieType()), allocator);
		object.AddMember("ZombieOpacity", static_cast<int>(zombie->getZombieAnimation()->getOpacity()), allocator);
		object.AddMember("ZombieVisible", zombie->getZombieAnimation()->isVisible(), allocator);

		auto number = to_string(++zombiesNumber);
		char* str = new char[number.size() + 1];
		strcpy(str, number.c_str());
		str[number.size()] = '\0';
		(*_levelDataDocument)[key]["Zombies"].AddMember(rapidjson::StringRef(str), object, _levelDataDocument->GetAllocator());
	}
}

void UserData::caveLevelSunData(char* key)
{
	unsigned int sunsNumber = 0;
	rapidjson::Value _object(rapidjson::kObjectType);
	rapidjson::Document::AllocatorType& allocator = _levelDataDocument->GetAllocator();
	(*_levelDataDocument)[key].AddMember("Sun", _object, allocator);
	(*_levelDataDocument)[key]["Sun"].AddMember("SunNumbers", SunsGroup.size(), allocator);

	for (auto sun : SunsGroup)
	{
		rapidjson::Value object(rapidjson::kObjectType);

		object.AddMember("SunTag", sun->getSunTag(), allocator);
		object.AddMember("Enable", sun->getEnable(), allocator);
		object.AddMember("PositionX", sun->getSun()->getPositionX(), allocator);
		object.AddMember("PositionY", sun->getSun()->getPositionY(), allocator);
		object.AddMember("Opacity", static_cast<int>(sun->getSun()->getOpacity()), allocator);

		auto number = to_string(++sunsNumber);
		char* str = new char[number.size() + 1];
		strcpy(str, number.c_str());
		str[number.size()] = '\0';
		(*_levelDataDocument)[key]["Sun"].AddMember(rapidjson::StringRef(str), object, _levelDataDocument->GetAllocator());
	}
}

void UserData::caveLevelCoinData(char* key)
{
	unsigned int coinNumber = 0;
	rapidjson::Value _object(rapidjson::kObjectType);
	rapidjson::Document::AllocatorType& allocator = _levelDataDocument->GetAllocator();
	(*_levelDataDocument)[key].AddMember("Coin", _object, allocator);
	(*_levelDataDocument)[key]["Coin"].AddMember("CoinNumbers", CoinsGroup.size(), allocator);

	for (auto coin : CoinsGroup)
	{
		rapidjson::Value object(rapidjson::kObjectType);

		object.AddMember("PositionX", coin->getCoin()->getPositionX(), allocator);
		object.AddMember("PositionY", coin->getCoin()->getPositionY(), allocator);
		object.AddMember("LocalZOrder", coin->getCoin()->getLocalZOrder(), allocator);
		object.AddMember("Enable", coin->getEnable(), allocator);

		auto number = to_string(++coinNumber);
		char* str = new char[number.size() + 1];
		strcpy(str, number.c_str());
		str[number.size()] = '\0';
		(*_levelDataDocument)[key]["Coin"].AddMember(rapidjson::StringRef(str), object, _levelDataDocument->GetAllocator());
	}
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
		
		auto number = to_string(++carsNumber);
		char* str = new char[number.size() + 1];
		strcpy(str, number.c_str());
		str[number.size()] = '\0';
		(*_levelDataDocument)[key]["Car"].AddMember(rapidjson::StringRef(str), object, _levelDataDocument->GetAllocator());
	}
}

void UserData::caveLevelBulletData(char* key)
{
	unsigned int bulletNumber = 0;
	rapidjson::Value _object(rapidjson::kObjectType);
	rapidjson::Document::AllocatorType& allocator = _levelDataDocument->GetAllocator();
	(*_levelDataDocument)[key].AddMember("Bullet", _object, allocator);
	(*_levelDataDocument)[key]["Bullet"].AddMember("BulletNumbers", BulletGroup.size(), allocator);

	for (auto bullet : BulletGroup)
	{
		rapidjson::Value object(rapidjson::kObjectType);

		object.AddMember("BulletType", static_cast<int>(bullet->getBulletType()), allocator);
		object.AddMember("PositionX", bullet->getBullet()->getPositionX(), allocator);
		object.AddMember("PositionY", bullet->getBullet()->getPositionY(), allocator);
		object.AddMember("IsUsed", bullet->getBulletIsUsed(), allocator);
		object.AddMember("LocalZOrder", bullet->getBullet()->getLocalZOrder(), allocator);
		object.AddMember("Opacity", static_cast<int>(bullet->getBullet()->getOpacity()), allocator);

		switch (bullet->getBulletType())
		{
		case BulletType::Pea:
		case BulletType::FirePea:
			object.AddMember("IsFire", dynamic_cast<Pea*>(bullet)->getIsFire(), allocator);
			object.AddMember("PeaFireNumbers", dynamic_cast<Pea*>(bullet)->getPeaFireNumbers(), allocator);
			break;
		case BulletType::Cabbage:
			object.AddMember("cabbageInitialPositionX", dynamic_cast<Cabbage*>(bullet)->getCabbageInitialPosition().x, allocator);
			object.AddMember("cabbageInitialPositionY", dynamic_cast<Cabbage*>(bullet)->getCabbageInitialPosition().y, allocator);
			object.AddMember("ZombiePositionX", dynamic_cast<Cabbage*>(bullet)->calculateZombiePosition().x, allocator);
			object.AddMember("ZombiePositionY", dynamic_cast<Cabbage*>(bullet)->calculateZombiePosition().y, allocator);
			object.AddMember("ZombieSpeed", dynamic_cast<Cabbage*>(bullet)->getZombieSpeed(), allocator);
			break;
		case BulletType::CitronBullet:
			object.AddMember("AnimationId", dynamic_cast<CitronBullet*>(bullet)->getAnimationId(), allocator);
			break;
		default:
			break;
		}

		auto number = to_string(++bulletNumber);
		char* str = new char[number.size() + 1];
		strcpy(str, number.c_str());
		str[number.size()] = '\0';
		(*_levelDataDocument)[key]["Bullet"].AddMember(rapidjson::StringRef(str), object, _levelDataDocument->GetAllocator());
	}
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
		object.AddMember("LastTime", buttonLayerInformation->plantsCards[card.cardTag].progressTimer->getPercentage() / 100.f *
			buttonLayerInformation->plantsInformation->PlantsCoolTime[card.cardTag], allocator);

		auto number = to_string(++plantsNumber);
		char* str = new char[number.size() + 1];
		strcpy(str, number.c_str());
		str[number.size()] = '\0';
		(*_levelDataDocument)[key]["SelectPlants"].AddMember(rapidjson::StringRef(str), object, _levelDataDocument->GetAllocator());
	}
}

void UserData::caveLevelOtherData(char* key)
{
	rapidjson::Document::AllocatorType& allocator = _levelDataDocument->GetAllocator();

	rapidjson::Value object(rapidjson::kObjectType);

	object.AddMember("SunNumbers", _global->userInformation->getSunNumbers(), allocator);
	object.AddMember("SurSunNumbers", backgroundLayerInformation->gameType->getSunNumberRequriement()->allSunNumbers, allocator);
	object.AddMember("SurPlusPlantsNumbers", backgroundLayerInformation->gameType->getPlantsRequriement()->surPlusPlantsNumbers, allocator);
	object.AddMember("ZombiesAppearFrequency", controlLayerInformation->_zombiesAppearControl->getZombiesAppearFrequency(), allocator);
	object.AddMember("ZombiesAppearTime", controlLayerInformation->_zombiesAppearControl->getTime(), allocator);
	object.AddMember("IsBegin", controlLayerInformation->_zombiesAppearControl->getIsBegin(), allocator);
	object.AddMember("IsShowWords", controlLayerInformation->_zombiesAppearControl->getIsShowWords(), allocator);
	object.AddMember("LastFrequencyZombiesWasDeath", controlLayerInformation->_zombiesAppearControl->getLastFrequencyZombiesWasDeath(), allocator);
	object.AddMember("ProgressBarPercent", informationLayerInformation->getProgressBarPercent(), allocator);
	object.AddMember("ProgressBarLastPercent", informationLayerInformation->getProgressBarLastPercent(), allocator);

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
	default: break;
	}
	return false;
}

void UserData::openLevelPlantsData(char* key)
{
	auto plantsNumbers = (*_levelDataDocument)[key]["Plants"]["PlantsNumber"].GetInt();
	for (int i = 1; i <= plantsNumbers; ++i)
	{
		auto type = static_cast<PlantsType>((*_levelDataDocument)[key]["Plants"][to_string(i).c_str()]["PlantsType"].GetInt());
		auto plants = animationLayerInformation->createDifferentPlants(type);
		plants->setPlantPosition(Vec2(
			(*_levelDataDocument)[key]["Plants"][to_string(i).c_str()]["PlantsPositionX"].GetFloat(),
			(*_levelDataDocument)[key]["Plants"][to_string(i).c_str()]["PlantsPositionY"].GetFloat()));
		plants->setPlantLocalZOrder((*_levelDataDocument)[key]["Plants"][to_string(i).c_str()]["PlantsLocalZOrder"].GetInt());
		plants->setPlantRowAndColumn(Vec2(
			(*_levelDataDocument)[key]["Plants"][to_string(i).c_str()]["PlantsRow"].GetInt(),
			(*_levelDataDocument)[key]["Plants"][to_string(i).c_str()]["PlantsColumn"].GetInt()));
		plants->setPlantTag((*_levelDataDocument)[key]["Plants"][to_string(i).c_str()]["PlantsTag"].GetInt());
		
		switch (type)
		{
		case PlantsType::SunFlower:
			dynamic_cast<SunFlower*>(plants)->setSunShowTime(
				Vec2((*_levelDataDocument)[key]["Plants"][to_string(i).c_str()]["SunShowTime.X"].GetFloat(),
					(*_levelDataDocument)[key]["Plants"][to_string(i).c_str()]["SunShowTime.Y"].GetFloat()));
			break;
		case PlantsType::PotatoMine:
			dynamic_cast<PotatoMine*>(plants)->setBreakGround((*_levelDataDocument)[key]["Plants"][to_string(i).c_str()]["BreakGround"].GetFloat());
			break;
		default:
			break;
		}

		plants->createPlantAnimation();
		plants->setPlantHealthPoint((*_levelDataDocument)[key]["Plants"][to_string(i).c_str()]["PlantsHealthPoint"].GetFloat());
		plants->getPlantAnimation()->setVisible((*_levelDataDocument)[key]["Plants"][to_string(i).c_str()]["PlantVisible"].GetBool());
		plants->getPlantAnimation()->getChildByName("SplashOfSoil")->setOpacity(0);

		PlantsGroup.insert(pair<int, Plants*>((*_levelDataDocument)[key]["Plants"][to_string(i).c_str()]["PlantsTag"].GetInt(), plants));
		
		controlLayerInformation->_gameMapInformation->plantsMap[plants->getPlantColumn()][plants->getPlantRow()] = static_cast<unsigned int>(type);/* 地图记录种植的植物 */
	}
}

void UserData::openLevelZombiesData(char* key)
{
	auto zombiesNumbers = (*_levelDataDocument)[key]["Zombies"]["ZombiesNumber"].GetInt();
	for (int i = 1; i <= zombiesNumbers; ++i)
	{
		auto zombies = animationLayerInformation->createDifferentZombies(
			static_cast<ZombiesType>((*_levelDataDocument)[key]["Zombies"][to_string(i).c_str()]["ZombieType"].GetInt()));

		zombies->setZombiePosition(Vec2(
			(*_levelDataDocument)[key]["Zombies"][to_string(i).c_str()]["ZombiePositionX"].GetFloat(),
			(*_levelDataDocument)[key]["Zombies"][to_string(i).c_str()]["ZombiePositionY"].GetFloat()));
		zombies->createZombie();

		zombies->setZombieCurrentBloodVolume((*_levelDataDocument)[key]["Zombies"][to_string(i).c_str()]["ZombieCurrentBloodVolume"].GetFloat());
		zombies->setZombieCurrentBodyShieldVolume((*_levelDataDocument)[key]["Zombies"][to_string(i).c_str()]["ZombieCurrentBodyShieldVolume"].GetFloat());
		zombies->setZombieCurrentHeadShieldVolume((*_levelDataDocument)[key]["Zombies"][to_string(i).c_str()]["ZombieCurrentHeadShieldVolume"].GetFloat());
		zombies->getZombieAnimation()->setOpacity((*_levelDataDocument)[key]["Zombies"][to_string(i).c_str()]["ZombieOpacity"].GetInt());
		zombies->getZombieAnimation()->getChildByName("shadow")->setOpacity(zombies->getZombieAnimation()->getOpacity());

		auto visible = (*_levelDataDocument)[key]["Zombies"][to_string(i).c_str()]["ZombieVisible"].GetBool();
		zombies->getZombieAnimation()->setVisible(visible);
		if (!visible) zombies->setZombieIsShowLoseLimbsAnimation(false);
		zombies->setZombieAttributeForGameType();
		ZombiesGroup.push_back(zombies);
		Zombies::zombiesNumbersChange("++");
	}
}

void UserData::openLevelSelectCardData(char* key)
{
	if (readLevelData())
	{
		UserSelectCard card;
		vector<UserSelectCard> userSelectCard;
		auto selectNumbers = (*_levelDataDocument)[key]["SelectPlants"]["SelectNumber"].GetInt();
		for (int i = 1; i <= selectNumbers; ++i)
		{
			card.cardTag = (*_levelDataDocument)[key]["SelectPlants"][to_string(i).c_str()]["CardTag"].GetInt();
			userSelectCard.push_back(card);
			PlantsInformation::PlantsCardFileData::PlantsSurPlusPrecent[card.cardTag] =
				(*_levelDataDocument)[key]["SelectPlants"][to_string(i).c_str()]["Percent"].GetFloat();
			PlantsInformation::PlantsCardFileData::PlantsSurPlusCoolTime[card.cardTag] =
				(*_levelDataDocument)[key]["SelectPlants"][to_string(i).c_str()]["LastTime"].GetFloat();
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
	auto sunNumbers = (*_levelDataDocument)[key]["Sun"]["SunNumbers"].GetInt();
	for (int i = 1; i <= sunNumbers; ++i)
	{
		auto sun = new Sun(animationLayerInformation->getSunLayer());
		sun->setSunTag((*_levelDataDocument)[key]["Sun"][to_string(i).c_str()]["SunTag"].GetInt());
		sun->setPosition(Vec2(
			(*_levelDataDocument)[key]["Sun"][to_string(i).c_str()]["PositionX"].GetFloat(),
			(*_levelDataDocument)[key]["Sun"][to_string(i).c_str()]["PositionY"].GetFloat()));
		sun->createSuns();

		auto enable = (*_levelDataDocument)[key]["Sun"][to_string(i).c_str()]["Enable"].GetBool();
		auto opacity = (*_levelDataDocument)[key]["Sun"][to_string(i).c_str()]["Opacity"].GetInt();
		sun->setEnable(enable);
		if (!enable || opacity < 255)
			sun->getSun()->setVisible(false);

		SunsGroup.push_back(sun);
	}
}

void UserData::openLevelCoinData(char* key)
{
	auto coinNumbers = (*_levelDataDocument)[key]["Coin"]["CoinNumbers"].GetInt();
	for (int i = 1; i <= coinNumbers; ++i)
	{
		auto coin = new Coin(animationLayerInformation->getAnimationLayer());
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
	auto carNumbers = (*_levelDataDocument)[key]["Car"]["CarNumbers"].GetInt();
	for (int i = 1; i <= carNumbers; ++i)
	{
		auto car = new Car(animationLayerInformation->getAnimationLayer());
		auto live = (*_levelDataDocument)[key]["Car"][to_string(i).c_str()]["Live"].GetBool();
		car->setLive(live);
		car->setPosition(Vec2(
			(*_levelDataDocument)[key]["Car"][to_string(i).c_str()]["PositionX"].GetFloat(),
			(*_levelDataDocument)[key]["Car"][to_string(i).c_str()]["PositionY"].GetFloat()));
		car->showCar();
		car->getCar()->setScale(0.8f);
		car->getCar()->getChildByName("fog")->setOpacity(0);
		CarsGroup.push_back(car);
	}
}

void UserData::openLevelBulletData(char* key)
{
	auto bulletNumbers = (*_levelDataDocument)[key]["Bullet"]["BulletNumbers"].GetInt();
	for (int i = 1; i <= bulletNumbers; ++i)
	{
		Bullet* bullet;
		auto type = static_cast<BulletType>((*_levelDataDocument)[key]["Bullet"][to_string(i).c_str()]["BulletType"].GetInt());
		switch (type)
		{
		case BulletType::Pea:
			bullet = new Pea(animationLayerInformation->getAnimationLayer());
			dynamic_cast<Pea*>(bullet)->setIsFire((*_levelDataDocument)[key]["Bullet"][to_string(i).c_str()]["IsFire"].GetBool());
			dynamic_cast<Pea*>(bullet)->setPeaFireNumbers((*_levelDataDocument)[key]["Bullet"][to_string(i).c_str()]["PeaFireNumbers"].GetInt());
			break;
		case BulletType::FirePea:
			bullet = new FirePea(animationLayerInformation->getAnimationLayer());
			dynamic_cast<FirePea*>(bullet)->setIsFire((*_levelDataDocument)[key]["Bullet"][to_string(i).c_str()]["IsFire"].GetBool());
			dynamic_cast<FirePea*>(bullet)->setPeaFireNumbers((*_levelDataDocument)[key]["Bullet"][to_string(i).c_str()]["PeaFireNumbers"].GetInt());
			break;
		case BulletType::Cabbage:
		{
			bullet = new Cabbage(animationLayerInformation->getAnimationLayer());
	
			Vec2 cabbageInitialPosition, zombiePosition;
			cabbageInitialPosition.x = (*_levelDataDocument)[key]["Bullet"][to_string(i).c_str()]["cabbageInitialPositionX"].GetFloat();
			cabbageInitialPosition.y = (*_levelDataDocument)[key]["Bullet"][to_string(i).c_str()]["cabbageInitialPositionY"].GetFloat();
			zombiePosition.x = (*_levelDataDocument)[key]["Bullet"][to_string(i).c_str()]["ZombiePositionX"].GetFloat();
			zombiePosition.y = (*_levelDataDocument)[key]["Bullet"][to_string(i).c_str()]["ZombiePositionY"].GetFloat();
			dynamic_cast<Cabbage*>(bullet)->setIsFileData(true);
			bullet->setBulletPosition(cabbageInitialPosition);
			dynamic_cast<Cabbage*>(bullet)->setCabbageCurrentPosition(Vec2(
				(*_levelDataDocument)[key]["Bullet"][to_string(i).c_str()]["PositionX"].GetFloat(),
				(*_levelDataDocument)[key]["Bullet"][to_string(i).c_str()]["PositionY"].GetFloat()));
			dynamic_cast<Cabbage*>(bullet)->setZombieSpeed((*_levelDataDocument)[key]["Bullet"][to_string(i).c_str()]["ZombiePositionY"].GetFloat());
			dynamic_cast<Cabbage*>(bullet)->setZombiePosition(zombiePosition);
		}
			break;
		case BulletType::AcidLemonBullet:
			bullet = new AcidLemonBullet(animationLayerInformation->getAnimationLayer());
			break;
		case BulletType::CitronBullet:
			bullet = new CitronBullet(animationLayerInformation->getAnimationLayer(),
				(*_levelDataDocument)[key]["Bullet"][to_string(i).c_str()]["AnimationId"].GetInt());
			break;
		default:
			break;
		}

		bullet->setBulletIsUsed((*_levelDataDocument)[key]["Bullet"][to_string(i).c_str()]["IsUsed"].GetBool());
		bullet->createBullet();

		bullet->getBullet()->setOpacity((*_levelDataDocument)[key]["Bullet"][to_string(i).c_str()]["Opacity"].GetInt());
		bullet->getBullet()->setLocalZOrder((*_levelDataDocument)[key]["Bullet"][to_string(i).c_str()]["LocalZOrder"].GetInt());

		if (type == BulletType::Pea)
		{
			if (dynamic_cast<Pea*>(bullet)->getIsFire())
			{
				bullet->getBullet()->setAnimation(0, "fire", true);
				bullet->getBullet()->getChildByName("shadow")->setPosition(Vec2(0, -52));
				bullet->getBullet()->getChildByName("shadow")->setScaleY(0.7f);
				bullet->getBullet()->getChildByName("shadow")->setScaleX(1.0f);
				bullet->getBullet()->setScale(1.5f);
			}
		}
		if (type != BulletType::Cabbage)
		{
			bullet->getBullet()->setPosition(Vec2(
				(*_levelDataDocument)[key]["Bullet"][to_string(i).c_str()]["PositionX"].GetFloat() - 20,
				(*_levelDataDocument)[key]["Bullet"][to_string(i).c_str()]["PositionY"].GetFloat()));
			bullet->setBulletOpacity((*_levelDataDocument)[key]["Bullet"][to_string(i).c_str()]["Opacity"].GetInt());
		}

		BulletGroup.push_back(bullet);
	}
}

void UserData::openLevelOtherData(char* key)
{
	auto number = (*_levelDataDocument)[key]["OtherData"]["ZombiesAppearFrequency"].GetInt();
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

	backgroundLayerInformation->gameType->getSunNumberRequriement()->allSunNumbers =
		(*_levelDataDocument)[key]["OtherData"]["SurSunNumbers"].GetInt();
	backgroundLayerInformation->gameType->getPlantsRequriement()->surPlusPlantsNumbers =
		(*_levelDataDocument)[key]["OtherData"]["SurPlusPlantsNumbers"].GetInt();
	backgroundLayerInformation->gameType->updateRequirementNumbers();
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

void UserData::replaceScene()
{
	Director::getInstance()->replaceScene(TransitionFade::create(1.0f, SelectPlantsScene::createScene()));
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