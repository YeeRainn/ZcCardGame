/**
 * @file TrayController.cpp
 * @brief TrayController的实现文件，定义了TrayController类的成员函数
 */

#include "TrayController.h"
#include "models/UndoRecord.h"
#include <algorithm>

 // 初始化控制器
void TrayController::init(GameModel* model, UndoManager* undoManager, GameView* gameView) {
    _model = model;
    _undoManager = undoManager;
    _gameView = gameView;
}

// 处理手牌区卡牌点击
bool TrayController::onCardClicked(int cardId) {
    // 获取被点击的牌
    CardModel* clickedCard = _model->getCardById(cardId);
    if (!clickedCard || clickedCard->getRegion() != CardRegionType::CRT_TRAY) return false;

    // 获取手牌区顶部牌
    CardModel* topCard = _model->getTopTrayCard();
    if (!topCard) return false;

    // 如果点击的就是顶部牌，无效
    if (clickedCard->getId() == topCard->getId()) return false; 

    // 创建撤销记录
    UndoRecord record;
    record.type = UndoType::TRAY_SWAP;
    record.cardId1 = clickedCard->getId();      // 点击的牌
    record.cardId2 = topCard->getId();          // 被替换的顶部牌
	record.oldPos1 = clickedCard->getPosition();// 点击牌的原位置
	record.oldPos2 = topCard->getPosition();    // 顶部牌的原位置
	record.newPos1 = topCard->getPosition();    // 点击牌的新位置（顶部位置）
    record.newPos2 = clickedCard->getPosition();// 被替换牌的原位置（用于撤销）

    // 更新模型
    //从手牌区移除顶部牌
    auto& tray = _model->getTrayCards();
    auto itTop = std::find(tray.begin(), tray.end(), topCard);
    if (itTop != tray.end()) tray.erase(itTop);

    // 从手牌区移除点击的牌（它原本也在列表中）
    auto itClicked = std::find(tray.begin(), tray.end(), clickedCard);
    if (itClicked != tray.end()) tray.erase(itClicked);

    // 将点击的牌添加到尾部，成为新顶部
    tray.push_back(clickedCard); 
    clickedCard->setRegion(CardRegionType::CRT_TRAY);
    clickedCard->setPosition(topCard->getPosition());   // 更新逻辑位置

    // 将被替换的顶部牌放入移除列表（备用撤销恢复）
    topCard->setRegion(CardRegionType::CRT_NONE);
    _model->getRemovedCards().push_back(topCard);

    // 保存撤销记录
    _undoManager->pushRecord(record);

    // 播放动画（两张牌交换位置）
    _gameView->playTraySwapAnimation(clickedCard->getId(), topCard->getId(), [this, topCard]() {
        // 动画完成后，从视图中移除被替换的牌的视图
        _gameView->removeCardView(topCard->getId());
        });

    return true;
}

void TrayController::undoSwap(const UndoRecord& record) {
    // 获取两张牌：card1是当前在手牌区的牌（之前点击的），card2是被移除的牌
    CardModel* card1 = _model->getCardById(record.cardId1); 
    CardModel* card2 = _model->getCardById(record.cardId2); 
    if (!card1 || !card2) return;

    // 从移除列表中恢复card2
    auto& removed = _model->getRemovedCards();
    auto it = std::find(removed.begin(), removed.end(), card2);
    if (it != removed.end()) removed.erase(it);

    // 从手牌区移除card1
    auto& tray = _model->getTrayCards();
    auto it1 = std::find(tray.begin(), tray.end(), card1);
    if (it1 != tray.end()) tray.erase(it1);

    // 将card2放回手牌区成为新顶部
    tray.push_back(card2);
    card2->setRegion(CardRegionType::CRT_TRAY);
    card2->setPosition(record.oldPos2);     // 回到原来的位置

    // 将card1放回手牌区原来的位置（不是顶部，插入到开头或任何位置，保持位置正确）
    tray.insert(tray.begin(), card1);
    card1->setRegion(CardRegionType::CRT_TRAY);
    card1->setPosition(record.oldPos1);

    // 重新创建card2的视图（因为它之前被移除了）
    CardView* newView = CardView::create(card2);
    newView->setClickCallback(_gameView->getCardClickCallback());
    _gameView->addChild(newView);
    _gameView->registerCardView(card2->getId(), newView);

    // 移动card1回到原位（如果它还在原位置，但可能因交换而移动了，所以强制移动）
    CardView* card1View = _gameView->getCardViewById(card1->getId());
    if (card1View) {
        card1View->moveTo(record.oldPos1, 0.3f, nullptr);
    }
}