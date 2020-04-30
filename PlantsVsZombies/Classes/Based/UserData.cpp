/**
 *Copyright (c) 2019 LZ.All Right Reserved
 *Author : LZ
 *Date: 2020.4.20
 *Emal: 2117610943@qq.com
 */

#include "UserData.h"
#include "GlobalVariable.h"

//#define DEBUG 

UserData* UserData::_instance = nullptr;

UserData::UserData() :
  _global(Global::getInstance())
, _fileUtils(FileUtils::getInstance())
, _document(nullptr)
{
}

UserData::~UserData()
{
	if (_document) delete _document, _document = nullptr;
}

UserData* UserData::getInstance()
{
	if (_instance == nullptr)
	{
		_instance = new (std::nothrow)UserData;
	}
	return _instance;
}

void UserData::flush()
{
	StringBuffer buffer;
	rapidjson::Writer<StringBuffer> Writer(buffer);
	_document->Accept(Writer);

	string str = string(buffer.GetString());

#ifdef DEBUG
	_fileUtils->writeStringToFile(str, getUserDataFileName());
#else
	char* encryptString;
	encryptString = (char*)malloc(sizeof(char) * str.size() * 3);

	encrypt(str, encryptString);

	_fileUtils->writeStringToFile(encryptString, getUserDataFileName());

	free(encryptString);
#endif
}

openUserDataReturnType UserData::openUserData()
{
	// 如果有这个存档
	if (_fileUtils->isFileExist(getUserDataFileName()))
	{
		if (_userData.empty())
		{
#ifdef DEBUG
			_userData = _fileUtils->getStringFromFile(getUserDataFileName());
			_document->Parse<rapidjson::kParseDefaultFlags>(_userData.c_str());
#else
			char* decryptString;
			_userData = _fileUtils->getStringFromFile(getUserDataFileName());
			decryptString = (char*)malloc(sizeof(char) * _userData.size());
			decrypt(_userData, decryptString);
			_document->Parse<rapidjson::kParseDefaultFlags>(decryptString);
			free(decryptString);
#endif
			if (_document->HasParseError()) {
				return openUserDataReturnType::FileExistError;
			}
		}
		return openUserDataReturnType::FileExistCorrect;
	}
	else
	{
		if (!_document->IsObject())
		{
			_document->SetObject();
			rapidjson::Value _object(rapidjson::kObjectType);
			_document->AddMember("UserData", _object, _document->GetAllocator());
		}
		return openUserDataReturnType::FileNotExist;
	}
}

void UserData::encrypt(string& cSrc, char* cDest)
{
	char  c;
	int i, h, l, j = 0;
	for (i = 0; i < static_cast<int>(cSrc.size()); i++)
	{
		c = cSrc[i];
		h = (c >> 4) & 0xf;
		l = c & 0xf;
		cDest[j] = h + 'x';
		cDest[j + 1] = l + 'z';
		j += 2;
	}
	cDest[j] = '\0';
}

void UserData::decrypt(string& cSrc, char* cDest)
{
	int i, h, l, m, n, j = 0;
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

bool UserData::isHaveMember(char* key)
{
	if ((*_document)["UserData"].HasMember(key))
		return true;
	return false;
}

string UserData::getUserDataFileName()
{
	return _fileUtils->getWritablePath() + "UserDataFile_" + to_string(_global->userInformation->getUserCaveFileNumber()) + ".dat";
}

void UserData::caveUserData(char* key, double value)
{
	switch (openUserData())
	{
	case openUserDataReturnType::FileExistCorrect:
		if (isHaveMember(key))
			(*_document)["UserData"][key].SetDouble(value);
		else
			(*_document)["UserData"].AddMember(rapidjson::StringRef(key), value, _document->GetAllocator());
		break;
	case openUserDataReturnType::FileNotExist:
		(*_document)["UserData"].AddMember(rapidjson::StringRef(key), value, _document->GetAllocator());
		break;
	}
	flush();
}

void UserData::caveUserData(char* key, bool value)
{
	switch (openUserData())
	{
	case openUserDataReturnType::FileExistCorrect:
		if (isHaveMember(key))
			(*_document)["UserData"][key].SetBool(value);
		else
			(*_document)["UserData"].AddMember(rapidjson::StringRef(key), value, _document->GetAllocator());
		break;
	case openUserDataReturnType::FileNotExist:
		(*_document)["UserData"].AddMember(rapidjson::StringRef(key), value,
			_document->GetAllocator());
		break;
	}
	flush();
}

void UserData::caveUserData(char* key, char* value)
{
	switch (openUserData())
	{
	case openUserDataReturnType::FileExistCorrect:
		if (isHaveMember(key))
			(*_document)["UserData"][key].SetString(rapidjson::StringRef(value));
		else
			(*_document)["UserData"].AddMember(rapidjson::StringRef(key), rapidjson::StringRef(value), _document->GetAllocator());
		break;
	case openUserDataReturnType::FileNotExist:
		(*_document)["UserData"].AddMember(rapidjson::StringRef(key), rapidjson::StringRef(value), _document->GetAllocator());
		break;
	}
	flush();
}

void UserData::caveUserData(char* key, int value)
{
	switch (openUserData())
	{
	case openUserDataReturnType::FileExistCorrect:
		if (isHaveMember(key))
			(*_document)["UserData"][key].SetInt(value);
		else
			(*_document)["UserData"].AddMember(rapidjson::StringRef(key), value, _document->GetAllocator());
		break;
	case openUserDataReturnType::FileNotExist:
		(*_document)["UserData"].AddMember(rapidjson::StringRef(key), value,
			_document->GetAllocator());
		break;
	}
	flush();
}

int UserData::openIntUserData(char* key)
{
	switch (openUserData())
	{
	case openUserDataReturnType::FileExistCorrect:
		if (isHaveMember(key))
			return (*_document)["UserData"][key].GetInt();
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
			return (*_document)["UserData"][key].GetDouble();
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
			return (*_document)["UserData"][key].GetBool();
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
			return (*_document)["UserData"][key].GetString();
		break;
	default: break;
	}
	return nullptr;
}

void UserData::createNewDocument()
{
	if (_document)
	{
		delete _document;
		_document = nullptr;
	}
	_document = new Document();
	_userData.clear();
}
