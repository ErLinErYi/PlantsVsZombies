/**
 *Copyright (c) 2020 LZ.All Right Reserved
 *Author : LZ
 *Date: 2020.2.20
 *Email: 2117610943@qq.com
 *1.2.0.3
 */

#include "UserInformation.h"
#include "GlobalVariable.h"

const string UserInformation::_clientEdition = "9.9.9.9";
string UserInformation::_editionName = "";
bool UserInformation::_updateRequired = false;
DWORD UserInformation::_screenDisplayFrequency = 0;

UserInformation::UserInformation():
  _isUpdate(false)
, _userName("Î´ÃüÃû´æµµ")
, _soundEffectVolume(0.5f)
, _backGroundMusicVolume(0.2f)
, _userCaveFileNumber(0)
, _killZombiesNumbers(0)
, _usePlantsNumbers(0)
, _breakThroughNumbers(0)
, _sunNumbers(100)
, _coinNumbers(0)
, _nowFps(60)
, _gameDifficulty(0)
, _mainToWorld(false)
, _isMirrorScene(false)
, _isReadFileLevelData(false)
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
    string keyName[] = { "USERNAMEDATA","USERNAMEDATA_2","USERNAMEDATA_3","USERNAMEDATA_4","USERNAMEDATA_5","USERNAMEDATA_6","USERNAMEDATA_7","USERNAMEDATA_8" };
    string fileName[] = { "WORLD_%d_LEVELS","WORLD_%d_LEVELS_2","WORLD_%d_LEVELS_3","WORLD_%d_LEVELS_4","WORLD_%d_LEVELS_5","WORLD_%d_LEVELS_6","WORLD_%d_LEVELS_7","WORLD_%d_LEVELS_8" };
    string dfileName[] = { "WORLD_%d_LEVELS_DIF","WORLD_%d_LEVELS_2_DIF","WORLD_%d_LEVELS_3_DIF","WORLD_%d_LEVELS_4_DIF","WORLD_%d_LEVELS_5_DIF","WORLD_%d_LEVELS_6_DIF","WORLD_%d_LEVELS_7_DIF","WORLD_%d_LEVELS_8_DIF" };

    for (int i = 0; i < 8; ++i) 
    {
        _userCaveFileNameKey[i] = keyName[i];
        _systemCaveFileName_difficult[i] = fileName[i];
        _systemCaveFileName[i] = dfileName[i];
    }
}

UserInformation::~UserInformation()
{
    deleteUserSelectWorldData();
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

CheckBox::EventType UserInformation::getIsEaseAnimation() const
{
    return _easeAnimation;
}

CheckBox::EventType UserInformation::getIsVerticalSynchronization() const
{
    return _verticalSynchronization;
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

int& UserInformation::getBreakThroughNumbers()
{
    return _breakThroughNumbers;
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

bool UserInformation::getIsMirrorScene() const
{
    return _isMirrorScene;
}

string UserInformation::getClientEdition()
{
    string str;
    for (auto s : _clientEdition)
        if ('.' != s)
            str += s;
    return str;
}

void UserInformation::setNewEditionName(const string& editionName)
{
    _editionName = editionName;
}

string UserInformation::getNewEditionName(bool dot)
{
    if (!dot)
    {
        string str;
        for (auto s : _editionName)
            if (s != '.')
                str += s;
        return str;
    }
    else
        return _editionName;
}

bool UserInformation::getUpdateRequired()
{
    return _updateRequired;
}

void UserInformation::setUpdateRequired(const bool updateRequired)
{
    _updateRequired = updateRequired;
}

DWORD UserInformation::getScreenDisplayFrequency()
{
    if (!_screenDisplayFrequency ||
        Global::getInstance()->userInformation->getIsSelectFullScreen() == cocos2d::ui::CheckBox::EventType::UNSELECTED)
    {
        DEVMODE dm;
        dm.dmSize = sizeof(DEVMODE);
        ::EnumDisplaySettings(nullptr, ENUM_CURRENT_SETTINGS, &dm);
        _screenDisplayFrequency = dm.dmDisplayFrequency;
    }
    return _screenDisplayFrequency;
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

void UserInformation::setBreakThroughNumbers(const int number)
{
    _breakThroughNumbers = number;
}

void UserInformation::setSunNumbers(const int number)
{
    if (_sunNumbers <= 100000)
    {
        _sunNumbers = number;
    }
    else
    {
        _sunNumbers = 100000;
    }
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

void UserInformation::setIsEaseAnimation(CheckBox::EventType easeAnimation)
{
    _easeAnimation = easeAnimation;
}

void UserInformation::setIsVerticalSynchronization(CheckBox::EventType verticalSynchronization)
{
    _verticalSynchronization = verticalSynchronization;
}

void UserInformation::setIsMirrorScene(const bool isMirror)
{
    _isMirrorScene = isMirror;
}

void UserInformation::setIsReadFileData(const bool isRead)
{
    _isReadFileLevelData = isRead;
}

bool UserInformation::getIsReadFileData() const
{
    return _isReadFileLevelData;
}

char* UserInformation::getCurrentCaveFileLevelWorldName()
{
    auto name = "LevelData_" +
        to_string(getCurrentPlayWorldTag()) + "_" +
        to_string(getGameDifficulty()) + "_" +
        to_string(getCurrentPlayLevels());
    char* str = new char[name.size() + 1];
    strcpy(str, name.c_str());
    str[name.size()] = '\0';
    return str;
}

void UserInformation::newUserSelectWorldData()
{
    deleteUserSelectWorldData();
    for (int i = 0; i < 3; ++i)
    {
        WorldData* worldData = new WorldData();

        _userSelectWorldData.push_back(worldData);
    }
}

void UserInformation::deleteUserSelectWorldData()
{
    for (auto worldName : _userSelectWorldData)
    {
        delete worldName;
        worldName = nullptr;
    }
    _userSelectWorldData.clear();
}
