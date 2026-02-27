/**
 * @file GameController.cpp
 * @brief GameController的实现文件，定义了GameController类的成员函数
 */

#include "GameController.h"
#include "configs/loaders/LevelConfigLoader.h"
#include "services/GameModelFromLevelGenerator.h"

// 构造函数，初始化成员变量
GameController::GameController() : 
    _gameModel(nullptr), 
    _gameView(nullptr), 
    _undoManager(nullptr),
    _playfieldController(nullptr), 
    _trayController(nullptr),
    _stockController(nullptr){
}

// 析构函数，释放资源  _gameView 由 Cocos 自动释放（如果已加入节点）
GameController::~GameController() {
    CC_SAFE_DELETE(_gameModel);
    CC_SAFE_DELETE(_undoManager);
    CC_SAFE_DELETE(_playfieldController);
    CC_SAFE_DELETE(_trayController);
    CC_SAFE_DELETE(_stockController);
}

void GameController::startGame(int levelId) {
    // 加载关卡配置,暂时固定为 level.json
    std::string configPath = "config/level.json";
    LevelConfig config = LevelConfigLoader::load(configPath);

    // 根据配置生成游戏数据模型
    _gameModel = GameModelFromLevelGenerator::generate(config);

    // 创建撤销管理器
    _undoManager = new UndoManager();

    // 创建游戏视图
    _gameView = GameView::create(_gameModel);
    _gameView->buildViews();

    // 创建子控制器
    _playfieldController = new PlayFieldController();
    _trayController = new TrayController();
    _stockController = new StockController(); // <- 必须先实例化

    // 初始化子控制器
    _playfieldController->init(_gameModel, _undoManager, _gameView);
    _trayController->init(_gameModel, _undoManager, _gameView);
    _stockController->init(_gameModel, _undoManager, _gameView);

    // 设置点击回调：将点击事件分发给对应的控制器
    _gameView->setCardClickCallback([this](CardView* cardView) {
        CardModel* model = cardView->getModel();
        switch (model->getRegion()) {
            case CardRegionType::CRT_PLAYFIELD:
                _playfieldController->onCardClicked(model->getId());
                break;
            case CardRegionType::CRT_TRAY:
                _trayController->onCardClicked(model->getId());
                break;
            case CardRegionType::CRT_STOCK:     //备用牌堆处理
                _stockController->onCardClicked(model->getId());
                break;
            default:
                break;
            }
    });
}

// 处理回退按钮点击事件
void GameController::onUndoButtonClicked() {
    if (!_undoManager->canUndo()) return;   // 无记录可撤销

    UndoRecord record = _undoManager->popRecord();  // 弹出最近记录

    // 根据操作类型调用对应的撤销方法
    if (record.type == UndoType::TRAY_SWAP) {
        _trayController->undoSwap(record);
    }else if (record.type == UndoType::PLAYFIELD_TO_TRAY) {
        _playfieldController->undoPlayfieldToTray(record);
    }else if (record.type == UndoType::STOCK_TO_TRAY) {
        _stockController->undoStockToTray(record);
    }
}