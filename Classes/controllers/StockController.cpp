/**
 * @file StockController.cpp
 * @brief StockController的实现，处理抽牌逻辑,备用牌堆控制器，抽牌和撤销，并在操作后更新布局
 */

#include "StockController.h"
#include "models/UndoRecord.h"
#include <algorithm>

void StockController::init(GameModel* model, UndoManager* undoManager, GameView* gameView) {
    _model = model;
    _undoManager = undoManager;
    _gameView = gameView;
}

bool StockController::onCardClicked(int cardId) {
    // 获取备用牌堆列表
    auto& stock = _model->getStockCards();
    if (stock.empty()) return false;  // 无牌可抽

    // 获取手牌区顶部牌
    CardModel* topTray = _model->getTopTrayCard();
    if (!topTray) return false;      

    // 取备用牌堆顶部牌（列表最后一个元素）
    CardModel* stockCard = stock.back();    // 最上面的牌
    if (stockCard->getId() != cardId) return false;

    // 创建撤销记录
    UndoRecord record;
    record.type = UndoType::STOCK_TO_TRAY;
    record.cardId1 = stockCard->getId();   // 从备用堆抽出的牌
    record.cardId2 = topTray->getId();     // 被替换的顶部牌
    record.oldPos1 = stockCard->getPosition();  // 备用牌堆位置
    record.oldPos2 = topTray->getPosition();    // 手牌区顶部牌位置
    record.newPos1 = topTray->getPosition();    // 抽出的牌要去的位置
    record.newPos2 = stockCard->getPosition();  // 被替换牌的原位置（用于撤销时放回）

    // 更新模型,从备用牌堆移除抽出的牌
    stock.pop_back();

    // 从手牌区移除顶部牌
    auto& tray = _model->getTrayCards();
    auto itTray = std::find(tray.begin(), tray.end(), topTray);
    if (itTray != tray.end()) tray.erase(itTray);

    // 将抽出的牌加入手牌区成为新顶部，并设为正面朝上
    tray.push_back(stockCard);
    stockCard->setRegion(CardRegionType::CRT_TRAY);
    //stockCard->setFaceUp(true);                // 翻到正面
    stockCard->setPosition(topTray->getPosition()); // 设置位置到原顶部牌位置

    // 将被替换的顶部牌放入移除列表（用于撤销）
    topTray->setRegion(CardRegionType::CRT_NONE);
    _model->getRemovedCards().push_back(topTray);

    // 重新布局备用牌堆（剩余牌重新排位）
    _model->layoutStockCards();
    // 通知视图更新备用牌堆牌的显示位置
    _gameView->updateStockLayout();

    // 保存撤销记录
    _undoManager->pushRecord(record);

    // 播放抽牌动画（抽出的牌从备用堆位置移动到原顶部牌位置，同时原顶部牌消失）
    _gameView->playStockToTrayAnimation(stockCard->getId(), topTray->getId(), [this, topTray]() {
        // 动画完成后，从视图中移除被替换的牌的视图
        _gameView->removeCardView(topTray->getId());
    });

    return true;
}

void StockController::undoStockToTray(const UndoRecord& record) {
    // 获取两张牌
    CardModel* card1 = _model->getCardById(record.cardId1); // 当前在手牌区的牌（从备用堆抽出的那张）
    CardModel* card2 = _model->getCardById(record.cardId2); // 被移除的顶部牌
    if (!card1 || !card2) return;

    // 从移除列表恢复 card2
    auto& removed = _model->getRemovedCards();
    auto it = std::find(removed.begin(), removed.end(), card2);
    if (it != removed.end()) removed.erase(it);

    // 从手牌区移除 card1
    auto& tray = _model->getTrayCards();
    auto it1 = std::find(tray.begin(), tray.end(), card1);
    if (it1 != tray.end()) tray.erase(it1);

    // 将 card1 放回备用牌堆
    _model->getStockCards().push_back(card1);
    card1->setRegion(CardRegionType::CRT_STOCK);
    //card1->setFaceUp(false);  // 备用牌翻回背面
    card1->setPosition(record.oldPos1); // 回到备用牌堆位置

    // 将 card2 放回手牌区成为顶部
    tray.push_back(card2);
    card2->setRegion(CardRegionType::CRT_TRAY);
    card2->setPosition(record.oldPos2);

    // 重新布局备用牌堆（包括刚放回的 card1）
    _model->layoutStockCards();
    _gameView->updateStockLayout();

    // 重新创建 card2 的视图（因为它之前被移除了）
    CardView* newView = CardView::create(card2);
    newView->setClickCallback(_gameView->getCardClickCallback());
    _gameView->addChild(newView);
    _gameView->registerCardView(card2->getId(), newView);

    // 移动 card1 回到原位
    CardView* card1View = _gameView->getCardViewById(card1->getId());
    if (card1View) {
        //card1View->moveTo(record.oldPos1, 0.3f, nullptr); //updateStockLayout 中已经处理了所有备用牌。
        // 移动 card1 回到原位并更新显示
        //card1View->updateDisplay(); 
    }
}