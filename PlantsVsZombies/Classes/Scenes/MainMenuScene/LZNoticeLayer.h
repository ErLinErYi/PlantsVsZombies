/**
 *Copyright (c) 2024 LZ.All Right Reserved
 *Author : LZ
 *Date: 2024.01.26
 *Email: 2117610943@qq.com
 */
#pragma once
#include "cocos2d.h"
#include "LZDetailedListLayer.h"
#include "network/CCDownloader.h"

using namespace cocos2d;

class NoticeLayer :public DetailedList
{
public:
	CREATE_FUNC(NoticeLayer);

CC_CONSTRUCTOR_ACCESS:
	NoticeLayer();
	~NoticeLayer();
	virtual bool init() override;

protected:
	void onShowBackButton();
	void onShowNoticeText();
	void downloadNoticeText();

private:
	std::unique_ptr<network::Downloader> _downloader;
	Label* _noticeText;
};