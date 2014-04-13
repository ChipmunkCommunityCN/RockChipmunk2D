//
//  BaseDemo.h
//  PhysicsTest
//
//  Created by ChildhoodAndy on 14-3-9.
//
//

#ifndef __PhysicsTest__BaseDemo__
#define __PhysicsTest__BaseDemo__

#include "cocos2d.h"
USING_NS_CC;

class ContentLayer;
class BaseDemo : public cocos2d::Layer
{
public:
    virtual bool init();
    void setPhysicWorld(PhysicsWorld* world) { _world = world; }
    virtual std::string demo_info() const;
    
    virtual void restartCallback(Ref* pSender);
    virtual void backCallback(Ref* pSender);
    
    virtual void onEnter() override;
    
    Sprite* createBall(const Point& point, float radius, PhysicsMaterial material = PHYSICSBODY_MATERIAL_DEFAULT);
    Sprite* createBox(const Point& point, Size size, PhysicsMaterial material = PHYSICSBODY_MATERIAL_DEFAULT);
    Sprite* createTriangle(const Point& point, Size size, PhysicsMaterial material = PHYSICSBODY_MATERIAL_DEFAULT);
    
    virtual bool onTouchBegan(Touch* touch, Event* event);
    virtual void onTouchMoved(Touch* touch, Event* event);
    virtual void onTouchEnded(Touch* touch, Event* event);
protected:
    PhysicsWorld* _world;
    Node* _wallNode;
    std::unordered_map<int, Node*> _mouses;
};

#endif /* defined(__PhysicsTest__BaseDemo__) */
