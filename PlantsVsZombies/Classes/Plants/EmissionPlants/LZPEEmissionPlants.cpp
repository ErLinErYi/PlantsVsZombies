/**
 *Copyright (c) 2020 LZ.All Right Reserved
 *Author : LZ
 *Date: 2020.2.6
 *Email: 2117610943@qq.com
 */

#include "LZPEEmissionPlants.h"
#include "Based/LZBPlayMusic.h"

EmissionPlants::EmissionPlants():
    _isChanged(false)
,   _isCreateBullet(false)
,   _isHaveZombies(false)
,   _bulletAnimation(nullptr)
{
}

EmissionPlants::~EmissionPlants()
{
}

void EmissionPlants::createListener(std::string animationName, std::string actionName)
{
    _plantAnimation->setEventListener([=](spTrackEntry* entry, spEvent* event)
        {
            if (!actionName.compare(event->data->name))
            {
                if (!_isCreateBullet)
                {
                    rand() % 2 == 0 ? PlayMusic::playMusic("throw") : PlayMusic::playMusic("throw2");
                    createBullet();
                }
            }
        });

    _plantAnimation->setCompleteListener([=](spTrackEntry* entry)
        {
            if (!animationName.compare(entry->animation->name))
            {
                _isCreateBullet = false;
            }
        });
}

void EmissionPlants::cavePlantInformation(rapidjson::Value& object, rapidjson::Document::AllocatorType& allocator)
{
    object.AddMember("isChanged", _isChanged, allocator);
    object.AddMember("isCreateBullet", _isCreateBullet, allocator);
}

void EmissionPlants::readPlantInforamtion(rapidjson::Document* levelDataDocument, char* key, int i)
{
    _isChanged = (*levelDataDocument)[key]["Plants"][to_string(i).c_str()]["isChanged"].GetBool();
    _isCreateBullet = (*levelDataDocument)[key]["Plants"][to_string(i).c_str()]["isCreateBullet"].GetBool();
}