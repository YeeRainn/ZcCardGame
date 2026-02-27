/**
 * @file CardModel.h头文件
 * @brief 卡牌的数据模型，存储每张牌的属性（ID、花色、牌面、位置、区域、是否翻开）
 */
#ifndef __CARD_MODEL_H__
#define __CARD_MODEL_H__

#include "cocos2d.h"
#include "utils/GameEnum.h"

class CardModel {
public:
    CardModel();   // 构造函数，自动分配唯一ID

    /**
     * @brief 静态工厂方法，创建一张新牌
     * @param id 可指定ID，通常传0让系统自动生成
     * @param face 牌面
     * @param suit 花色
     * @param pos 初始位置
     */
    static CardModel* create(int id, CardFaceType face, CardSuitType suit, const cocos2d::Vec2& pos);

    // 以下为各种属性的getter/setter
    int getId() const { 
        return _id; 
    }
    CardFaceType getFace() const {
        return _face; 
    }
    CardSuitType getSuit() const {
        return _suit; 
    }
    cocos2d::Vec2 getPosition() const { 
        return _position; 
    }
    void setPosition(const cocos2d::Vec2& pos) { 
        _position = pos; 
    }

    CardRegionType getRegion() const {
        return _region; 
    }

    void setRegion(CardRegionType region) {
        _region = region; 
    }

    bool isFaceUp() const { 
        return _faceUp; 
    }
    void setFaceUp(bool up) { 
        _faceUp = up; 
    }

private:
    int _id;                // 唯一标识符
    CardFaceType _face;         // 牌面点数
    CardSuitType _suit;         // 花色
    cocos2d::Vec2 _position;// 当前在屏幕上的逻辑位置
    CardRegionType _region;     // 当前所在区域
    bool _faceUp;           // 是否正面朝上
};

#endif // __CARD_MODEL_H__