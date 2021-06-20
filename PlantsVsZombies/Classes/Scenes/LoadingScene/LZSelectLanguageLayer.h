/**
 *Copyright (c) 2021 LZ.All Right Reserved
 *Author : LZ
 *Date: 2021.01.02
 *Email: 2117610943@qq.com
 */

#pragma once
#include "cocos2d.h"
#include "json/document.h"

#include "Based/LZDialog.h"

using namespace cocos2d;

class SelectLanguage :public Dialog
{
public:
	CREATE_FUNC(SelectLanguage);

CC_CONSTRUCTOR_ACCESS:
	SelectLanguage();
	~SelectLanguage();
	virtual bool init() override;

private:
	void createDialog();
	void showLanguageOptionBox();
	void reloadLanguageText(const char* language);
	void setNewLanguage();
	void createButtons(Sprite* sprite, const std::string& Label, const Vec2& vec2, const float& scale, const int& ID);
	void onChangedRadioButtonSelect(RadioButton* radioButton, RadioButton::EventType type);
	void createMouseListener();

private:
	Sprite* _selectLanguageDialog;
	string _selectLanguageName;
	ui::ScrollView* _scrollView;
	rapidjson::Document _document;
};