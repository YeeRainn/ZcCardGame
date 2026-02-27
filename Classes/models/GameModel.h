/**
 * @file GameModel.h头文件
 * @brief 游戏全局数据模型，包含所有牌以及当前状态
 */

#ifndef __GAME_MODEL_H__
#define __GAME_MODEL_H__

#include "CardModel.h"
#include <vector>

class GameModel {
public:
    GameModel();

    void reset();   // 清空所有容器（用于重新开始）

    // 获取各个区域的牌列表（返回引用，便于直接修改）
    std::vector<CardModel*>& getPlayfieldCards() { 
        return _playfieldCards; 
    }
    std::vector<CardModel*>& getTrayCards() { 
        return _trayCards; 
    }
    std::vector<CardModel*>& getStockCards() { 
        return _stockCards; 
    }
    std::vector<CardModel*>& getRemovedCards() { 
        return _removedCards; 
    }

    // 获取手牌区顶部牌（即列表中的最后一张）
    CardModel* getTopTrayCard() const;

    // 通过ID查找牌（遍历所有区域）
    CardModel* getCardById(int id);

    // 纸牌位置布局
    /**
     * @brief 重新计算备用牌堆所有牌的位置，形成叠放效果。
     *        最多展示三张牌，后面的牌与第三张重叠。
     *        每张牌相对于前一张水平偏移139像素。
     */
    void layoutStockCards();

    /**
     * @brief 桌面牌所有牌的位置整体水平居中于桌面牌区。
     *        保持牌之间的相对水平距离不变，只做整体平移。
     */
    void layoutPlayfieldCards();

private:
    std::vector<CardModel*> _playfieldCards;    // 主牌区
    std::vector<CardModel*> _trayCards;         // 手牌区（底牌堆）
    std::vector<CardModel*> _stockCards;        // 备用牌堆
    std::vector<CardModel*> _removedCards;      // 存放被替换（消除）的牌，用于回退
};

#endif // __GAME_MODEL_H__