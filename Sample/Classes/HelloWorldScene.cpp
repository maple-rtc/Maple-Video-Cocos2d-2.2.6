#include "HelloWorldScene.h"

#include "MapleViewHelp.h"


#include "../BMGamingSDK/include/BMediaNativeCore.h"
#include "../BMGamingSDK/include/jsonRtc.h"

USING_NS_CC;

using namespace maple;

static BMediaKit* mBMedia = nullptr;
static std::string ownerUid = "";
bool exitFlag = false;


class MyIGamingRtcEngineEventHandler : public IBMediaEventHandler
{
private:
    HelloWorld* mUi;

public:
    MyIGamingRtcEngineEventHandler(HelloWorld* ui) :mUi(ui) {
    }

    ~MyIGamingRtcEngineEventHandler() {
    }

    void onJoinChannelSuccess(const char* channel, const char* uid) override
    {
        CCLOG("[General C++]:onJoinChannelSuccess, channel:%s, uid:%s", channel, uid);
        ownerUid = uid;
        std::stringstream rawMsg;
        rawMsg << "onJoinChannelSuccess, channel:" << channel << ", uid:" << uid;

        mUi->RenderLocalView();

        //mUi->updateMsgContent(rawMsg.str());
    }

    void onLeaveChannel(int reason) override
    {
        CCLOG("[General C++]:onLeaveChannel, reason:%d ", reason);
        std::stringstream rawMsg;
        rawMsg << "onLeaveChannel, reason:" << reason;
        //mUi->updateMsgContent(rawMsg.str());
        if(exitFlag) {
            exitFlag = false;
            // Close the cocos2d-x game scene and quit the application
            CCDirector::sharedDirector()->end();

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
            exit(0);
#endif
        }

    }

    void onUserJoinedNotice(const char* uids) override {
        Json::Reader reader;
        Json::Value value;
        if(!reader.parse(uids, value)){
            return;
        }
        std::vector<std::string> new_uids;
        if(!rtc::JsonArrayToStringVector(value, &new_uids)){
            return;
        }

        for(int i = 0; i < new_uids.size(); i++){
            if(new_uids[i].find(ownerUid, 0) != std::string::npos)
                continue;
            mUi->RenderRemoteView(new_uids[i].c_str());
        }
        std::stringstream rawMsg;
        rawMsg << "onUserJoinedNotice, uids:"<<  uids;
        //mUi->updateMsgContent(rawMsg.str());
    }

    void onUserOfflineNotice(const char* uids) override {
        std::stringstream rawMsg;
        rawMsg << "onUserOfflineNotice, uids:" << uids;
        //mUi->updateMsgContent(rawMsg.str());
    }

    void onForceKickOutChannel(int reason) override{
        std::stringstream rawMsg;
        rawMsg << "onForceKickOutChannel, reason:" << reason;
       // mUi->updateMsgContent(rawMsg.str());
    }

    void onAudioVolumeIndication(int volume) override{

    }

};

