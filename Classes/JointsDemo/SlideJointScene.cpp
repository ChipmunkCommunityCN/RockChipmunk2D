//
//  SlideJointScene.cpp
//  RockChipmunkWithCocos2DX
//
//  Created by ChildhoodAndy on 14-3-14.
//
//

#include "SlideJointScene.h"

Scene* SlideJoint::createScene()
{
    auto scene = Scene::createWithPhysics();
    scene->getPhysicsWorld()->setDebugDrawMask(PhysicsWorld::DEBUGDRAW_ALL);
    scene->getPhysicsWorld()->setGravity(Vect(0, -500));
    auto layer = SlideJoint::create();
    layer->setPhysicWorld(scene->getPhysicsWorld());
    scene->addChild(layer);
    return scene;
}

void SlideJoint::onEnter()
{
    BaseDemo::onEnter();
    
    auto box1 = createBox(Point(400, 200), Size(100, 100));
    auto box2 = createBox(Point(600, 400), Size(100, 100));
    this->addChild(box1);
    this->addChild(box2);
    
    PhysicsJointLimit * box_joint = PhysicsJointLimit::construct(box1->getPhysicsBody(), box2->getPhysicsBody(), Point(0, 0), Point(0, 0), 120, 200);
    _world->addJoint(box_joint);
    
    Sprite* box = nullptr;
    PhysicsJointLimit* chainJoint_limit = nullptr;
    int chainLen = 10;
    Vector<Sprite*> boxes;
    for (int i = 0; i < chainLen; i++) {
        box = createBox(Point(500 + 40 * i, 600), Size(20, 10));
        this->addChild(box);
        boxes.pushBack(box);
        if (i == 0)
        {
            box->getPhysicsBody()->setDynamic(false);
        }
        if(i > 0)
        {
            chainJoint_limit = PhysicsJointLimit::construct(boxes.at(i-1)->getPhysicsBody(), boxes.at(i)->getPhysicsBody(), Point(10, 0), Point(-10, 0), 10, 20);
            _world->addJoint(chainJoint_limit);
        }
    }
}

std::string SlideJoint::demo_info() const
{
    return "04 了解SlideJoint【滑动关节】";
}

void SlideJoint::restartCallback(Ref *pSender)
{
    Director::getInstance()->replaceScene(SlideJoint::createScene());
}