/**
 *Copyright (c) 2019 LZ.All Right Reserved
 *Author : LZ
 *Date: 2019.7.17
 *Modify: 2020.2.3
 *Email: 2117610943@qq.com
 */

#pragma once
#include "Based/Dialog.h"
#include "cocos2d.h"
#include "cocos-ext.h"
#include "Scenes/EasterEggsScene/GameEasterEggs.h"


#ifdef WIN32
#pragma execution_character_set("utf-8")
#endif

using namespace ui;
using namespace cocos2d;
using namespace cocos2d::experimental;
using namespace cocos2d::extension;

enum class OptionScene_CheckBox
{
	显示信息 = 1,
	全屏,
	高帧率,
	鼠标隐藏,
	拉伸显示,
	缓入动画,
	垂直同步
};

enum class OptionScene_Slider
{
	音乐 = 1,
	音效
};

class OptionsMenu :public Dialog
{
public:
	CREATE_FUNC(OptionsMenu);
	
CC_CONSTRUCTOR_ACCESS:
	OptionsMenu();
	~OptionsMenu();
	virtual bool init() override;
	
protected:
	virtual void createDialog();                                                                                      /* 创建对话框 */
	virtual CheckBox* createCheckBox(Vec2&, Vec2&, const std::string&, OptionScene_CheckBox, const std::string,
		const std::string, const bool IsNew = false);                                                                 /* 创建复选框 */
	virtual ControlSlider* createSlider(Vec2&, Vec2&, const std::string&, OptionScene_Slider, Sprite*, Sprite*,
		Sprite*, Sprite* = nullptr, const bool IsNew = false);                                                        /* 创建滑动条 */
	virtual void deleteDialog() override;                                                                             /* 删除对话框 */

private:
	void createButton();
	void backGroundMusicVolumeChangeCallBack(Ref* sender, Control::EventType type);                                  /* 背景音乐音量调节回调函数*/
	void soundEffectMusicVolumeChangeCallBack(Ref* sender, Control::EventType type);                                 /* 音效音乐音量调节回调函数*/

protected:
	Sprite* _option;         /* 对话框 */
	Director* _director;

private:
	UserDefault* _userDefault;
};