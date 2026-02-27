/**
 * @file CardModel.cpp
 * @brief CardModel的实现文件，定义了CardModel类的成员函数
 */

#include "CardModel.h"

static int s_nextId = 0;    // 静态变量，用于自动分配唯一ID

CardModel::CardModel() : 
    _id(++s_nextId),    // 每创建一个新对象，ID递增
    _face(CardFaceType::CFT_NONE),
    _suit(CardSuitType::CST_NONE),
    _position(cocos2d::Vec2::ZERO), 
    _region(CardRegionType::CRT_NONE),
    _faceUp(false) {

}

// 静态工厂方法，创建一张新牌
CardModel* CardModel::create(int id, CardFaceType face, CardSuitType suit, const cocos2d::Vec2& pos) {
    CardModel* model = new CardModel();
    if (model) {

        // 如果传入了有效的ID（>0），则覆盖自动生成的ID
        if (id > 0) model->_id = id;
        //model->_id = id;

        model->_face = face;
        model->_suit = suit;
        model->_position = pos;
        model->_region = CardRegionType::CRT_NONE;
        model->_faceUp = false;
    }
    return model;
}