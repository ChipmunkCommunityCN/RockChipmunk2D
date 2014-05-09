//
//  SpringJointScene.h
//  RockChipmunk2D
//
//  Created by ChildhoodAndy on 14-5-9.
//
//

#ifndef __RockChipmunk2D__SpringJointScene__
#define __RockChipmunk2D__SpringJointScene__

#include "cocos2d.h"
#include "BaseDemo.h"
USING_NS_CC;

class SpringJointScene : BaseDemo
{
public:
    static cocos2d::Scene* createScene();
    virtual std::string demo_info() const override;
    virtual void onEnter() override;
    
    CREATE_FUNC(SpringJointScene);
    
    virtual void restartCallback(Ref* pSender);
};

#endif /* defined(__RockChipmunk2D__SpringJointScene__) */
