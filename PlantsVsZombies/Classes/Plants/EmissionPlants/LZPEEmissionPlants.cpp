/**
 *Copyright (c) 2020 LZ.All Right Reserved
 *Author : LZ
 *Date: 2020.2.6
 *Email: 2117610943@qq.com
 */

#include "LZPEEmissionPlants.h"

EmissionPlants::EmissionPlants():
    _isChanged(false)
,   _bulletAnimation(nullptr)
{
}

EmissionPlants::~EmissionPlants()
{
}

void EmissionPlants::cavePlantInformation(rapidjson::Value& object, rapidjson::Document::AllocatorType& allocator)
{
    object.AddMember("isChanged", _isChanged, allocator);
}

void EmissionPlants::readPlantInforamtion(rapidjson::Document* levelDataDocument, char* key, int i)
{
    _isChanged = (*levelDataDocument)[key]["Plants"][to_string(i).c_str()]["isChanged"].GetBool();
}