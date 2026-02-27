/**
 * @file GameView.h
 * @brief 游戏主视图，继承自Layer，包含所有CardView，并提供动画接口，以及备用牌堆布局的方法
 */

#ifndef __GAME_VIEW_H__
#define __GAME_VIEW_H__

#include "cocos2d.h"
#include "models/GameModel.h"
#include "views/CardView.h"
#include <map>
#include <functional>

class GameView : public cocos2d::Layer {
public:

    /**
     * @brief 创建GameView
     * @param model 游戏数据模型
     */
    static GameView* create(GameModel* model);

    /**
     * @brief 根据模型中的牌数据，创建所有CardView并添加到图层
     */
    void buildViews();

    /**
     * @brief 根据ID获取对应的CardView
     */
    CardView* getCardViewById(int cardId);

    /**
     * @brief 移除指定ID的CardView，从图层和映射中删除
     */
    void removeCardView(int cardId);

    /**
     * @brief 注册一个CardView到映射，用于撤销时重新添加
     */
    void registerCardView(int cardId, CardView* view);

    /**
     * @brief 播放手牌区内部交换动画（两张牌互换位置）
     * @param cardId1 第一张牌ID
     * @param cardId2 第二张牌ID
     * @param onComplete 动画完成回调
     */
    void playTraySwapAnimation(int cardId1, int cardId2, const std::function<void()>& onComplete);

    /**
     * @brief 播放桌面牌移入手牌区替换动画
     * @param playfieldCardId 桌面牌ID
     * @param trayCardId 被替换的手牌区顶部牌ID
     * @param onComplete 动画完成回调
     */
    void playPlayfieldToTrayAnimation(int playfieldCardId, int trayCardId, const std::function<void()>& onComplete);

    /**
     * @brief 播放从备用牌堆抽牌到手的动画
     * @param stockCardId 被抽出的备用牌ID
     * @param trayCardId 被替换的手牌区顶部牌ID
     * @param onComplete 动画完成回调
     */
    void playStockToTrayAnimation(int stockCardId, int trayCardId, const std::function<void()>& onComplete);

    //更新牌堆布局
    void updateStockLayout();

    /**
     * @brief 设置所有卡牌的点击回调
     */
    void setCardClickCallback(const std::function<void(CardView*)>& callback);

    /**
     * @brief 获取当前的点击回调（供撤销时重新设置）
     */
    std::function<void(CardView*)> getCardClickCallback() const { 
        return _clickCallback; 
    }

private:
    // 私有构造函数，使用create
    GameView() {
    }
    // 初始化
    bool init(GameModel* model);

    GameModel* _model;  // 关联的模型
    std::map<int, CardView*> _cardViews;    // ID到视图的映射
    std::function<void(CardView*)> _clickCallback;  // 统一的点击回调
};

#endif // __GAME_VIEW_H__