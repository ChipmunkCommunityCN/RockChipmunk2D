//
//  PinJointScene.h
//  PhysicsTest
//
//  Created by ChildhoodAndy on 14-3-11.
//
//

#ifndef __PhysicsTest__PinJointScene__
#define __PhysicsTest__PinJointScene__

#include "cocos2d.h"
#include "BaseDemo.h"
USING_NS_CC;

class PinJoint : BaseDemo
{
public:
    static cocos2d::Scene* createScene();
    virtual std::string demo_info() const override;
    virtual void onEnter() override;
    
    CREATE_FUNC(PinJoint);
    
    virtual void restartCallback(Ref* pSender);
};

#endif /* defined(__PhysicsTest__PinJointScene__) */
