#include "Based/LevelData.h"
#include "Based/GameType.h"

OpenLevelData* OpenLevelData::_instance = nullptr;

OpenLevelData* OpenLevelData::getInstance()
{
	if (_instance == nullptr)
	{
		_instance = new (std::nothrow)OpenLevelData;
	}
	return _instance;
}

bool OpenLevelData::openLevelsData(const string& worlddata)
{
	char* passWords;
	string str = FileUtils::getInstance()->getStringFromFile(worlddata);
	passWords = (char*)malloc(sizeof(char) * str.size());

	/* 解密 */
	decrypt(str, passWords);
	
	documentInit();
	_document->Parse<rapidjson::kParseDefaultFlags>(passWords);

	free(passWords);

	if (_document->HasParseError())return false;

	return true;
}

void OpenLevelData::decrypt(char* cSrc, char* cDest) 
{
	int   i, h, l, m, n, j = 0;
	int len = strlen(cSrc);
	for (i = 0; i < len; i = i + 2)
	{
		h = (cSrc[i] - 'x');
		l = (cSrc[i + 1] - 'z');
		m = (h << 4);
		n = (l & 0xf);
		cDest[j] = m + n;
		j++;
	}
	cDest[j] = '\0';
}

void OpenLevelData::decrypt(string& cSrc, char* cDest)
{
	int   i, h, l, m, n, j = 0;
	for (i = 0; i < static_cast<int>(cSrc.size()); i = i + 2)
	{
		h = (cSrc[i] - 'x');
		l = (cSrc[i + 1] - 'z');
		m = (h << 4);
		n = (l & 0xf);
		cDest[j] = m + n;
		j++;
	}
	cDest[j] = '\0';
}

Document* OpenLevelData::getDocument()
{
	return _document;
}

void OpenLevelData::createLevelData(const int level, const char* levelName)
{
	/* map中如果没有关卡数据 */
	if (!_levelData.count(level))
	{
		LevelData* levelData = new LevelData;
		levelData->readLevelData(levelName);
		_levelData.insert(pair<int, LevelData*>(level, levelData));
	}
}

LevelData* OpenLevelData::readLevelData(const int level)
{
	auto data = _levelData.find(level);
	if (data != _levelData.end())
	{
		return data->second;
	}
	return nullptr;
}

void OpenLevelData::setLevelNumber(const int levelNumber)
{
	_levelNumber = levelNumber;
}

int OpenLevelData::getLevelNumber() const
{
	return _levelNumber;
}

void OpenLevelData::documentInit()
{
	for (auto data : _levelData)
	{
		delete data.second; 
		data.second = nullptr;
	}
	_levelData.clear();
}

LevelData::LevelData():
	_isEncryption(false),
	_zombiesIsVisible(false),
	_zombiesIsSmall(false),
	_zombiesIsBig(false),
	_isNoPlants(false),
	_zombiesFrequency(0),
	_coinNumbers(0),
	_atLeastSunNumbers(-1),
	_flowerPosition(-1),
	_carNumbers(0),
	_usePlantsNumbers(666666),
	_firstFrequencyTime(18),
	_userLose(0)
{
}

LevelData::~LevelData()
{
}

bool LevelData::readLevelData(const char* LevelName)
{
	_document = OpenLevelData::getInstance()->getDocument();

	if (_document->HasMember(LevelName))
	{
		_isEncryption = (*_document)[LevelName]["IsEncryption"].GetBool();
		_coinNumbers = (*_document)[LevelName]["CoinNumbers"].GetInt();
		_zombiesFrequency = (*_document)[LevelName]["Frequency"].GetInt();
		_firstFrequencyTime = (*_document)[LevelName]["FirstFrequencyTime"].GetInt();
		_userLose = (*_document)[LevelName]["UserLose"].GetFloat();

		for (unsigned int i = 0; i < (*_document)[LevelName]["GameType"].Size(); i++)
		{
			_gameType.push_back((*_document)[LevelName]["GameType"][i].GetInt());
		}

		for (unsigned int i = 0; i < (*_document)[LevelName]["ZombiesType"].Size(); i++)
		{
			_zombiesType.push_back((*_document)[LevelName]["ZombiesType"][i].GetInt());
		}

		for (unsigned int i = 0; i < (*_document)[LevelName]["MunchZombiesFrequency"].Size(); i++)
		{
			_munchZombiesFrequency.push_back((*_document)[LevelName]["MunchZombiesFrequency"][i].GetInt());
		}

		for (unsigned int i = 0; i < (*_document)[LevelName]["ZombiesNumbers"].Size(); i++)
		{
			_zombiesNumbers.push_back((*_document)[LevelName]["ZombiesNumbers"][i].GetInt());
		}

		vector<int> v;
		for (unsigned int i = 0; i < (*_document)[LevelName]["ZombiesTypeProbability"].Size(); i++)
		{
			v.clear();
			for (unsigned int j = 0; j < (*_document)[LevelName]["ZombiesTypeProbability"][i].Size(); j++)
			{
				v.push_back((*_document)[LevelName]["ZombiesTypeProbability"][i][j].GetInt());
			}
			_zombiesTypeProbabilityFrequency.push_back(v);
		}

		setGameTypes(LevelName);

		return true;
	}
	return false;
}

void LevelData::setGameTypes(const char* LevelName)
{
	for (unsigned int i = 0; i < _gameType.size(); i++)
	{
		switch (static_cast<GameTypes>(_gameType.at(i)))
		{
		case GameTypes::AtLeastSunNumbers:
			_atLeastSunNumbers = (*_document)[LevelName]["AtLeastSunNumbers"].GetInt();
			break;
		case GameTypes::FlowerPosition:
			_flowerPosition = 570 + 122 * (*_document)[LevelName]["FlowerPosition"].GetInt();
			break;
		case GameTypes::CarNumbers:
			_carNumbers = (*_document)[LevelName]["CarNumbers"].GetInt();
			break;
		case GameTypes::UserPlantsNumbers:
			_usePlantsNumbers = (*_document)[LevelName]["UserPlantsNumbers"].GetInt();
			break;
		case GameTypes::ZombiesInvisible:
			_zombiesIsVisible = true;
			break;
		case GameTypes::SmallZombies:
			_zombiesIsSmall = true;
			break;
		case GameTypes::BigZombies:
			_zombiesIsBig = true;
			break;
		case GameTypes::NoPlants:
		{
			_isNoPlants = true;
			MyPoint MyPoint;
			for (unsigned int i = 0; i < (*_document)[LevelName]["NoPlants"].Size(); i++)
			{
				MyPoint.x = (*_document)[LevelName]["NoPlants"][i][0].GetInt();
				MyPoint.y = (*_document)[LevelName]["NoPlants"][i][1].GetInt();
				_noPlantsPosition.push_back(MyPoint);
			}
		}
		break;
		}
	}
}