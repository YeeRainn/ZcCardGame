/**
 * @file StockController.h
 * @brief 备用牌堆控制器，处理点击备用牌堆抽牌及撤销
 */

#ifndef __STOCK_CONTROLLER_H__
#define __STOCK_CONTROLLER_H__

#include "cocos2d.h"
#include "models/GameModel.h"
#include "managers/UndoManager.h"
#include "views/GameView.h"

class StockController {
public:
    /**
     * @brief 初始化控制器
     * @param model 游戏数据模型
     * @param undoManager 撤销管理器
     * @param gameView 游戏视图
     */
    void init(GameModel* model, UndoManager* undoManager, GameView* gameView);

    /**
     * @brief 处理备用牌堆点击事件
     * @param cardId 被点击的卡牌ID（这里通常是备用牌堆最上面那张牌的ID）
     * @return true表示事件被处理
     */
    bool onCardClicked(int cardId);

    /**
     * @brief 撤销从备用牌堆抽牌的操作
     * @param record 对应的撤销记录
     */
    void undoStockToTray(const UndoRecord& record);

    //GameModel* _model;

private:
    GameModel* _model;
    UndoManager* _undoManager;
    GameView* _gameView;
};

#endif // __STOCK_CONTROLLER_H__