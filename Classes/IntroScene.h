//
//  IntroScene.h
//  PhysicsTest
//
//  Created by ChildhoodAndy on 14-3-9.
//
//

#ifndef __PhysicsTest__IntroScene__
#define __PhysicsTest__IntroScene__

#include "cocos2d.h"
USING_NS_CC;

class IntroScene : Layer
{
public:
    static Scene* createScene();
    virtual bool init() override;
    virtual void update(float delta) override;
    virtual void onEnter() override;
    virtual void onExit() override;

    
    CREATE_FUNC(IntroScene);
    void play(Ref* pSender);    
    MenuItemImage* _playMenuItem;
    
private:
    float _counter;
};

#endif /* defined(__PhysicsTest__IntroScene__) */
