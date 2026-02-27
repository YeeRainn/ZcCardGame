/**
 * @file LevelConfig的头文件
 * @brief 定义关卡配置的数据结构，用于从JSON加载
 */

#ifndef __LEVEL_CONFIG_H__
#define __LEVEL_CONFIG_H__

#include "cocos2d.h"
#include "utils/GameEnum.h"
#include <vector>

// 单张牌的配置
struct CardConfig {
    CardFaceType face;   // 牌面（如ACE, TWO...）
    CardSuitType suit;   // 花色（如CLUBS, HEARTS...）
    cocos2d::Vec2 position;  // 在主牌区的位置（屏幕坐标）,仅对主牌区有效
};

// 整个关卡配置
struct LevelConfig {
    std::vector<CardConfig> playfieldCards;  // 主牌区初始牌列表
    std::vector<CardConfig> stockCards;      // 备用牌堆初始牌列表（位置暂为(0,0)）
};

#endif // __LEVEL_CONFIG_H__