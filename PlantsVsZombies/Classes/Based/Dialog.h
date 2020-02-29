#pragma once
#include "cocos2d.h"
#include "Based/GlobalVariable.h"

#ifdef WIN32
#pragma execution_character_set("utf-8")
#endif

using namespace cocos2d;
using namespace cocos2d::experimental;

class Dialog :public Layer
{
public:

	/**
	 *设置鼠标监听
	 */
	virtual void setMouseListener(EventListenerMouse* listener);

protected:
	/**
	 *加载背景
	 */                    
	virtual void createBackGroundImage();

	/**
	 *创建触摸监听 
	 */           
	virtual EventListenerTouchOneByOne* createTouchtListener(Sprite* sprite);

	/**
	 *创建标签
	 */
	virtual Label* label(const std::string &name, const float& fontsize, Vec2 &vec2 = Vec2(0, 0), 
		const float& space = 0, const Color3B& color = Color3B::GREEN, const float& scale = 1);

	/**
	 *创建按钮上的标签 
	 */
	virtual void createLabel(Sprite* sprite, MenuItemImage* MenuItem, const std::string &name, 
		Vec2 &vec2, float& fontsize, const float& space = 0, const Color3B& color = Color3B::GREEN);

	/**
	 *创建屏蔽层
	 */
	virtual void createShieldLayer(Node* node);

	/**
	 *删除层
	 */
	virtual void deleteDialog() { _dialogNode->removeFromParent(); }

	/**
	 *设置鼠标监听是否可用
	 */
	virtual void setMouseListenerEnable(bool isEnable = true);

CC_CONSTRUCTOR_ACCESS:
	Dialog();
	~Dialog();

protected:
	Global* _global;
	Node* _dialogNode;
	EventListenerMouse* _mouseListener;

private:
	Vec2 _phasePosition; /* 相差位置 */
	EventListenerTouchOneByOne* _shieldListener;
};
