/**
 *Copyright (c) 2020 LZ.All Right Reserved
 *Author : LZ
 *Date: 2020.2.20
 *Email: 2117610943@qq.com
 */

#include "UserInformation.h"

UserInformation::UserInformation():
  _isUpdate(false)
, _userName("·ßÅ­µÄÐ¡½©Ê¬")
, _userCaveFileNameKey{ "USERNAMEDATA","USERNAMEDATA_2","USERNAMEDATA_3","USERNAMEDATA_4","USERNAMEDATA_5","USERNAMEDATA_6","USERNAMEDATA_7","USERNAMEDATA_8" }
, _systemCaveFileName_difficult{ "WORLD_1_LEVELS","WORLD_1_LEVELS_2","WORLD_1_LEVELS_3","WORLD_1_LEVELS_4","WORLD_1_LEVELS_5","WORLD_1_LEVELS_6","WORLD_1_LEVELS_7","WORLD_1_LEVELS_8" }
, _systemCaveFileName{ "WORLD_1_LEVELS_DIF","WORLD_1_LEVELS_2_DIF","WORLD_1_LEVELS_3_DIF","WORLD_1_LEVELS_4_DIF","WORLD_1_LEVELS_5_DIF","WORLD_1_LEVELS_6_DIF","WORLD_1_LEVELS_7_DIF","WORLD_1_LEVELS_8_DIF" }
, _soundEffectVolume(0.5f)
, _backGroundMusicVolume(0.2f)
, _userCaveFileNumber(-1)
, _killZombiesNumbers(0)
, _usePlantsNumbers(0)
, _breakThroughnumbers(0)
, _sunNumbers(100)
, _coinNumbers(0)
, _nowFps(60)
, _gameDifficulty(0)
, _mainToWorld(false)
, _background(nullptr)
, _currentPlayLevels(1)
, _currentPlayWorldTag(0)
, _showInformation(CheckBox::EventType::SELECTED)
, _highFPS(CheckBox::EventType::SELECTED)
, _fullScreen(CheckBox::EventType::UNSELECTED)
, _cursorNotHide(CheckBox::EventType::UNSELECTED)
, _stretchingShow(CheckBox::EventType::SELECTED)
, _selectWorldName(WorldName::Mordern)
{
    for (int i = 0; i < 5; ++i)
    {
        WorldData* worldData = new WorldData();

        _userSelectWorldData.push_back(worldData);
    }
}

UserInformation::~UserInformation()
{
    for (auto worldName :_userSelectWorldData)
    {
        delete worldName;
        worldName = nullptr;
    }
    _userSelectWorldData.clear();
}

vector<UserSelectCard>& UserInformation::getUserSelectCrads()
{
    return _userSelectCard;
}

vector<WorldData*>& UserInformation::getUserSelectWorldData()
{
    return _userSelectWorldData;
}

CheckBox::EventType UserInformation::getIsShowInformation() const
{
    return _showInformation;
}

CheckBox::EventType UserInformation::getIsSelectHighFPS() const
{
    return _highFPS;
}

CheckBox::EventType UserInformation::getIsSelectFullScreen() const
{
    return _fullScreen;
}

CheckBox::EventType UserInformation::getIsSelectCursorNotHide() const
{
    return _cursorNotHide;
}

CheckBox::EventType UserInformation::getIsSelectStretchingShow() const
{
    return _stretchingShow;
}

map<string, spSkeletonData*>& UserInformation::getAnimationData()
{
    return _animationData;
}

map<string, string>& UserInformation::getGameText()
{
    return _gameText;
}

map<string, string>& UserInformation::getImagePath()
{
    return _imagesPath;
}

map<string, string>& UserInformation::getMusicPath()
{
    return _musicsPath;
}

map<string, string>& UserInformation::getAnimationPath()
{
    return _animationPath;
}

map<string, string>& UserInformation::getTextPath()
{
    return _textPath;
}

WorldName UserInformation::getSelectWorldName() const
{
    return _selectWorldName;
}

string& UserInformation::getUserName()
{
    return _userName;
}

string& UserInformation::getUserCaveFileName(const int id)
{
    return _userCaveFileName[id];
}

string& UserInformation::getUserCaveFileNameKey(const int id)
{
    return _userCaveFileNameKey[id];
}

string& UserInformation::getSystemCaveFileName(const int id)
{
    return _systemCaveFileName[id];
}

string& UserInformation::getSystemDifCaveFileName(const int id)
{
    return _systemCaveFileName_difficult[id];
}

