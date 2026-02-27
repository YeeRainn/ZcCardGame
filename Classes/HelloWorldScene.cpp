/**
 * @file HelloWorldScene.cpp
 * @brief HelloWorld场景的实现文件，定义了HelloWorld类的成员函数
 */

#include "HelloWorldScene.h"
#include "SimpleAudioEngine.h"

USING_NS_CC;

Scene* HelloWorld::createScene()
{
    //return HelloWorld::create();    // 通过create函数创建场景
    auto scene = Scene::create();
    auto layer = HelloWorld::create();
    scene->addChild(layer);
    return scene;
}

// 当文件不存在时，应打印有用的错误信息，而不能出现程序崩溃的情况。
static void problemLoading(const char* filename)
{
    printf("Error while loading: %s\n", filename);
    printf("Depending on how you compiled you might have to add 'Resources/' in front of filenames in HelloWorldScene.cpp\n");
}

// 在“初始化”阶段，您需要对您的实例进行初始化设置。
#if 0
bool HelloWorld::init()
{
    //////////////////////////////
    // 1. 超级初始化优先
    if ( !Scene::init() )
    {
        return false;
    }

    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    /////////////////////////////
    // 2. 添加一个带有“X”图像的菜单项，点击该图像即可退出程序。您可以对其进行修改。

    // 添加一个“关闭”图标以用于退出进度界面。这是一个自动释放的对象。
    auto closeItem = MenuItemImage::create(
                                           "CloseNormal.png",
                                           "CloseSelected.png",
                                           CC_CALLBACK_1(HelloWorld::menuCloseCallback, this));

    if (closeItem == nullptr ||
        closeItem->getContentSize().width <= 0 ||
        closeItem->getContentSize().height <= 0)
    {
        problemLoading("'CloseNormal.png' and 'CloseSelected.png'");
    }
    else
    {
        float x = origin.x + visibleSize.width - closeItem->getContentSize().width/2;
        float y = origin.y + closeItem->getContentSize().height/2;
        closeItem->setPosition(Vec2(x,y));
    }

    // 创建菜单，它是一个自动释放的对象
    auto menu = Menu::create(closeItem, NULL);
    menu->setPosition(Vec2::ZERO);
    this->addChild(menu, 1);

    /////////////////////////////
    // 3. 请在下方添加您的代码……




    // 添加一个标签，其显示内容为“你好，世界”
    // 创建并初始化一个标签
    auto label = Label::createWithTTF("Hello World", "fonts/Marker Felt.ttf", 24);
    if (label == nullptr)
    {
        problemLoading("'fonts/Marker Felt.ttf'");
    }
    else
    {
        // 将标签放置在屏幕的中心位置
        label->setPosition(Vec2(origin.x + visibleSize.width/2,
                                origin.y + visibleSize.height - label->getContentSize().height));

        // 将标签作为子项添加到此层中
        this->addChild(label, 1);
    }

    // 添加“HelloWorld”启动画面
    auto sprite = Sprite::create("HelloWorld.png");
    if (sprite == nullptr)
    {
        problemLoading("'HelloWorld.png'");
    }
    else
    {
        // 将精灵置于屏幕的中心位置
        sprite->setPosition(Vec2(visibleSize.width/2 + origin.x, visibleSize.height/2 + origin.y));

        // 将该精灵对象作为子对象添加到此层中
        this->addChild(sprite, 0);
    }
    return true;
}
#endif

//创建GameController游戏控制器，启动游戏，获取游戏视图并添加到当前层，添加回退按钮
bool HelloWorld::init()
{
    if (!Layer::init())
    {
        return false;
    }

    // 创建 GameController 对象
    _gameController = new GameController();

    // 启动游戏,传入关卡ID，暂时固定为1
    _gameController->startGame(1);

    // 获取游戏主视图并添加到当前层
    auto gameView = _gameController->getGameView();
    if (gameView) {
        this->addChild(gameView);
    }

    // 创建回退按钮
    // 创建一个 Label 用于按钮文字
    auto label = Label::createWithSystemFont(u8"回退", "fonts/arial.ttf", 48);
    label->setColor(Color3B::WHITE);
    // 创建 MenuItemLabel，点击时调用控制器的撤销方法
    auto undoItem = MenuItemLabel::create(label, [this](Ref* sender) {
        _gameController->onUndoButtonClicked();
        });
    // 设置按钮位置：手牌区右侧，Y 与手牌区一致 (300)
    undoItem->setPosition(900, 300);    

    auto menu = Menu::create(undoItem, nullptr);
    menu->setPosition(Vec2::ZERO);
    this->addChild(menu);

    return true;
}

void HelloWorld::menuCloseCallback(Ref* pSender)
{
    //关闭 cocos2d-x 游戏场景并退出应用程序
    Director::getInstance()->end();

    #if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif

    //若要无需退出应用程序而直接返回到原生 iOS 屏幕（如果存在的话），
    //请勿使用上述给出的 Director::getInstance()->end() 和 exit(0) 方法，
    // 而是触发在 RootViewController.mm 中创建的自定义事件如下所示

    //EventCustom customEndEvent("game_scene_close_event");
    //_eventDispatcher->dispatchEvent(&customEndEvent);


}

//释放 _gameController 内存
HelloWorld::~HelloWorld()
{
    CC_SAFE_DELETE(_gameController);
}
