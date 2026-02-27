/**
 * @file PlayFieldController.h
 * @brief 主牌区控制器，处理桌面牌点击匹配及撤销。
 */

#ifndef __PLAYFIELD_CONTROLLER_H__
#define __PLAYFIELD_CONTROLLER_H__

#include "cocos2d.h"
#include "models/GameModel.h"
#include "managers/UndoManager.h"
#include "views/GameView.h"

class PlayFieldController {
public:
    void init(GameModel* model, UndoManager* undoManager, GameView* gameView);
    bool onCardClicked(int cardId);
    void undoPlayfieldToTray(const UndoRecord& record);

private:
    GameModel* _model;
    UndoManager* _undoManager;
    GameView* _gameView;
};

#endif // __PLAYFIELD_CONTROLLER_H__