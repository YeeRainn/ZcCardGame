/**
 * @file GameModel.cpp
 * @brief GameModel的实现文件，定义了GameModel类的成员函数，以及布局方法。
 */

#include "GameModel.h"
#include <algorithm>

 //cocos2dx头文件
#include "cocos2d.h"
USING_NS_CC;

GameModel::GameModel() {
    reset();
}

// 清空所有牌的容器，准备重新开始游戏
void GameModel::reset() {
    _playfieldCards.clear();
    _trayCards.clear();
    _stockCards.clear();
    _removedCards.clear();
}

// 获取手牌区顶部牌（即列表中的最后一张）
CardModel* GameModel::getTopTrayCard() const {
    if (_trayCards.empty()) return nullptr;
    return _trayCards.back();   // 手牌区以vector尾部为顶部
}

// 通过ID查找牌，遍历所有区域寻找匹配的牌
CardModel* GameModel::getCardById(int id) {
    for (auto* card : _playfieldCards) if (card->getId() == id) return card;
    for (auto* card : _trayCards) if (card->getId() == id) return card;
    for (auto* card : _stockCards) if (card->getId() == id) return card;
    for (auto* card : _removedCards) if (card->getId() == id) return card;
    return nullptr;
}

// 纸牌布局实现
void GameModel::layoutStockCards() {
    // 备用牌堆基准起始位置（手牌区左侧，Y坐标固定）
    const float START_X = 200.0f;
    const float Y = 300.0f;
    const float OFFSET_X = 139.0f;   // 每张牌向右偏移139像素
    const int MAX_VISIBLE = 3;       // 最多展示三张牌

    int count = (int)_stockCards.size();
    for (int i = 0; i < count; ++i) {
        // 确定偏移量索引：超过MAX_VISIBLE的牌与第MAX_VISIBLE张牌重叠
        int offsetIndex = i;
        if (i >= MAX_VISIBLE) {
            offsetIndex = MAX_VISIBLE - 1;  // 索引从0开始，所以第三张索引为2
        }
        float x = START_X + offsetIndex * OFFSET_X;
        _stockCards[i]->setPosition(Vec2(x, Y));
    }
}

void GameModel::layoutPlayfieldCards() {
    if (_playfieldCards.empty()) return;

    // 初始化最小最大值
    float minX = _playfieldCards[0]->getPosition().x;
    float maxX = minX;
    float minY = _playfieldCards[0]->getPosition().y;
    float maxY = minY;

    // 遍历所有桌面牌，找到X和Y的范围
    for (auto* card : _playfieldCards) {
        float x = card->getPosition().x;
        float y = card->getPosition().y;
        if (x < minX) minX = x;
        if (x > maxX) maxX = x;
        if (y < minY) minY = y;
        if (y > maxY) maxY = y;
    }

    // 计算当前中心
    float currentCenterX = (minX + maxX) / 2.0f;
    float currentCenterY = (minY + maxY) / 2.0f;

    // 目标中心
    const float TARGET_CENTER_X = 540.0f;      // 屏幕水平中心
    const float TARGET_CENTER_Y = 1330.0f;     // 主牌区垂直中心 (580 + 1500/2)

    // 计算偏移量
    float offsetX = TARGET_CENTER_X - currentCenterX;
    float offsetY = TARGET_CENTER_Y - currentCenterY;

    // 平移每张牌
    for (auto* card : _playfieldCards) {
        Vec2 pos = card->getPosition();
        pos.x += offsetX;
        pos.y += offsetY;
        card->setPosition(pos);
    }
}