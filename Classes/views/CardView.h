/**
 * @file CardView.h
 * @brief 卡牌的视图类，负责显示一张牌（背景、大数字、小数字、花色），并处理点击事件。
 *       继承自Node，因为它需要组合多个Sprite。
 */

#ifndef __CARD_VIEW_H__
#define __CARD_VIEW_H__

#include "cocos2d.h"
#include "models/CardModel.h"
#include <functional>

class CardView : public cocos2d::Node {
public:
    /**
     * @brief 静态工厂方法，创建并初始化一个CardView
     * @param model 对应的数据模型
     * @return 自动释放的CardView指针
     */
    static CardView* create(CardModel* model);

    /**
     * @brief 设置点击回调函数
     * @param callback 当卡片被点击时调用的函数，参数为当前CardView指针
     */
    void setClickCallback(const std::function<void(CardView*)>& callback);

    /**
     * @brief 根据模型的状态更新显示（例如翻牌时调用）
     */
    void updateDisplay();  
    CardModel* getModel() const { 
        return _model; 
    }

    /**
     * @brief 播放移动到目标位置的动画
     * @param dest 目标位置（世界坐标）
     * @param duration 动画时长（秒）
     * @param onComplete 动画完成后的回调
     */
    void moveTo(const cocos2d::Vec2& dest, float duration, const std::function<void()>& onComplete = nullptr);

private:
    bool init(CardModel* model);    // 初始化方法
    void createFaceSprites();       // 创建正面部件（数字、花色）
    void ensureFaceSprites();       // 确保正面部件已存在（若不存在则创建）

    CardModel* _model;      // 关联的数据模型
    std::function<void(CardView*)> _clickCallback;  // 点击回调

    // 组成卡牌的子Sprite
    cocos2d::Sprite* _bgSprite;     // 背景图
    cocos2d::Sprite* _bigNumberSprite;      // 中间大数字
    cocos2d::Sprite* _smallNumberSprite;    // 左上角小数字
    cocos2d::Sprite* _suitSprite;   // 右上角花色
};

#endif // __CARD_VIEW_H__