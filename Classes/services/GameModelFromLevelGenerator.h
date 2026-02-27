/**
 * @file GameModelFromLevelGenerator.h
 * @brief 服务类，负责根据关卡配置生成运行时游戏数据模型。
 */

#ifndef __GAME_MODEL_FROM_LEVEL_GENERATOR_H__
#define __GAME_MODEL_FROM_LEVEL_GENERATOR_H__

#include "models/GameModel.h"
#include "configs/models/LevelConfig.h"

class GameModelFromLevelGenerator {
public:
    /**
     * @brief 根据配置生成GameModel
     * @param config 已加载的关卡配置
     * @return 新创建的GameModel对象（需要外部释放）
     */
    static GameModel* generate(const LevelConfig& config);
};

#endif // __GAME_MODEL_FROM_LEVEL_GENERATOR_H__