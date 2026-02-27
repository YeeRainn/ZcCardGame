/**
 * @file UndoRecord.h
 * @brief 定义撤销记录的数据结构，用于存储一次操作前后的状态
 */

#ifndef __UNDO_RECORD_H__
#define __UNDO_RECORD_H__

#include "cocos2d.h"
#include "CardModel.h"

// 操作类型
enum class UndoType {
    TRAY_SWAP,          // 手牌区内替换（点击非顶部牌替换顶部牌）
    PLAYFIELD_TO_TRAY,   // 桌面牌移入替换手牌区顶部牌
    STOCK_TO_TRAY       // 备用牌堆抽牌替换
};

// 一条撤销记录
struct UndoRecord {
    UndoType type;              // 操作类型
    int cardId1;                // 操作的主要牌ID（移动的牌）
    int cardId2;                // 被替换的牌ID
    cocos2d::Vec2 oldPos1;      // 移动前位置
    cocos2d::Vec2 oldPos2;      // 被替换牌移动前位置
    cocos2d::Vec2 newPos1;      // 移动后位置（用于反向移动）
    cocos2d::Vec2 newPos2;      // 被替换牌移动后位置（实际消失，但记录原位置供撤销）
};

#endif // __UNDO_RECORD_H__