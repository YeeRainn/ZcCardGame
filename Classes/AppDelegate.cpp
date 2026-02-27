/**
 * @file AppDelegate.cpp
 * @brief 应用程序入口，设置窗口并启动HelloWorld场景。
 */

#include "AppDelegate.h"
#include "HelloWorldScene.h"

//使用音频引擎
// #define USE_AUDIO_ENGINE 1
// 
// 使用简单音频引擎
// #define USE_SIMPLE_AUDIO_ENGINE 1

#if USE_AUDIO_ENGINE && USE_SIMPLE_AUDIO_ENGINE
#error "Don't use AudioEngine and SimpleAudioEngine at the same time. Please just select one in your game!"
#endif

#if USE_AUDIO_ENGINE
#include "audio/include/AudioEngine.h"
using namespace cocos2d::experimental;
#elif USE_SIMPLE_AUDIO_ENGINE
#include "audio/include/SimpleAudioEngine.h"
using namespace CocosDenshion;
#endif

USING_NS_CC;

//设置静态屏幕分辨率的参数
static cocos2d::Size designResolutionSize = cocos2d::Size(1080, 2080);
static cocos2d::Size smallResolutionSize = cocos2d::Size(480, 320);
static cocos2d::Size mediumResolutionSize = cocos2d::Size(1024, 768);
static cocos2d::Size largeResolutionSize = cocos2d::Size(2048, 1536);

AppDelegate::AppDelegate()
{
}

AppDelegate::~AppDelegate() 
{
#if USE_AUDIO_ENGINE
    AudioEngine::end();
#elif USE_SIMPLE_AUDIO_ENGINE
    SimpleAudioEngine::end();
#endif
}

// 如果您想要不同的环境，请修改 glContextAttrs 的值。
// 这将影响到所有的平台。
void AppDelegate::initGLContextAttrs()
{
    //设置 OpenGL 上下文属性：红色、绿色、蓝色、透明度、深度、存档、多重采样次数
    GLContextAttrs glContextAttrs = {8, 8, 8, 8, 24, 8, 0};

    GLView::setGLContextAttrs(glContextAttrs);
}

// 如果您想使用包管理器来安装更多软件包，请不要修改或删除此功能。
static int register_all_packages()
{
    return 0; //用于包管理器的标志
}

//设置游戏运行窗口的大小，创建场景
bool AppDelegate::applicationDidFinishLaunching() {
    // initialize director 初始化
    auto director = Director::getInstance();
    auto glview = director->getOpenGLView();
    if(!glview) {
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32) || (CC_TARGET_PLATFORM == CC_PLATFORM_MAC) || (CC_TARGET_PLATFORM == CC_PLATFORM_LINUX)
		// 在桌面平台上，创建一个固定大小的窗口
        glview = GLViewImpl::createWithRect("ZcCardGame", cocos2d::Rect(0, 0, designResolutionSize.width, designResolutionSize.height),0.5f);
#else
        glview = GLViewImpl::create("ZcCardGame");
#endif
        director->setOpenGLView(glview);
    }

    // turn on display FPS 开关FPS
    director->setDisplayStats(false);

    // set FPS. the default value is 1.0/60 if you don't call this
    director->setAnimationInterval(1.0f / 60);

    // 设置设计分辨率 1080x2080，采用 FIXED_WIDTH 策略
    glview->setDesignResolutionSize(designResolutionSize.width, designResolutionSize.height, ResolutionPolicy::FIXED_WIDTH);

#if 0
    auto frameSize = glview->getFrameSize();
    // if the frame's height is larger than the height of medium size.
    if (frameSize.height > mediumResolutionSize.height)
    {        
        director->setContentScaleFactor(MIN(largeResolutionSize.height/designResolutionSize.height, largeResolutionSize.width/designResolutionSize.width));
    }
    // if the frame's height is larger than the height of small size.
    else if (frameSize.height > smallResolutionSize.height)
    {        
        director->setContentScaleFactor(MIN(mediumResolutionSize.height/designResolutionSize.height, mediumResolutionSize.width/designResolutionSize.width));
    }
    // if the frame's height is smaller than the height of medium size.
    else
    {        
        director->setContentScaleFactor(MIN(smallResolutionSize.height/designResolutionSize.height, smallResolutionSize.width/designResolutionSize.width));
    }
#endif

    register_all_packages();

    // 创建一个场景。这是一个自动释放的对象。
    auto scene = HelloWorld::createScene();

    // run
    director->runWithScene(scene);

    return true;
}

// 当应用程序处于非活动状态时，此功能将会被调用。需要注意的是，当接收到电话时，该功能会被触发。
void AppDelegate::applicationDidEnterBackground() {
    Director::getInstance()->stopAnimation();

#if USE_AUDIO_ENGINE
    AudioEngine::pauseAll();
#elif USE_SIMPLE_AUDIO_ENGINE
    SimpleAudioEngine::getInstance()->pauseBackgroundMusic();
    SimpleAudioEngine::getInstance()->pauseAllEffects();
#endif
}

// 当应用程序再次处于激活状态时，此功能将会被调用。
void AppDelegate::applicationWillEnterForeground() {
    Director::getInstance()->startAnimation();

#if USE_AUDIO_ENGINE
    AudioEngine::resumeAll();
#elif USE_SIMPLE_AUDIO_ENGINE
    SimpleAudioEngine::getInstance()->resumeBackgroundMusic();
    SimpleAudioEngine::getInstance()->resumeAllEffects();
#endif
}