CCScene* HelloWorld::scene()
{
    // 'scene' is an autorelease object
    CCScene *scene = CCScene::create();
    
    // 'layer' is an autorelease object
    HelloWorld *layer = HelloWorld::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool HelloWorld::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !CCLayer::init() )
    {
        return false;
    }
    
    CCSize visibleSize = CCDirector::sharedDirector()->getVisibleSize();
    CCPoint origin = CCDirector::sharedDirector()->getVisibleOrigin();

    /////////////////////////////
    // 2. add a menu item with "X" image, which is clicked to quit the program
    //    you may modify it.

    // add a "close" icon to exit the progress. it's an autorelease object
    CCMenuItemImage *pCloseItem = CCMenuItemImage::create(
                                        "CloseNormal.png",
                                        "CloseSelected.png",
                                        this,
                                        menu_selector(HelloWorld::menuCloseCallback));
    
	pCloseItem->setPosition(ccp(origin.x + visibleSize.width - pCloseItem->getContentSize().width/2 ,
                                origin.y + pCloseItem->getContentSize().height/2));

    // create menu, it's an autorelease object
    CCMenu* pMenu = CCMenu::create(pCloseItem, NULL);
    pMenu->setPosition(CCPointZero);
    this->addChild(pMenu, 1);

    /////////////////////////////
    // 3. add your codes below...

    // add a label shows "Hello World"
    // create and initialize a label
    std::stringstream title;
    title << " BMGame ";
    title << BMediaKit_getInstance(MAPLE_APP_ID)->getSdkVersion();
    title << " DEMO";
    CCLabelTTF* pLabel = CCLabelTTF::create(title.str().c_str(), "Arial", 24);

    // position the label on the center of the screen
    pLabel->setPosition(ccp(origin.x + visibleSize.width/2,
                            origin.y + visibleSize.height - pLabel->getContentSize().height));

    // add the label as a child to this layer
    this->addChild(pLabel, 1);

    // add "HelloWorld" splash screen"
    CCSprite* pSprite = CCSprite::create("HelloWorld.png");

    // position the sprite on the center of the screen
    pSprite->setPosition(ccp(visibleSize.width/2 + origin.x, visibleSize.height/2 + origin.y));

    // add the sprite as a child to this layer
    this->addChild(pSprite, 0);

    mBMedia = BMediaKit_getInstance(MAPLE_APP_ID);

    if(mBMedia){
        mBMedia->setEventHandler(new MyIGamingRtcEngineEventHandler(this));

        mBMedia->setChannelProfile(CHANNEL_PROFILE_VOICE_VIDEO);

        int version = mBMedia->getSdkVersion();

    }
    //join Channel
    mBMedia->joinChannel("1111");
    
    return true;
}


void HelloWorld::menuCloseCallback(CCObject* pSender)
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WINRT) || (CC_TARGET_PLATFORM == CC_PLATFORM_WP8)
	CCMessageBox("You pressed the close button. Windows Store Apps do not implement a close button.","Alert");
#else
    CCDirector::sharedDirector()->end();
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif
#endif
}



void HelloWorld::RenderLocalView()
{
    local_view_= mBMedia->createRenderView();
    MapleViewHelp::SetSurfaceView(local_view_, remotePosition[0].x, remotePosition[0].y);
    BMVideoCanvas vc;
    vc.view = local_view_;
    mBMedia->setupLocalVideo(vc);
    mBMedia->startPreview();
    mBMedia->muteLocalVideoStream(false);
}

void HelloWorld::RemoveLocalView()
{
    if(local_view_){
        int ret =  mBMedia->muteLocalVideoStream(true);
        mBMedia->stopPreview();
        BMVideoCanvas vc;
        vc.view = nullptr;
        mBMedia->setupLocalVideo(vc);
        MapleViewHelp::RemoveSurfaceView(local_view_);
        local_view_ = nullptr;
    }
}

void HelloWorld::RenderRemoteView(const char* uid)
{
    //support max 4 users
    if(remote_views_.size() > 4){
        return;
    }

    auto it = remote_views_.find(uid);
    if (it != remote_views_.end()) {
        return;
    }

    std::string uid_ = uid;

    /*for(int i = 0; i < 4; i ++){
        if(uid_.find(remotePosition[i].name, 0) != std::string::npos)
            return;
    }*/

    remote_views_[uid] = mBMedia->createRenderView();
    MapleViewHelp::SetSurfaceView(remote_views_[uid], remotePosition[1].x, remotePosition[1].y);
    BMVideoCanvas vc;
    vc.view = remote_views_[uid];
    vc.uid = (char*) uid;
    mBMedia->setupRemoteVideo(vc);
    mBMedia->muteRemoteVideoStream(uid, false);
}


void HelloWorld::RemoveRemoteView(const char* uid)
{
    auto it = remote_views_.find(uid);
    if (it != remote_views_.end()) {
        //停止接受远端视频
        int ret =  mBMedia->muteRemoteVideoStream(uid, true);
        //解绑远端视频view
        BMVideoCanvas vc;
        vc.view = nullptr;
        vc.uid = (char*) uid;
        mBMedia->setupRemoteVideo(vc);
        MapleViewHelp::RemoveSurfaceView(remote_views_[uid]);
        remote_views_.erase(uid);
    }
}
