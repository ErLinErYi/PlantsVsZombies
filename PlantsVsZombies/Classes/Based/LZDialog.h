﻿/**
 *Copyright (c) 2021 LZ.All Right Reserved
 *Author : LZ
 *Email: 2117610943@qq.com
 */

#pragma once
#include "cocos2d.h"
#include "Based/LZGlobalVariable.h"

#ifdef WIN32
#pragma execution_character_set("utf-8")
#endif

using namespace cocos2d;
using namespace cocos2d::experimental;

class Dialog :public LayerColor
{
public:
	CREATE_FUNC(Dialog);
	/**
	 *创建触摸监听
	 */
	static EventListenerTouchOneByOne* createTouchtListener(Sprite* sprite);

	/**
	 *显示对话框出现动画 
	 */
	static void showDialogAction(Sprite* sprite);

	/**
	 *显示对话框消失动画 
	 */
	static void showDeleteDialogAction(Sprite* sprite);

	/**
	 *设置鼠标监听
	 */
	virtual void setMouseListener(EventListenerMouse* listener);

    /**
	 * 设置文本 
	 */
	virtual void setString(string str);

	/**
	 * 创建按钮
	 */
	virtual void createButtons(string name, float position, int id);

	/**
	 * 获取返回值 
	 */
	virtual void getData(const std::function<void(bool flag)>& pSelector);

protected:
	/**
	 *创建标签
	 */
	virtual Label* label(const std::string &name, const float& fontsize, const Vec2 &vec2 = Vec2(0, 0), 
		const float& space = 0, const Color3B& color = Color3B::GREEN, const float& scale = 1);

	/**
	 *创建按钮上的标签 
	 */
	virtual void createLabel(Sprite* sprite, MenuItemImage* MenuItem, const std::string &name, 
		const Vec2 &vec2, float& fontsize, const float& space = 0, const Color3B& color = Color3B::GREEN);

	/**
	 *创建屏蔽层
	 */
	virtual void createShieldLayer(Node* node);

	/**
	 *删除层
	 */
	virtual void deleteDialog();

	/**
	 *设置鼠标监听是否可用
	 */
	virtual void setMouseListenerEnable(bool isEnable = true);

	/**
	 *  创建对话框
	 */
	virtual void createDialog();        	     

	
	virtual void createText();

CC_CONSTRUCTOR_ACCESS:
	Dialog();
	virtual ~Dialog();
	virtual bool init() override;

protected:
	Global* _global;
	EventListenerMouse* _mouseListener;
	Sprite* _dialog;
	string _strText;
	std::function<void(bool flag)> selectButtonCallBack;

private:
	Vec2 _phasePosition; /* 相差位置 */
	EventListenerTouchOneByOne* _shieldListener;
};