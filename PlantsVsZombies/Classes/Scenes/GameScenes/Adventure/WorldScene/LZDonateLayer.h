/**
 *Copyright (c) 2021 LZ.All Right Reserved
 *Author : LZ
 *Date: 2021.6.28
 *Email: 2117610943@qq.com
 */
#pragma once
#include "Based/LZDialog.h"

class DonateLayer : public Dialog
{
public:
	CREATE_FUNC(DonateLayer);
	virtual bool init() override;

CC_CONSTRUCTOR_ACCESS:
	DonateLayer();
	~DonateLayer();

protected:
	virtual void createDiglog();	  /* 创建对话框 */
	virtual void createText();       /* 创建文本 */
	virtual void deleteDialog();     /* 删除对话框 */
	virtual void createButtons();    /* 创建按钮 */

protected:
	Sprite* _image;
};