/**
 *Copyright (c) 2020 LZ.All Right Reserved
 *Author : LZ
 *Date: 2020.2.20
 *Email: 2117610943@qq.com
 */

#pragma once
#include <list>

#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "spine/spine-cocos2dx.h"

using namespace std;
using namespace cocos2d;
using namespace cocos2d::ui;

/* 世界名字 */
enum class WorldName
{
	Mordern = 0,
	DarkAges,
	NeonMixtapeTour,
	FrostibiteCaves
};

/* 用户选择的卡牌存储 */
struct UserSelectCard
{
	Button* cardbutton;     //卡牌按钮
	int cardTag;            //卡牌的编号
};

/* 世界关卡 */
struct WorldData
{
	WorldData()
		: levels(1),
		isBeginShowEggs(false),
		isReadWoldInformation(false)
	{}
	int levels;                      // 当前世界当前最高关卡数
	bool isBeginShowEggs;
	bool isReadWoldInformation;      // 是否读取世界信息
};

class UserInformation
{
public:
	vector<UserSelectCard>& getUserSelectCrads();
	vector<WorldData*>& getUserSelectWorldData();
	CheckBox::EventType getIsShowInformation() const;
	CheckBox::EventType getIsSelectHighFPS() const;
	CheckBox::EventType getIsSelectFullScreen() const;
	CheckBox::EventType getIsSelectCursorNotHide() const;
	CheckBox::EventType getIsSelectStretchingShow() const;
	CheckBox::EventType getIsEaseAnimation() const;
	CheckBox::EventType getIsVerticalSynchronization() const;
	map<string, spSkeletonData*>& getAnimationData();
	map<string, string>& getGameText();
	map<string, string>& getImagePath();
	map<string, string>& getMusicPath();
	map<string, string>& getAnimationPath();
	map<string, string>& getTextPath();
	WorldName getSelectWorldName() const;
	string& getUserName();
	string& getUserCaveFileName(const int id);
	string& getUserCaveFileNameKey(const int id);
	string& getSystemCaveFileName(const int id);
	string& getSystemDifCaveFileName(const int id);
	list<int>& getBackgroundMusic();
	float getSoundEffectVolume() const;
	float getBackGroundMusicVolume() const;
	int getUserCaveFileNumber() const;
	int& getKillZombiesNumbers();
	int& getUsePlantsNumbers();
	int& getBreakThroughNumbers();
	int getSunNumbers() const;
	int getCoinNumbers() const;
	int getFps() const;
	int getCurrentPlayLevels() const;
	int getCurrentPlayWorldTag() const;
	bool getMainToWorld() const;
	bool getIsUpdate() const;
	bool getIsShowEggs() const;
	Sprite* getBackground() const;
	string& getCurrentPlayWorldName();
	int getGameDifficulty() const;
	bool getIsMirrorScene() const;
	void setGameDifficulty(const int difficulty);
	void setCurrentPlayWorldName(const string& worldName);
	void setUserSelectCrads(vector<UserSelectCard>& card);
	void setSelectWorldName(WorldName name);
	void setBackground(Sprite* background);
	void setIsShowEggs(const bool isShow);
	void setUserName(const string& name);
	void setUserCaveFileName(const int id, const string& name);
	void setSoundEffectVolume(const float volume);
	void setBackGroundMusicVolume(const float volume);
	void setUserCaveFileNumber(const int number);
	void setKillZombiesNumbers(const int number);
	void setUsePlantsNumbers(const int number);
	void setBreakThroughNumbers(const int number);
	void setSunNumbers(const int number);
	void setCoinNumbers(const int number);
	void setFps(const int fps);
	void setCurrentPlayLevels(const int level);
	void setCurrentPlayWorldTag(const int tag);
	void setMainToWorld(const bool isMainToWorld);
	void setIsUpdate(const bool isUpdate);
	void setIsSelectStretchingShow(CheckBox::EventType isSelect);
	void setIsSelectCursorNotHide(CheckBox::EventType isSelect);
	void setIsSelectFullScreen(CheckBox::EventType isSelect);
	void setIsSelectHighFPS(CheckBox::EventType isSelect);
	void setIsShowInformation(CheckBox::EventType isShow);
	void setIsEaseAnimation(CheckBox::EventType easeAnimation);
	void setIsVerticalSynchronization(CheckBox::EventType verticalSynchronization);
	void setIsMirrorScene(const bool isMirror);
	void setIsReadFileData(const bool isRead);
	bool getIsReadFileData() const;
	char* getCurrentCaveFileLevelWorldName();
	void newUserSelectWorldData();
	void deleteUserSelectWorldData();

