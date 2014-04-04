//
//  ContactFilterScene.h
//  RockChipmunkWithCocos2DX
//
//  Created by ChildhoodAndy on 14-4-4.
//
//

#ifndef __RockChipmunkWithCocos2DX__ContactFilterScene__
#define __RockChipmunkWithCocos2DX__ContactFilterScene__

#include "cocos2d.h"
#include "BaseDemo.h"
USING_NS_CC;

class ContactFilterScene : BaseDemo
{
public:
    static cocos2d::Scene* createScene();
    virtual std::string demo_info() const override;
    virtual void onEnter() override;
    
    CREATE_FUNC(ContactFilterScene);
    
    virtual void restartCallback(Ref* pSender);
    
    bool onContactBegin(PhysicsContact& contact);
    bool onContactPreSolve(PhysicsContact& contact, PhysicsContactPreSolve& solve);
    void onContactPostSolve(PhysicsContact& contact, const PhysicsContactPostSolve& solve);
    void onContactSeperate(PhysicsContact& contact);
};

#endif /* defined(__RockChipmunkWithCocos2DX__ContactFilterScene__) */
