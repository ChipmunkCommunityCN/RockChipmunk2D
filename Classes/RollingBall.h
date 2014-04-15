//
//  RollingBall.h
//  PhysicsTest
//
//  Created by ChildhoodAndy on 14-2-28.
//
//

#ifndef __PhysicsTest__RollingBall__
#define __PhysicsTest__RollingBall__

#include "cocos2d.h"
#include "BaseDemo.h"
USING_NS_CC;

class RollingBall : BaseDemo
{
public:
    static cocos2d::Scene* createScene();
    virtual bool init();
    ~RollingBall();
    void menuCloseCallBack(Ref* pSender);
    CREATE_FUNC(RollingBall);
    
    
    void setPhysicWorld(PhysicsWorld* world) { _world = world; }
    void addNewSpriteAtPosition(Point& location);
    
    virtual std::string demo_info() const override;
    virtual void restartCallback(Ref* pSender) override;
    virtual bool onTouchBegan(Touch* touch, Event* unused_event) override;
    virtual void onTouchEnded(Touch* touch, Event* unused_event) override;
    
    bool onContactBegin(PhysicsContact& contact);
    bool onContactPreSolve(PhysicsContact& contact, PhysicsContactPreSolve& solve);
    void onContactPostSolve(PhysicsContact& contact, const PhysicsContactPostSolve& solve);
    void onContactSeperate(PhysicsContact& contact);
private:
    PhysicsWorld* _world;
    Point _verts_egg[10];
    Point _verts_cai1[22];
};

#endif /* defined(__PhysicsTest__RollingBall__) */
