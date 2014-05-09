//
//  AABBQueryScene.h
//  RockChipmunk2D
//
//  Created by ChildhoodAndy on 14-4-18.
//
//

#ifndef __RockChipmunk2D__AABBQueryScene__
#define __RockChipmunk2D__AABBQueryScene__

#include "cocos2d.h"
#include "BaseDemo.h"
USING_NS_CC;
class AABBQueryScene : BaseDemo
{
public:
    static Scene* createScene();
    virtual bool init() override;
    virtual std::string demo_info() const override;
    
    virtual void update(float dt) override;
    virtual void restartCallback(Ref* pSender);
    virtual bool onTouchBegan(Touch* touch, Event* event) override;
    virtual void onTouchEnded(Touch* touch, Event* event) override;
    
    CREATE_FUNC(AABBQueryScene);
};

#endif /* defined(__RockChipmunk2D__AABBQueryScene__) */
