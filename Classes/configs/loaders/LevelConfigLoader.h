/**
 * @file LevelConfigLoader.h的头文件
 * @brief 配置加载器，负责从JSON文件读取关卡配置
 */

#ifndef __LEVEL_CONFIG_LOADER_H__
#define __LEVEL_CONFIG_LOADER_H__

#include "configs/models/LevelConfig.h"
#include <string>

class LevelConfigLoader {
public:
    /**
     * @brief 从指定路径的JSON文件加载关卡配置
     * @param filePath JSON文件的路径（相对于Resources目录）
     * @return 解析后的LevelConfig结构
     */
    static LevelConfig load(const std::string& filePath);
};

#endif // __LEVEL_CONFIG_LOADER_H__