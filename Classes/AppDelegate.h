/**
 * @file AppDelegate.h
 * @brief 应用程序入口，定义AppDelegate类，负责应用生命周期管理和初始化。
 */

#ifndef  _APP_DELEGATE_H_
#define  _APP_DELEGATE_H_

#include "cocos2d.h"

/**
@brief    The cocos2d Application.

这里的私有继承使得一部分接口信息对 Director 而言处于隐藏状态。
*/
class  AppDelegate : private cocos2d::Application
{
public:
    AppDelegate();
    virtual ~AppDelegate();

    virtual void initGLContextAttrs();

    /**
    @brief    在此处实现导演和场景的初始化代码。
    @return true    初始化成功，应用继续运行。
    @return false   初始化失败，应用程序终止。
    */
    virtual bool applicationDidFinishLaunching();

    /**
    @brief  当应用程序移至后台时触发此事件
    @param  该应用程序的指针
    */
    virtual void applicationDidEnterBackground();

    /**
    @brief  当应用程序 即将 重新进入前台时触发此事件
    @param  该应用程序的指针
    */
    virtual void applicationWillEnterForeground();
};

#endif // _APP_DELEGATE_H_

