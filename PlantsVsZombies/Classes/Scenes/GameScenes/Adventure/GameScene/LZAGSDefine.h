/**
 *Copyright (c) 2020 LZ.All Right Reserved
 *Author : LZ
 *Date: 2020.2.5
 *Email: 2117610943@qq.com
 */

#pragma once

 /*
     游戏地图示意图

    CARDBAR
       |
     +---+-----------------------------------------------+
     |   |    5 +-----------------top--------------+     |
     |   |    4 |----            810               |     |
     |   |    3 |----                              |     |
    0|   |220 2 |----570    GRASSPOSITION      1668|right|
     |   |    1 |----                              |     |
     |   |    0 |----           110                |     |
     |   |      +----------------bottom------------+     |
     +---+-----------------------------------------------+

 */
 //@ define map position 
#define GRASS_POSITION_LEFT           570
#define GRASS_POSITION_RIGHT          1668
#define GRASS_POSITION_TOP            810
#define GRASS_POSITION_BOTTOM         110
#define CARD_BAR_RIGHT                220
#define ROW                           4
#define COLUMN                        8

#define GRASS_BIGMAP_POSITION_LEFT    1590
#define GRASS_BIGMAP_POSITION_RIGHT   3780
#define GRASS_BIGMAP_POSITION_TOP     1605
#define GRASS_BIGMAP_POSITION_BOTTOM  235
#define BIGMAP_ROW                    9
#define BIGMAP_COLUMN                 17

// row_column->position/position->row_column
#define ROW_COLUMN_TO_POSITION(__ROW_COLUMN__) Vec2(GRASS_POSITION_LEFT + 122 * __ROW_COLUMN__.x + 60,GRASS_POSITION_BOTTOM + 138 * (__ROW_COLUMN__.y + 1) - 125)
#define POSITION_TO_ROW_COLUMN(__POSITION__) Vec2((-GRASS_POSITION_LEFT + __POSITION__.x - 60)/ 122,(-GRASS_POSITION_BOTTOM + __POSITION__.y -125)/ 138 - 1)
#define POSITION_TO_COLUMN(__POSITION__,__X__) __POSITION__.y > 110 + 138 * __X__ && __POSITION__.y < 110 + 138 * (__X__ + 1)

#define BIGMAP_ROW_COLUMN_TO_POSITION(__ROW_COLUMN__) Vec2(GRASS_BIGMAP_POSITION_LEFT + 121 * __ROW_COLUMN__.x + 60,GRASS_BIGMAP_POSITION_BOTTOM + 136 * (__ROW_COLUMN__.y + 1) - 125)
#define BIGMAP_POSITION_TO_ROW_COLUMN(__POSITION__) Vec2((-GRASS_BIGMAP_POSITION_LEFT + __POSITION__.x - 60)/ 121,(-GRASS_BIGMAP_POSITION_BOTTOM + __POSITION__.y -125)/ 136 - 1)

// judge map
#define GRASS_INSIDE(__POSITION__,__X__,__Y__) (__POSITION__.x > GRASS_POSITION_LEFT + 122 * __Y__ && __POSITION__.x < GRASS_POSITION_LEFT + 122 * (__Y__ + 1) && __POSITION__.y > GRASS_POSITION_BOTTOM + 138 * __X__ && __POSITION__.y < GRASS_POSITION_BOTTOM + 138 * (__X__ + 1))
#define GRASS_OUTSIDE(__POSITION__) (__POSITION__.x < GRASS_POSITION_LEFT || __POSITION__.x>GRASS_POSITION_RIGHT || __POSITION__.y < GRASS_POSITION_BOTTOM || __POSITION__.y>GRASS_POSITION_TOP)

#define GRASS_BIGMAP_INSIDE(__POSITION__,__X__,__Y__) (__POSITION__.x > GRASS_BIGMAP_POSITION_LEFT + 121 * __Y__ && __POSITION__.x < GRASS_BIGMAP_POSITION_LEFT + 121 * (__Y__ + 1) && __POSITION__.y > GRASS_BIGMAP_POSITION_BOTTOM + 136 * __X__ && __POSITION__.y < GRASS_BIGMAP_POSITION_BOTTOM + 136 * (__X__ + 1))
#define GRASS_BIGMAP_OUTSIDE(__POSITION__) (__POSITION__.x < GRASS_BIGMAP_POSITION_LEFT || __POSITION__.x>GRASS_BIGMAP_POSITION_RIGHT || __POSITION__.y < GRASS_BIGMAP_POSITION_BOTTOM || __POSITION__.y>GRASS_BIGMAP_POSITION_TOP)

// calculator loaclzorder
#define SET_ANIMATION_Z_ORDER(__POSITION__) (50 - __POSITION__.y) * 100 + 50 - __POSITION__.x
/* 
 * .....
 * 4850 4849 4848......
 * 4950 4949 4948 .....
 * 5050 5049 5048 .....
 */

// calculator tag
#define SET_TAG(__POSITION__) __POSITION__.y * 100 + __POSITION__.x
/*
 * 0   1   2   3   ......
 * 100 101 102 103 ......
 * 200 201 202 203 ......
 * .....
 */

// read level data
#define READ_LEVEL_DATA OpenLevelData::getInstance()->readLevelData(OpenLevelData::getInstance()->getLevelNumber())

// 地图初始化
#define MAP_INIT(__MAP__,__NUMBER__)  for(auto& __ROW__ : __MAP__) { for(auto& __COLUMN__ : __ROW__ ) { __COLUMN__ = __NUMBER__;} }
#define MAP_CAN_NOT_PLANT(__MAP__) do{ if(READ_LEVEL_DATA->getIsNoPlants()){ for(auto& __CAN__NOT__PLANT__ : READ_LEVEL_DATA->getNoPlantsPosition()){__MAP__[__CAN__NOT__PLANT__.y][__CAN__NOT__PLANT__.x] = CAN_NOT_PLANT;}}}while(0)

// 地图没有种植
#define NO_PLANTS -1
#define CAN_NOT_PLANT -2

#define SET_OUT_MAP Vec2(-10000,-10000)
// cursorvisible
#define CURSOR_VISIBLE(__TRUE__OR__FALSE__) if(_global->userInformation->getIsSelectCursorNotHide() == cocos2d::ui::CheckBox::EventType::UNSELECTED)  _director->getOpenGLView()->setCursorVisible(__TRUE__OR__FALSE__);

// 空格暂停/恢复 控制
#define KEY_PRESS(__NUMBER__) static_cast<bool>((++__NUMBER__)%2)

// delete group
#define DELETE_MAP_GROUP(__MAP__GROUP__) do{ for(auto& __ITERATOR__ : __MAP__GROUP__){ delete __ITERATOR__.second; __ITERATOR__.second = nullptr;} __MAP__GROUP__.clear(); }while(0)
#define DELETE_LIST_GROUP(__LIST__GROUP__) do{ for(auto __ITERATOR__ : __LIST__GROUP__){ __ITERATOR__->releaseFunction(); delete __ITERATOR__; __ITERATOR__ = nullptr;} __LIST__GROUP__.clear(); }while(0)