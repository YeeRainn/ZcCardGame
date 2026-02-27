/**
 * @file GameModelFromLevelGenerator.cpp
 * @brief 实现从配置生成游戏模型
 */

#include "GameModelFromLevelGenerator.h"

#include "cocos2d.h"
using namespace cocos2d;


GameModel* GameModelFromLevelGenerator::generate(const LevelConfig& config) {
    GameModel* model = new GameModel();     // 创建空模型

    // 创建主牌区牌（全部翻开）
    for (const auto& cardCfg : config.playfieldCards) {
        CardModel* card = CardModel::create(0, cardCfg.face, cardCfg.suit, cardCfg.position);
        card->setRegion(CardRegionType::CRT_PLAYFIELD);
        card->setFaceUp(true);      // 假设所有主牌区牌初始翻开
        model->getPlayfieldCards().push_back(card);
    }

    // 设置手牌区各部分的坐标基准
    // 备用牌堆的起始位置，实际布局由 layoutStockCards 完成，默认值
    float stockX = 200.0f;
    float trayY = 300.0f;
    float topTrayX = 640.0f;   // 手牌区顶部牌位置

    // 创建备用牌堆牌
    for (const auto& cardCfg : config.stockCards) {
        // 所有牌同一位置
        CardModel* card = CardModel::create(0, cardCfg.face, cardCfg.suit, Vec2(stockX, trayY));

        card->setRegion(CardRegionType::CRT_STOCK);
        //card->setFaceUp(false);     //备用牌堆牌全部覆盖
        card->setFaceUp(true);     //备用牌堆牌正面朝上
        model->getStockCards().push_back(card);
    }

    // 初始底牌：从备用牌堆取一张作为底牌，并翻开
    if (!model->getStockCards().empty()) {
        CardModel* initialTray = model->getStockCards().front();    // 取第一张
        model->getStockCards().erase(model->getStockCards().begin());// 从备用牌堆移除
        initialTray->setRegion(CardRegionType::CRT_TRAY);
        initialTray->setFaceUp(true);
        // 设置底牌位置
        initialTray->setPosition(Vec2(topTrayX, trayY));// 手牌区位置
        model->getTrayCards().push_back(initialTray);
    }

    // 应用布局（备用牌堆叠放、桌面牌整体居中）
    model->layoutStockCards();       // 备用牌堆按叠放规则排布
    model->layoutPlayfieldCards();   // 桌面牌整体水平居中

    return model;
}