	static string getClientEdition();
	static void setNewEditionName(const string& editionName);
	static string getNewEditionName(bool dot = false);
	static bool getUpdateRequired();
	static void setUpdateRequired(const bool update);
	static DWORD getScreenDisplayFrequency();

CC_CONSTRUCTOR_ACCESS:
	UserInformation();
	~UserInformation();

private:
	vector<UserSelectCard> _userSelectCard;              // 用户一次闯关所选植物
	vector<WorldData*> _userSelectWorldData;             // 用户所选世界游戏数据

	WorldName _selectWorldName;                          // 所选世界

	CheckBox::EventType _showInformation;                // 显示fps
	CheckBox::EventType _highFPS;                        // 高帧率
	CheckBox::EventType _fullScreen;                     // 全屏
	CheckBox::EventType _cursorNotHide;                  // 鼠标隐藏
	CheckBox::EventType _stretchingShow;                 // 拉伸显示
	CheckBox::EventType _easeAnimation;                  // 缓入动画
	CheckBox::EventType _verticalSynchronization;        // 垂直同步

	map<string, spSkeletonData*> _animationData;         // 动画存储
	map<string, string> _gameText;                       // 游戏文本
	map<string, string> _imagesPath;                     // 图片路径
	map<string, string> _musicsPath;                     // 音乐路径 
	map<string, string> _animationPath;                  // 动画路径
	map<string, string> _textPath;                       // 文本路径

	bool _isReadFileLevelData;
	bool _isUpdate;                                      // 名字是否更新
	string _userName;                                    // 用户名字
	string _userCaveFileName[8];                         // 用户自己定义存档名称（主要用于显示）
	string _userCaveFileNameKey[8];                      // 用户存档key
	string _systemCaveFileName[8];                       // 系统定义存档名称（用于存取，防止用户定义的为空)
	string _systemCaveFileName_difficult[8];             // 系统定义困难存档名称（用于存取，防止用户定义的为空)

	float _soundEffectVolume;                            // 音效
	float _backGroundMusicVolume;                        // 音乐
	list<int>_backGroundMusic;

	int _userCaveFileNumber;                             // 用户游戏存档编号 

	int _killZombiesNumbers;                             // 杀死僵尸总数 
	int _usePlantsNumbers;                               // 使用植物数量 
	int _breakThroughNumbers;                            // 闯关失败次数

	int _sunNumbers;                                     // 阳光数 
	int _coinNumbers;                                    // 金币数
	int _nowFps;                                         // 当前fps
	bool _mainToWorld;                                   // 标记主菜单到世界
	bool _isShowEggs;                                    // 是否显示彩蛋
	bool _isMirrorScene;                                 // 是否镜像世界

	int _currentPlayLevels;                              // 当前关数
	int _gameDifficulty;                                 // 游戏难度
	int _currentPlayWorldTag;                            // 当前世界编号 
	Sprite* _background;                                 // 当前世界地图 
	string _currentPlayWorldName;                        // 当前世界名字

	const static string _clientEdition;                  // 当前客户端版本
	static string _editionName;                          // 版本名称
	static bool _updateRequired;                         // 是否需要更新
	static DWORD _screenDisplayFrequency;                // 屏幕刷新率
};