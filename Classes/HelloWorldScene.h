/**
 * @file HelloWorldScene.h
 * @brief 游戏的入口场景，负责创建GameController并显示游戏视图。
 */

#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "cocos2d.h"

#include "controllers/GameController.h"

//class HelloWorld : public cocos2d::Scene
class HelloWorld : public cocos2d::Layer
{
public:
    //创建场景
    static cocos2d::Scene* createScene();

    //初始化
    virtual bool init();
    
    // 一个选择器回调函数
    void menuCloseCallback(cocos2d::Ref* pSender);
    
    // 手动执行“静态创建()”方法
    CREATE_FUNC(HelloWorld);

    // 声明析构函数
    ~HelloWorld();

private:
    // 游戏主控制器
    GameController* _gameController;
};

#endif // __HELLOWORLD_SCENE_H__
