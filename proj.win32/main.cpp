
#include "main.h"
#include "AppDelegate.h"

//cocos2dx头文件
#include "cocos2d.h"
USING_NS_CC;

int WINAPI _tWinMain(HINSTANCE hInstance,
                       HINSTANCE hPrevInstance,
                       LPTSTR    lpCmdLine,
                       int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // 创建应用程序实例
    AppDelegate app;
    return Application::getInstance()->run();
}
