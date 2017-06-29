#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "network/WebSocket.h"
#include "client.pb.h"
static int64_t sequence = 0;
class HelloWorld : public cocos2d::Layer, cocos2d::network::WebSocket::Delegate
{
public:
    static cocos2d::Scene* createScene();

    virtual bool init() override;
    
    // a selector callback
    void menuCloseCallback(cocos2d::Ref* pSender);
    
    // implement the "static create()" method manually
    CREATE_FUNC(HelloWorld);
    
    HelloWorld();
    virtual ~HelloWorld();
    
    virtual void onOpen(cocos2d::network::WebSocket* ws) override;
    virtual void onMessage(cocos2d::network::WebSocket* ws, const cocos2d::network::WebSocket::Data& data) override;
    virtual void onClose(cocos2d::network::WebSocket* ws) override;
    virtual void onError(cocos2d::network::WebSocket* ws, const cocos2d::network::WebSocket::ErrorCode& error) override;
    
    cocos2d::network::WebSocket* _webSocket;
    
    proto::AutoID *_test;
    unsigned char _buf[1024];
    
    proto::UserRequest::LobbyLogin *_login;
    unsigned char _loginBuf[1024];
};

#endif // __HELLOWORLD_SCENE_H__
