/**
 *Copyright (c) 2022 LZ.All Right Reserved
 *Author : LZ
 *Date: 2022.01.19
 *Email: 2117610943@qq.com
 */
#pragma once
#include "Scenes/GameScenes/TestingGround/GameScene/LZTRankingListLayer.h"

class HRankingListLayer :public TRankingListLayer
{
public:
	CREATE_FUNC(HRankingListLayer);

	static HRankingListLayer* createLayer(bool notUploadData = true);
	
CC_CONSTRUCTOR_ACCESS:
	HRankingListLayer();
	~HRankingListLayer();

protected:
	virtual void onShowDifferentTitle() override;
	virtual void onDownloadRankingList() override;
	virtual void onUploadData() override;
	virtual void onCheckUploadButtonEnable() override;
	virtual void onSelectCsvFile(int id) override;
};
