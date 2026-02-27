/**
 * @file PlayFieldController.cpp
 * @brief 主牌区控制器，PlayFieldController的实现文件，定义了PlayFieldController类的成员函数
 */

#include "PlayFieldController.h"
#include "models/UndoRecord.h"
#include <algorithm>
#include <cmath>

// 初始化控制器
void PlayFieldController::init(GameModel* model, UndoManager* undoManager, GameView* gameView) {
    _model = model;
    _undoManager = undoManager;
    _gameView = gameView;
}

// 处理桌面牌点击事件
bool PlayFieldController::onCardClicked(int cardId) {
    //获取被点击的桌面牌
    CardModel* clickedCard = _model->getCardById(cardId);
    if (!clickedCard || clickedCard->getRegion() != CardRegionType::CRT_PLAYFIELD) return false;

    //获取手牌区顶部牌
    CardModel* topTray = _model->getTopTrayCard();
    if (!topTray) return false;

    //检查点数差是否为1
    int face1 = static_cast<int>(clickedCard->getFace());
    int face2 = static_cast<int>(topTray->getFace());
    if (std::abs(face1 - face2) != 1) return false;

    // 创建撤销记录
    UndoRecord record;
    record.type = UndoType::PLAYFIELD_TO_TRAY;
    record.cardId1 = clickedCard->getId();   // 移动的桌面牌
    record.cardId2 = topTray->getId();       // 被替换的手牌区顶部牌
    record.oldPos1 = clickedCard->getPosition();
    record.oldPos2 = topTray->getPosition();
    record.newPos1 = topTray->getPosition(); // 桌面牌的新位置（原顶部牌位置）
    record.newPos2 = clickedCard->getPosition(); // 被替换牌的原位置（用于撤销）

    // 更新模型
    //从主牌区移除点击的牌
    auto& playfield = _model->getPlayfieldCards();
    auto itPlay = std::find(playfield.begin(), playfield.end(), clickedCard);
    if (itPlay != playfield.end()) playfield.erase(itPlay);

    //从手牌区移除顶部牌
    auto& tray = _model->getTrayCards();
    auto itTray = std::find(tray.begin(), tray.end(), topTray);
    if (itTray != tray.end()) tray.erase(itTray);

    //将点击的牌添加到手牌区成为新顶部
    tray.push_back(clickedCard);
    clickedCard->setRegion(CardRegionType::CRT_TRAY);
    clickedCard->setPosition(topTray->getPosition());

    //将被替换的顶部牌放入移除列表
    topTray->setRegion(CardRegionType::CRT_NONE);
    _model->getRemovedCards().push_back(topTray);

    //保存撤销记录
    _undoManager->pushRecord(record);

    // 播放动画
    _gameView->playPlayfieldToTrayAnimation(clickedCard->getId(), topTray->getId(), [this, topTray]() {
        _gameView->removeCardView(topTray->getId());
        });

    return true;
}

// 撤销桌面牌移入手牌区替换操作
void PlayFieldController::undoPlayfieldToTray(const UndoRecord& record) {
    //获取两张牌
    CardModel* card1 = _model->getCardById(record.cardId1); // 当前在手牌区的桌面牌
    CardModel* card2 = _model->getCardById(record.cardId2); // 被移除的顶部牌
    if (!card1 || !card2) return;

    // 从移除列表恢复 card2
    auto& removed = _model->getRemovedCards();
    auto it = std::find(removed.begin(), removed.end(), card2);
    if (it != removed.end()) removed.erase(it);

    // 从手牌区移除card1
    auto& tray = _model->getTrayCards();
    auto it1 = std::find(tray.begin(), tray.end(), card1);
    if (it1 != tray.end()) tray.erase(it1);

    // 将 card1 放回主牌区
    _model->getPlayfieldCards().push_back(card1);
    card1->setRegion(CardRegionType::CRT_PLAYFIELD);
    card1->setPosition(record.oldPos1);

    // 将card2放回手牌区成为顶部
    tray.push_back(card2);
    card2->setRegion(CardRegionType::CRT_TRAY);
    card2->setPosition(record.oldPos2);

    // 重新创建 card2 的视图
    CardView* newView = CardView::create(card2);
    newView->setClickCallback(_gameView->getCardClickCallback());
    _gameView->addChild(newView);
    _gameView->registerCardView(card2->getId(), newView);

    // 移动 card1 回到原位
    CardView* card1View = _gameView->getCardViewById(card1->getId());
    if (card1View) {
        card1View->moveTo(record.oldPos1, 0.3f, nullptr);
    }
}