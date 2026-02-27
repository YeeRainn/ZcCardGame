/**
 * @file TrayController.h
 * @brief 手牌区控制器，处理手牌区卡牌的点击事件和撤销。
 */

#ifndef __TRAY_CONTROLLER_H__
#define __TRAY_CONTROLLER_H__

#include "cocos2d.h"
#include "models/GameModel.h"
#include "managers/UndoManager.h"
#include "views/GameView.h"

class TrayController {
public:
    /**
     * @brief 初始化控制器
     * @param model 游戏数据模型
     * @param undoManager 撤销管理器
     * @param gameView 游戏视图（用于播放动画）
     */
    void init(GameModel* model, UndoManager* undoManager, GameView* gameView);

    /**
     * @brief 处理手牌区卡牌点击
     * @param cardId 被点击的卡牌ID
     * @return true表示事件被处理
     */
    bool onCardClicked(int cardId);

    /**
     * @brief 撤销手牌区内部替换操作
     * @param record 对应的撤销记录
     */
    void undoSwap(const UndoRecord& record);

private:
    GameModel* _model;
    UndoManager* _undoManager;
    GameView* _gameView;
};

#endif // __TRAY_CONTROLLER_H__