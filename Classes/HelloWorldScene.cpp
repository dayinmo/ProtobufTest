#include "HelloWorldScene.h"
#include "SimpleAudioEngine.h"


USING_NS_CC;
using namespace cocos2d::network;

Scene* HelloWorld::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = HelloWorld::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

HelloWorld::HelloWorld()
{
    
}

// on "init" you need to initialize your instance
bool HelloWorld::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Layer::init() )
    {
        return false;
    }
    
    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    /////////////////////////////
    // 2. add a menu item with "X" image, which is clicked to quit the program
    //    you may modify it.

    // add a "close" icon to exit the progress. it's an autorelease object
    auto closeItem = MenuItemImage::create(
                                           "CloseNormal.png",
                                           "CloseSelected.png",
                                           CC_CALLBACK_1(HelloWorld::menuCloseCallback, this));
    
    closeItem->setPosition(Vec2(origin.x + visibleSize.width - closeItem->getContentSize().width/2 ,
                                origin.y + closeItem->getContentSize().height/2));

    // create menu, it's an autorelease object
    auto menu = Menu::create(closeItem, NULL);
    menu->setPosition(Vec2::ZERO);
    this->addChild(menu, 1);

    /////////////////////////////
    // 3. add your codes below...

    // add a label shows "Hello World"
    // create and initialize a label
    
    auto label = Label::createWithTTF("Hello World", "fonts/Marker Felt.ttf", 24);
    
    // position the label on the center of the screen
    label->setPosition(Vec2(origin.x + visibleSize.width/2,
                            origin.y + visibleSize.height - label->getContentSize().height));

    // add the label as a child to this layer
    this->addChild(label, 1);

    // add "HelloWorld" splash screen"
    auto sprite = Sprite::create("HelloWorld.png");

    // position the sprite on the center of the screen
    sprite->setPosition(Vec2(visibleSize.width/2 + origin.x, visibleSize.height/2 + origin.y));

    // add the sprite as a child to this layer
    this->addChild(sprite, 0);
    
    
    
    
    
    
    _test = new proto::AutoID();
    int id = 2017;
    _test->set_id(id);
    printf("name %lld \n", _test->id());
    
    int32_t len = _test->ByteSize();
    uint32_t length = len + 6;
    printf("length %d\n", length);
    
    unsigned char buf[4];
    
    buf[0]  = length >> 24;
    
    buf[1] = length >> 16;
    
    buf[2] = length >> 8;
    
    buf[3] = length;
    
    for(int i = 0; i < 4; i++)
    {
        _buf[i] = buf[i];
    }
    
    _login = new proto::UserRequest::LobbyLogin();
    _login->set_name("test1");
    _login->set_passwd("aaa111");
    _login->set_way("-1");
    _login->set_ip("");
    _login->set_platform(1);
    int loginLength = _login->ByteSize();
    length = loginLength  + 6;
    
    buf[0]  = length >> 24;
    
    buf[1] = length >> 16;
    
    buf[2] = length >> 8;
    
    buf[3] = length;
    
    for(int i = 0; i < 4; i++)
    {
        _loginBuf[i] = buf[i];
    }
    
    uint32_t sequence1 = 1;
    unsigned char buf2[4];
    buf2[0]  = sequence1 >> 24;
    
    buf2[1] = sequence1 >> 16;
    
    buf2[2] = sequence1 >> 8;
    
    buf2[3] = sequence1;
    for(int i = 0; i < 4; i++)
    {
        _loginBuf[i+4] = buf2[i];
        _buf[i+4] = buf2[i];
    }
    
    
    uint16_t command;
    CCLOG("command %d", command);
    command = proto::Command::client_heart_beat;
    unsigned char bufShort[2];
    CCLOG("command %d %d", command, command >> 8);
    bufShort[0] = command >> 8;
    bufShort[1] = command;
    for(int i = 0; i < 2; i++)
    {
        _buf[i + 8] = bufShort[i];
        CCLOG("bufv %d test!", bufShort[i]);
    }
    
    command = proto::Command::user_lobby_login;
    bufShort[0] = command >> 8;
    bufShort[1] = command;
    for(int i = 0; i < 2; i++)
    {
        _loginBuf[i + 8] = bufShort[i];
        CCLOG("bufvvv %d test!", bufShort[i]);
    }
    
    char *bufLogin = new char[loginLength];
    _login->SerializeToArray(bufLogin, loginLength);
    for(int i = 0; i < loginLength; i++)
    {
        _loginBuf[i + 10] = bufLogin[i];
    }
    
    char *bufAutoID = new char[len];
    _test->SerializeToArray(bufAutoID, len);
    
    for(int i = 0; i < len; i++)
    {
        _buf[i + 10] = bufAutoID[i];
    }
    
    
    _webSocket = new WebSocket();
    if(!_webSocket->init(*this, "ws://192.168.11.111:5188"))
    {
        CCLOG("_webSocket init!!!");
    }
    printf("byte size = %d\n", len);
    
    
    return true;
}


void HelloWorld::menuCloseCallback(Ref* pSender)
{
    //Close the cocos2d-x game scene and quit the application
    Director::getInstance()->end();

    #if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif
    
    /*To navigate back to native iOS screen(if present) without quitting the application  ,do not use Director::getInstance()->end() and exit(0) as given above,instead trigger a custom event created in RootViewController.mm as below*/
    
    //EventCustom customEndEvent("game_scene_close_event");
    //_eventDispatcher->dispatchEvent(&customEndEvent);
    
    
}

HelloWorld::~HelloWorld()
{
    CCLOG("~HelloWorld!");
}

void HelloWorld::onOpen(cocos2d::network::WebSocket *ws)
{
    CCLOG("HelloWorld onOpen!");
//    CCLOG("_test1 %s", _buf);
//    CCLOG("_webSocket %p", _webSocket);
//    CCLOG("_test2 %d", _test->ByteSize());
//    _webSocket->send(_buf, _test->ByteSize()+10);
    _webSocket->send(_loginBuf, _login->ByteSize()+10);
}

void HelloWorld::onMessage(cocos2d::network::WebSocket *ws, const cocos2d::network::WebSocket::Data &data)
{
    CCLOG("HelloWorld onMessage!");
    
//    proto::AutoID test2;
//    char *test = new char[data.len];
//    CCLOG("data.len - 10 = %d", data.len - 10);
//    for(int i = 0; i < data.len; i++)
//    {
//        CCLOG("data.byte %d", data.bytes[i]);
//    }
//    
//    for(int i = 10, j = 0; i < data.len; i++, j++)
//    {
//        test[j] = data.bytes[i];
//        CCLOG("data.bytes %d", data.bytes[i]);
//    }
//    test2.ParseFromArray(test, data.len - 10);
//    CCLOG("test!!!!! id = %lld", test2.id());
    proto::CommonReply reply;
    char *replyBuf = new char[data.len - 10];
    for(int i = 10, j = 0; i < data.len; i++, j++)
    {
        replyBuf[j] = data.bytes[i];
    }
    reply.ParseFromArray(replyBuf, data.len - 10);
    CCLOG("reply %d, %s!!", reply.code(), reply.desc().c_str());
}

void HelloWorld::onClose(cocos2d::network::WebSocket *ws)
{
    CCLOG("HelloWorld onClose!");
}

void HelloWorld::onError(cocos2d::network::WebSocket *ws, const cocos2d::network::WebSocket::ErrorCode &error)
{
    CCLOG("HelloWorld onError!");
}