list<int>& UserInformation::getBackgroundMusic()
{
    return _backGroundMusic;
}

float UserInformation::getSoundEffectVolume() const
{
    return _soundEffectVolume;
}

float UserInformation::getBackGroundMusicVolume() const
{
    return _backGroundMusicVolume;
}

int UserInformation::getUserCaveFileNumber() const
{
    return _userCaveFileNumber;
}

int& UserInformation::getKillZombiesNumbers()
{
    return _killZombiesNumbers;
}

int& UserInformation::getUsePlantsNumbers()
{
    return _usePlantsNumbers;
}

int& UserInformation::getBreakThroughnumbers()
{
    return _breakThroughnumbers;
}

int UserInformation::getSunNumbers() const
{
    return _sunNumbers;
}

int UserInformation::getCoinNumbers() const
{
    return _coinNumbers;
}

int UserInformation::getFps() const
{
    return _nowFps;
}

int UserInformation::getCurrentPlayLevels() const
{
    return _currentPlayLevels;
}

int UserInformation::getCurrentPlayWorldTag() const
{
    return _currentPlayWorldTag;
}

bool UserInformation::getMainToWorld() const
{
    return _mainToWorld;
}

bool UserInformation::getIsUpdate() const
{
    return _isUpdate;
}

bool UserInformation::getIsShowEggs() const
{
    return _isShowEggs;
}

Sprite* UserInformation::getBackground() const
{
    return _background;
}

string& UserInformation::getCurrentPlayWorldName()
{
    return _currentPlayWorldName;
}

int UserInformation::getGameDifficulty() const
{
    return _gameDifficulty;
}

void UserInformation::setGameDifficulty(const int difficulty)
{
    _gameDifficulty = difficulty;
}

void UserInformation::setCurrentPlayWorldName(const string& worldName)
{
    _currentPlayWorldName = worldName;
}

void UserInformation::setUserSelectCrads(vector<UserSelectCard>& card)
{
    _userSelectCard = card;
}

void UserInformation::setSelectWorldName(WorldName name)
{
    _selectWorldName = name;
}

void UserInformation::setBackground(Sprite* background)
{
    _background = background;
}

void UserInformation::setIsShowEggs(const bool isShow)
{
    _isShowEggs = isShow;
}

void UserInformation::setUserName(const string& name)
{
    _userName = name;
}

void UserInformation::setUserCaveFileName(const int id, const string& name)
{
    _userCaveFileName[id] = name;
}

void UserInformation::setSoundEffectVolume(const float volume)
{
    _soundEffectVolume = volume;
}

void UserInformation::setBackGroundMusicVolume(const float volume)
{
    _backGroundMusicVolume = volume;
}

void UserInformation::setUserCaveFileNumber(const int number)
{
    _userCaveFileNumber = number;
}

void UserInformation::setKillZombiesNumbers(const int number)
{
    _killZombiesNumbers = number;
}

void UserInformation::setUsePlantsNumbers(const int number)
{
    _usePlantsNumbers = number;
}

void UserInformation::setBreakThroughnumbers(const int number)
{
    _breakThroughnumbers = number;
}

void UserInformation::setSunNumbers(const int number)
{
    _sunNumbers = number;
}

void UserInformation::setCoinNumbers(const int number)
{
    _coinNumbers = number;
}

void UserInformation::setFps(const int fps)
{
    _nowFps = fps;
}

void UserInformation::setCurrentPlayLevels(const int level)
{
    _currentPlayLevels = level;
}

void UserInformation::setCurrentPlayWorldTag(const int tag)
{
    _currentPlayWorldTag = tag;
}

void UserInformation::setMainToWorld(const bool isMainToWorld)
{
    _mainToWorld = isMainToWorld;
}

void UserInformation::setIsUpdate(bool isUpdate)
{
    _isUpdate = isUpdate;
}

void UserInformation::setIsSelectStretchingShow(CheckBox::EventType isSelect)
{
    _stretchingShow = isSelect;
}

void UserInformation::setIsSelectCursorNotHide(CheckBox::EventType isSelect)
{
    _cursorNotHide = isSelect;
}

void UserInformation::setIsSelectFullScreen(CheckBox::EventType isSelect)
{
    _fullScreen = isSelect;
}

void UserInformation::setIsSelectHighFPS(CheckBox::EventType isSelect)
{
    _highFPS = isSelect;
}

void UserInformation::setIsShowInformation(CheckBox::EventType isShow)
{
    _showInformation = isShow;
}
