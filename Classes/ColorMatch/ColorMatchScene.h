//
//  ColorMatchScene.h
//  RockChipmunk2D
//
//  Created by ChildhoodAndy on 14-4-7.
//
//

#ifndef __RockChipmunk2D__ColorMatchScene__
#define __RockChipmunk2D__ColorMatchScene__

#include "cocos2d.h"
#include "Ball.h"
USING_NS_CC;

class ColorMatchScene : Layer
{
public:
    static Scene* createScene();
    virtual bool init() override;
    virtual void update(float delta) override;
    CREATE_FUNC(ColorMatchScene);
    void setPhysicWorld(PhysicsWorld* world) { _world = world; }
    
    Ball* addBall();
    void removeBall(Ball* ball);
   
    void toggleDebugCallback(Ref* sender);
    
    bool onTouchBegan(Touch* touch, Event* event);
    bool onContactBegin(PhysicsContact& contact);
    bool onContactPreSolve(PhysicsContact& contact, PhysicsContactPreSolve& solve);
private:
    PhysicsWorld* _world;
    Vector<Ball*> _balls;
    int _ticks;
};


#endif /* defined(__RockChipmunk2D__ColorMatchScene__) */
