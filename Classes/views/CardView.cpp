/**
 * @file CardView.cpp
 * @brief CardView的实现，包含卡牌组合与动画逻辑。
 */

#include "CardView.h"
#include "base/CCDirector.h"
#include "2d/CCActionInterval.h"
#include "2d/CCActionInstant.h"

using namespace cocos2d;

// 静态工厂方法，创建并初始化一个CardView
CardView* CardView::create(CardModel* model) {
    CardView* view = new CardView();
    if (view && view->init(model)) {
        view->autorelease();     // 加入自动内存管理
        return view;
    }
    CC_SAFE_DELETE(view);
    return nullptr;
}

// 设置点击回调函数
bool CardView::init(CardModel* model) {
    _model = model;

    // 创建背景图片
    _bgSprite = Sprite::create("res/card_general.png");
    if (!_bgSprite) {
        CCLOG("错误：无法加载背景图片: res/card_general.png");
        return false;
    }
    this->addChild(_bgSprite);  // 将背景添加到CardView节点

    // 初始化部件指针为空
    _bigNumberSprite = nullptr;
    _smallNumberSprite = nullptr;
    _suitSprite = nullptr;

    //设置整个视图的位置
    this->setPosition(_model->getPosition());

    // 根据当前牌面状态（正面/背面）更新显示
    updateDisplay();

    // 添加触摸监听
    auto listener = EventListenerTouchOneByOne::create();
    listener->setSwallowTouches(true);  // 吞噬触摸，防止穿透
    listener->onTouchBegan = [this](Touch* touch, Event* event) -> bool {
        // 将触摸点转换到CardView的本地坐标系
        Vec2 localPos = this->convertToNodeSpace(touch->getLocation());
        // 判断是否点击在背景图上
        if (_bgSprite->getBoundingBox().containsPoint(localPos)) {
            if (_clickCallback) _clickCallback(this);   // 触发回调
            return true;
        }
        return false;
    };

    // 将监听器添加到事件分发器，优先级为场景图优先级
    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);

    return true;
}

void CardView::ensureFaceSprites() {

    // 如果大数字还没有创建，说明当前是背面，现在需要显示正面，则创建所有正面部件
    if (!_bigNumberSprite) {
        createFaceSprites();
    }
}

void CardView::createFaceSprites() {
    // 根据花色确定颜色字符串，用于加载对应颜色的数字图片
    std::string color;
    CardSuitType suit = _model->getSuit();
    if (suit == CardSuitType::CST_CLUBS || suit == CardSuitType::CST_SPADES) {
        color = "black";
    }
    else {
        color = "red";
    }

    // 根据牌面确定点数字符串
    std::string faceStr;
    switch (_model->getFace()) {
    case CardFaceType::CFT_ACE:   faceStr = "A"; break;
    case CardFaceType::CFT_TWO:   faceStr = "2"; break;
    case CardFaceType::CFT_THREE: faceStr = "3"; break;
    case CardFaceType::CFT_FOUR:  faceStr = "4"; break;
    case CardFaceType::CFT_FIVE:  faceStr = "5"; break;
    case CardFaceType::CFT_SIX:   faceStr = "6"; break;
    case CardFaceType::CFT_SEVEN: faceStr = "7"; break;
    case CardFaceType::CFT_EIGHT: faceStr = "8"; break;
    case CardFaceType::CFT_NINE:  faceStr = "9"; break;
    case CardFaceType::CFT_TEN:   faceStr = "10"; break;
    case CardFaceType::CFT_JACK:  faceStr = "J"; break;
    case CardFaceType::CFT_QUEEN: faceStr = "Q"; break;
    case CardFaceType::CFT_KING:  faceStr = "K"; break;
    default: faceStr = "back"; break;
    }

    Size bgSize = _bgSprite->getContentSize();  // 获取背景图尺寸，182x282像素

    // 创建大数字（位于中间） 
    std::string bigFile = "res/number/big_" + color + "_" + faceStr + ".png";
    _bigNumberSprite = Sprite::create(bigFile);
    if (_bigNumberSprite) {

        // 大数字锚点默认为中心，设置在背景中央
        _bigNumberSprite->setPosition(bgSize.width / 2, bgSize.height / 2);

        // 作为背景的子节点，跟随背景移动
        _bgSprite->addChild(_bigNumberSprite);
    }
    else {
        CCLOG("错误：无法加载大数字图片: %s", bigFile.c_str());
    }

    // 创建小数字（位于左上角，锚点左上角）
    std::string smallFile = "res/number/small_" + color + "_" + faceStr + ".png";
    _smallNumberSprite = Sprite::create(smallFile);
    if (_smallNumberSprite) {
        // 小数字锚点设为左上角(0,1)，位置为(左边距, 背景高-上边距)
        _smallNumberSprite->setAnchorPoint(Vec2(0, 1));
        _smallNumberSprite->setPosition(10, bgSize.height - 10);// 边距设为10像素
        _bgSprite->addChild(_smallNumberSprite);
    }
    else {
        CCLOG("错误：无法加载小数字图片: %s", smallFile.c_str());
    }

    // 创建花色（位于右上角，锚点右上角）
    std::string suitFile;
    switch (suit) {
    case CardSuitType::CST_CLUBS:    suitFile = "res/suits/club.png"; break;
    case CardSuitType::CST_DIAMONDS: suitFile = "res/suits/diamond.png"; break;
    case CardSuitType::CST_HEARTS:   suitFile = "res/suits/heart.png"; break;
    case CardSuitType::CST_SPADES:   suitFile = "res/suits/spade.png"; break;
    default: break;
    }
    _suitSprite = Sprite::create(suitFile);
    if (_suitSprite) {
        // 花色锚点设为右上角(1,1)，位置为(背景宽-右边距, 背景高-上边距)
        _suitSprite->setAnchorPoint(Vec2(1, 1));
        _suitSprite->setPosition(bgSize.width - 10, bgSize.height - 10);
        _bgSprite->addChild(_suitSprite);
    }
    else {
        CCLOG("错误：无法加载花色图片: %s", suitFile.c_str());
    }
}

void CardView::updateDisplay() {
    if (_model->isFaceUp()) {
        // 正面朝上：确保正面部件存在并显示
        ensureFaceSprites();
        if (_bigNumberSprite) _bigNumberSprite->setVisible(true);
        if (_smallNumberSprite) _smallNumberSprite->setVisible(true);
        if (_suitSprite) _suitSprite->setVisible(true);
    }
    else {
        // 背面朝上：隐藏所有正面部件（如果有）
        if (_bigNumberSprite) {
            _bigNumberSprite->setVisible(false);
            _smallNumberSprite->setVisible(false);
            _suitSprite->setVisible(false);
        }
    }
}

// 设置点击回调函数
void CardView::setClickCallback(const std::function<void(CardView*)>& callback) {
    _clickCallback = callback;
}

void CardView::moveTo(const Vec2& dest, float duration, const std::function<void()>& onComplete) {
    // 创建移动动作
    auto move = MoveTo::create(duration, dest);
    if (onComplete) {
        // 如果需要完成回调，则创建序列动作：移动 + 回调
        auto seq = Sequence::create(move, CallFunc::create(onComplete), nullptr);
        this->runAction(seq);
    }
    else {
        this->runAction(move);
    }
    // 同时更新模型中的逻辑位置，保持模型与视图同步
    _model->setPosition(dest);
}