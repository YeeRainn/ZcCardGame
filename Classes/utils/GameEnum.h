/**
 * @file GameEnum的头文件，用于枚举
 * @brief 定义游戏中使用的枚举类型（花色、牌面、区域）
 */

#ifndef __GAME_ENUM_H__
#define __GAME_ENUM_H__

// 花色类型
enum class CardSuitType {
    CST_NONE = -1,  // 无花色（错误状态）
    CST_CLUBS,      // 梅花
    CST_DIAMONDS,   // 方块
    CST_HEARTS,     // 红桃
    CST_SPADES,     // 黑桃
    CST_NUM_CARD_SUIT_TYPES   // 花色总数（用于循环）
};

// 牌面类型
enum class CardFaceType {
    CFT_NONE = -1,  // 无效牌面
    CFT_ACE,        // A(点数1)
    CFT_TWO,        // 2
    CFT_THREE,      // 3
    CFT_FOUR,       // 4
    CFT_FIVE,       // 5
    CFT_SIX,        // 6
    CFT_SEVEN,      // 7
    CFT_EIGHT,      // 8
    CFT_NINE,       // 9
    CFT_TEN,        // 10
    CFT_JACK,       // J(点数11)
    CFT_QUEEN,      // Q(点数12)
    CFT_KING,       // K(点数13)
    CFT_NUM_CARD_FACE_TYPES   // 牌面总数
};

// 牌所在的区域
enum class CardRegionType {
    CRT_NONE,       // 未归属（如被移除的牌）
    CRT_PLAYFIELD,  // 主牌区
    CRT_TRAY,       // 底牌堆（手牌区）
    CRT_STOCK       // 备用牌堆
};

#endif // __GAME_ENUM_H__