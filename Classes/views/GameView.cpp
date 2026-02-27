/**
 * @file GameView.cpp
 * @brief GameView的实现文件，定义了GameView类的成员函数,实现抽牌动画、备用牌堆布局的动画
 */

#include "GameView.h"

using namespace cocos2d;

// 静态工厂方法，创建GameView
GameView* GameView::create(GameModel* model) {
    GameView* view = new GameView();
    if (view && view->init(model)) {
        view->autorelease();
        return view;
    }
    CC_SAFE_DELETE(view);
    return nullptr;
}

// 初始化
bool GameView::init(GameModel* model) {
    if (!Layer::init()) return false;
    _model = model;

    return true;
}

void GameView::buildViews() {
    this->removeAllChildren();  // 清空原有视图
    _cardViews.clear();         // 清空映射

    // 添加整体背景（设为灰色）
    auto background = LayerColor::create(Color4B(100, 100, 100, 255));
    this->addChild(background, -10);  // 低 ZOrder，让其他内容在上面

    // 添加主牌区背景（橙黄色）,主牌区范围：Y 从 580 到 2080，高度 1500
    auto playfieldBg = LayerColor::create(Color4B(255, 200, 100, 255)); // 橙黄色
    playfieldBg->setContentSize(Size(1080, 1500));
    playfieldBg->setPosition(0, 580);  // 从 Y=580 开始
    this->addChild(playfieldBg, -9);

    // 添加手牌区背景（紫色）,手牌区范围：Y 从 0 到 580，高度 580
    auto trayBg = LayerColor::create(Color4B(150, 100, 200, 255)); // 紫色
    trayBg->setContentSize(Size(1080, 580));
    trayBg->setPosition(0, 0);
    this->addChild(trayBg, -9);

    // 创建主牌区的所有卡牌视图
    for (auto* cardModel : _model->getPlayfieldCards()) {
        auto* cardView = CardView::create(cardModel);
        cardView->setClickCallback(_clickCallback);     // 设置回调
        this->addChild(cardView);                       // 添加到图层
        _cardViews[cardModel->getId()] = cardView;      // 记录映射
    }

    // 创建手牌区的所有卡牌视图
    for (auto* cardModel : _model->getTrayCards()) {
        auto* cardView = CardView::create(cardModel);
        cardView->setClickCallback(_clickCallback);     // 设置回调
        this->addChild(cardView);                       // 添加到图层
        _cardViews[cardModel->getId()] = cardView;      // 记录映射
    }

    // 创建备用牌堆的所有卡牌视图
    for (auto* cardModel : _model->getStockCards()) {
        auto* cardView = CardView::create(cardModel);
        cardView->setClickCallback(_clickCallback);
        this->addChild(cardView);
        _cardViews[cardModel->getId()] = cardView;
    }
}

// 根据ID获取对应的CardView
CardView* GameView::getCardViewById(int cardId) {
    auto it = _cardViews.find(cardId);
    return it != _cardViews.end() ? it->second : nullptr;
}

// 移除指定ID的CardView，从图层和映射中删除
void GameView::removeCardView(int cardId) {
    auto it = _cardViews.find(cardId);
    if (it != _cardViews.end()) {
        it->second->removeFromParent();     // 从父节点移除
        _cardViews.erase(it);               // 从映射中删除
    }
}

// 注册一个CardView到映射，用于撤销时重新添加
void GameView::registerCardView(int cardId, CardView* view) {
    _cardViews[cardId] = view;      // 重新加入映射
}

// 播放手牌区内部交换动画（两张牌互换位置）
void GameView::playTraySwapAnimation(int cardId1, int cardId2, const std::function<void()>& onComplete) {
    CardView* view1 = getCardViewById(cardId1);
    CardView* view2 = getCardViewById(cardId2);
    if (!view1 || !view2) return;

    Vec2 pos1 = view1->getPosition();
    Vec2 pos2 = view2->getPosition();

    // 两张牌同时移动到对方的位置
    view1->moveTo(pos2, 0.3f, nullptr);
    view2->moveTo(pos1, 0.3f, onComplete);  // 第二张牌动画完成后触发回调
}

// 播放桌面牌移入手牌区替换动画
void GameView::playPlayfieldToTrayAnimation(int playfieldCardId, int trayCardId, const std::function<void()>& onComplete) {
    CardView* playfieldView = getCardViewById(playfieldCardId);
    CardView* trayView = getCardViewById(trayCardId);
    if (!playfieldView || !trayView) return;

    Vec2 dest = trayView->getPosition();    // 目标位置为被替换牌的位置

    // 桌面牌移动到目标位置
    playfieldView->moveTo(dest, 0.3f, [this, trayCardId, onComplete]() {
        // 动画完成后移除被替换的牌的视图
        removeCardView(trayCardId);
        if (onComplete) onComplete();
    });
}

void GameView::setCardClickCallback(const std::function<void(CardView*)>& callback) {
    _clickCallback = callback;
    // 更新所有已存在的卡牌视图的回调
    for (auto& pair : _cardViews) {
        pair.second->setClickCallback(callback);
    }
}

// 播放从备用牌堆抽牌到手的动画
void GameView::playStockToTrayAnimation(int stockCardId, int trayCardId, const std::function<void()>& onComplete) {
    CardView* stockView = getCardViewById(stockCardId);
    CardView* trayView = getCardViewById(trayCardId);
    if (!stockView || !trayView) return;

    Vec2 dest = trayView->getPosition(); // 目标位置为原顶部牌位置

    // 抽出的牌移动到目标位置
    stockView->moveTo(dest, 0.3f, [this, trayCardId, onComplete]() {
        // 动画完成后移除被替换的牌的视图
        removeCardView(trayCardId);
        if (onComplete) onComplete();
        });
}

//备用牌堆布局并播放移动动画
void GameView::updateStockLayout() {
    // 遍历所有备用牌堆的牌
    for (auto* cardModel : _model->getStockCards()) {
        CardView* view = getCardViewById(cardModel->getId());
        if (view) {
            // 播放移动动画到模型中的新位置，时长0.3秒
            view->moveTo(cardModel->getPosition(), 0.3f, nullptr);
        }
    }
}

