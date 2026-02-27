/**
 * @file GameController.h
 * @brief 游戏主控制器，负责整体流程协调。
 */

#ifndef __GAME_CONTROLLER_H__
#define __GAME_CONTROLLER_H__

#include "cocos2d.h"
#include "models/GameModel.h"
#include "views/GameView.h"
#include "managers/UndoManager.h"
#include "controllers/PlayFieldController.h"
#include "controllers/TrayController.h"
#include "controllers/StockController.h"

// 游戏控制器，负责协调模型、视图和其他控制器的交互
class GameController {
public:

	// 构造函数和析构函数
    GameController();
    ~GameController();

	//启动游戏，加载指定关卡的数据并初始化模型和视图，具有可扩展性
    void startGame(int levelId);

    // 获取游戏视图
    GameView* getGameView() const { 

        return _gameView; 
        
    }

    // 处理回退按钮点击事件
    void onUndoButtonClicked();

private:
    GameModel* _gameModel;
    GameView* _gameView;
    UndoManager* _undoManager;

    PlayFieldController* _playfieldController;
    TrayController* _trayController;
    StockController* _stockController;
};

#endif // __GAME_CONTROLLER_H__