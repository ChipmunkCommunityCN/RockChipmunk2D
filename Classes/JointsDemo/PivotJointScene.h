//
//  PivotJointScene.h
//  RockChipmunk2D
//
//  Created by ChildhoodAndy on 14-5-9.
//  PhysicsJointPin(cocos2dx)
//  cpPivotJoint(chipmunk)

#ifndef __RockChipmunk2D__PivotJointScene__
#define __RockChipmunk2D__PivotJointScene__

#include "cocos2d.h"
#include "BaseDemo.h"
USING_NS_CC;

class PivotJointScene : BaseDemo
{
public:
    static cocos2d::Scene* createScene();
    virtual std::string demo_info() const override;
    virtual void onEnter() override;
    
    CREATE_FUNC(PivotJointScene);
    
    virtual void restartCallback(Ref* pSender);
};

#endif /* defined(__RockChipmunk2D__PivotJointScene__) */
