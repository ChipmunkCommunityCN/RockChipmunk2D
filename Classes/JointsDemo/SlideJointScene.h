//
//  SlideJointScene.h
//  RockChipmunkWithCocos2DX
//
//  Created by ChildhoodAndy on 14-3-14.
//
//

#ifndef __RockChipmunkWithCocos2DX__SlideJointScene__
#define __RockChipmunkWithCocos2DX__SlideJointScene__

#include "cocos2d.h"
#include "BaseDemo.h"
USING_NS_CC;

class SlideJoint : BaseDemo
{
public:
    static cocos2d::Scene* createScene();
    virtual std::string demo_info() const override;
    virtual void onEnter() override;
    
    CREATE_FUNC(SlideJoint);
    
    virtual void restartCallback(Ref* pSender);
};

#endif /* defined(__RockChipmunkWithCocos2DX__SlideJointScene__) */
