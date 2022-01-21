/**
 *Copyright (c) 2022 LZ.All Right Reserved
 *Author : LZ
 *Date: 2022.01.18
 *Email: 2117610943@qq.com
 */
#pragma once
#include "Scenes/GameScenes/IZombies/GameScene/LZIRankingListLayer.h"

class TRankingListLayer :public IRankingListLayer
{
public:
	CREATE_FUNC(TRankingListLayer);

	static TRankingListLayer* createLayer(bool notUploadData = true);

CC_CONSTRUCTOR_ACCESS:
	TRankingListLayer();
	~TRankingListLayer();
	
protected:
	virtual void onShowDifferentTitle() override;
	virtual void onShowBackButton() override;
	virtual void onUploadData() override;
	virtual void onSelectCsvFile(int id) override;
	virtual void onAddLocalData() override;
};
