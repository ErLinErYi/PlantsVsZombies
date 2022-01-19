/**
 *Copyright (c) 2022 LZ.All Right Reserved
 *Author : LZ
 *Date: 2022.01.14
 *Email: 2117610943@qq.com
 */

#pragma once
#include "cocos2d.h"
#include "Based/LZDialog.h"
#include "network/CCDownloader.h"

using namespace cocos2d;

class MomentTime;
class CSVFile;
class IRankingListLayer :public Dialog
{
public:
	CREATE_FUNC(IRankingListLayer);

	static IRankingListLayer* createLayer(bool notUploadData = true);

CC_CONSTRUCTOR_ACCESS:
	IRankingListLayer();
	~IRankingListLayer();
	virtual bool init() override;

protected:
	virtual void onShowTitle();
	virtual void onShowTitleButton(int id);
	virtual void onShowDifferentTitle();
	virtual void onShowBackButton();
	virtual void onCreateScrollView();
	virtual void onDownloadRankingList();
	virtual void onParseCsvData();
	virtual void onMouseEvent();
	virtual void onShowLoadingText();
	virtual void onUploadData();
	virtual void onShowUploadDataRequirement();
	virtual void onShowRankingListUpdateInformation();
	virtual void onCheckUploadButtonEnable();
	virtual void onSuccessfulFeedback();
	virtual void onSelectCsvFile(int id);

protected:
	static bool notUploadData;
	int _mostLevel;
	CSVFile* _csvFile;
	DrawNode* _draw;
	string _strRankingList;
	cocos2d::ui::Text* _loadingText;
	cocos2d::ui::Button* _uploadButton;
	MomentTime* _nowNettime;
	cocos2d::ui::ScrollView* _rankingListScrollView;
	std::unique_ptr<network::Downloader> _downloader;
};