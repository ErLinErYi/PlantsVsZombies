/**
 *Copyright (c) 2021 LZ.All Right Reserved
 *Author : LZ
 *Date: 2021.2.15
 *Email: 2117610943@qq.com
 */
#pragma once
#include "Scenes/GameScenes/Adventure/GameScene/LZAGSButtonLayer.h"

class BMButtonLayer :public GSButtonLayer
{
public:
	CREATE_FUNC(BMButtonLayer);

	/**
	 *添加到主场景中
	 */
	void addLayer(Node* node, const int order, const string& name) { node->addChild(this, order, name); }

CC_CONSTRUCTOR_ACCESS:
	BMButtonLayer();
	~BMButtonLayer();
	virtual bool init();

protected:
	virtual void createQuitDialog() override;
	virtual void createKeyBoardListener() override;
	virtual void createRequirementButton() override;
	virtual void showShovelBank() override;
	virtual void changeScrollViewOffset();

protected:
	Vec2 _offset;
};