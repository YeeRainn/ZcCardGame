/**
 * @file LevelConfigLoader.cpp源文件
 * @brief 实现对JSON文件的解析
 */

#include "LevelConfigLoader.h"
#include "cocos2d.h"
#include "external/json/document.h"  // Cocos2d-x 自带的 rapidjson 库

using namespace cocos2d;
using namespace rapidjson;

LevelConfig LevelConfigLoader::load(const std::string& filePath) {
    LevelConfig config;   // 创建一个空的配置对象

    // 读取文件内容
    auto data = FileUtils::getInstance()->getStringFromFile(filePath);
    if (data.empty()) {
        CCLOG("错误：无法加载配置文件：%s", filePath.c_str());
        return config;   // 返回空配置
    }

    // 解析JSON
    Document doc;
    doc.Parse(data.c_str());
    if (doc.HasParseError()) {
        CCLOG("错误：JSON解析失败");
        return config;
    }

    // 解析 Playfield 数组
    if (doc.HasMember("Playfield") && doc["Playfield"].IsArray()) {
        const auto& arr = doc["Playfield"].GetArray();
        for (SizeType i = 0; i < arr.Size(); ++i) {
            const auto& item = arr[i];
            CardConfig card;

            // 从JSON读取整数，并转换为枚举类型
            card.face = static_cast<CardFaceType>(item["CardFace"].GetInt());
            card.suit = static_cast<CardSuitType>(item["CardSuit"].GetInt());
            card.position.x = item["Position"]["x"].GetFloat();
            card.position.y = item["Position"]["y"].GetFloat();
            config.playfieldCards.push_back(card);
        }
    }

    // 解析 Stack 数组（备用牌堆）
    if (doc.HasMember("Stack") && doc["Stack"].IsArray()) {
        const auto& arr = doc["Stack"].GetArray();
        for (SizeType i = 0; i < arr.Size(); ++i) {
            const auto& item = arr[i];
            CardConfig card;
            card.face = static_cast<CardFaceType>(item["CardFace"].GetInt());
            card.suit = static_cast<CardSuitType>(item["CardSuit"].GetInt());
            // 备用牌堆的位置会在后面动态计算，这里先设为(0,0)
            card.position = Vec2::ZERO;
            config.stockCards.push_back(card);
        }
    }

    return config;
}