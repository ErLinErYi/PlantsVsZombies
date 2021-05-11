/**
 *Copyright (c) 2021 LZ.All Right Reserved
 *Author : LZ
 *Date: 2021.2.17
 *Email: 2117610943@qq.com
 */
#pragma once
#include "Scenes/MainMenuScene/LZUnlockDialogLayer.h"

class SWSUnlockDialogLayer : public UnlockDialogLayer
{
public:
	CREATE_FUNC(SWSUnlockDialogLayer);
	virtual bool init() override;

CC_CONSTRUCTOR_ACCESS:
	SWSUnlockDialogLayer();

protected:
	virtual void createDiglog() override;	  /* 创建对话框 */
	virtual void createText() override;       /* 创建文本 */
	virtual void deleteDialog() override;     /* 删除对话框 */
	virtual void createButtons() override;    /* 创建按钮 */

public:
	static unsigned int unlockNeedNumbers;
};