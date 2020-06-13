/**
 *Copyright (c) 2019 LZ.All Right Reserved
 *Author : LZ
 *Date: 2019.7.17
 *Modify: 2020.2.3
 *Email: 2117610943@qq.com
 */

#pragma once
#include "cocos2d.h"
#include "Based/Dialog.h"
#include "cocos-ext.h"
#include "ui/CocosGUI.h"

#ifdef WIN32
#pragma execution_character_set("utf-8")
#endif

using namespace ui;
using namespace cocos2d;
using namespace cocos2d::experimental;
using namespace cocos2d::extension;

class InputDataMenu :public Dialog
{
public:
	CREATE_FUNC(InputDataMenu);

CC_CONSTRUCTOR_ACCESS:
	InputDataMenu();
	~InputDataMenu();
	virtual bool init();

protected:
	virtual void deleteDialog() override;

private:
	/* 创建对话框 */
	void createDialog();
	void CreateInputDialog();

	/* 创建按钮 */
	void createButtons(Sprite* sprite, const std::string &Label, Vec2 &vec2, const float& scale, const int& ID);
	void createDataButton(const std::string& Label, Vec2& vec2, const float& scale, const int& ID);
	void createShieldLayer();
	void setButtonColor(Button* button);
	void setButtonText(Button* button, const string& text, const float& scale);
	void caveData();
	void updateButtonText();
	void inputNewFileDataName();
	Label* onTextFieldInsertText();
	Label* onTextFieldDeleteBackward();

private:
	LayerColor* _shieldDialogLayer;
	Sprite* _caveFileDialog;
	Sprite* _inputCursor;
	Sprite* _inputDataDialog;                                /* 对话框 */
	EditBox* _editbox;                                       /* 输入框 */
	TextField* _textField;
	Vec2  _beginPos;
	string _inputString, _newInputstring, _lastString;
	int _caveFileNumber;
	map<int, Button*> _dataButton;
	EventListenerTouchOneByOne* _shieldListener;
};
