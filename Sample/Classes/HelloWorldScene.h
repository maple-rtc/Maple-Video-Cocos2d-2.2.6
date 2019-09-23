#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "cocos2d.h"

#define MAPLE_APP_ID <##>

const struct
{
    char *name;
    const int x;
    const int y;
}remotePosition[] =
        {
                {"",  2,  2},
                {"",  3,  2},
                {"",  2,  3},
                {"",  3,  3}
        };


class HelloWorld : public cocos2d::CCLayer
{
public:
    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();  

    // there's no 'id' in cpp, so we recommend returning the class instance pointer
    static cocos2d::CCScene* scene();
    
    // a selector callback
    void menuCloseCallback(CCObject* pSender);
    
    // implement the "static node()" method manually
    CREATE_FUNC(HelloWorld);

public:

    void RenderLocalView();
    void RemoveLocalView();
    void RenderRemoteView(const char* uid);
    void RemoveRemoteView(const char* uid);


private:

    void * local_view_;
    std::map<std::string, void*>   remote_views_;

};

#endif // __HELLOWORLD_SCENE_H__
