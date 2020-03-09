#pragma once
#include "cocos2d.h"
#include "Based/GlobalVariable.h"

#ifdef WIN32
#pragma execution_character_set("utf-8")
#endif

using namespace cocos2d;
using namespace cocos2d::ui;
using namespace cocos2d::experimental;

enum class GameTypes;

class UserWinRequirement :public Node
{
public:
	static UserWinRequirement* UserWinRequirement::create(Node* node);
	void createDialogBox(GameTypes finishedid);
	void setParent(Node* node);
	void setButtonVisible(bool visible);
	void setDialogOpacity(const int opacity);
	void setListenerEnable(bool enable);
	void setShowDialogAction();
	void setDelectDialogAction();
	Sprite* getDialog() const;

CC_CONSTRUCTOR_ACCESS:
	UserWinRequirement(Node* node = nullptr);
	~UserWinRequirement();

private:
	void showRequirement(GameTypes finishedid);                         /* 显示要求 */
	void showText(const string& text, const int& ID, Color3B color = Color3B::BLACK);
	void createShieldLayer();

private:
	Node* _node;
	Global* _global;
	Button* _continuesGame;
	Scale9Sprite* _levelObjiectives;
	EventListenerTouchOneByOne* _listener;
	EventListenerTouchOneByOne* _shieldListener;
	Vec2 _phasePosition;                                                 /* 相差位置 */

	Vec2 _textPosition[4][4] =                                           /* 文字位置 */
	{
		{Vec2(100,135),},
		{Vec2(100,110),Vec2(100,160),},
		{Vec2(100,90),Vec2(100,135),Vec2(100,180)},
		{Vec2(100,80),Vec2(100,115),Vec2(100,150),Vec2(100,185)}
	};
};