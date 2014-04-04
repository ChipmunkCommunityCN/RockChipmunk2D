//
//  PinJointScene.cpp
//  PhysicsTest
//
//  Created by ChildhoodAndy on 14-3-11.
//
//

#include "PinJointScene.h"

Scene* PinJoint::createScene()
{
    auto scene = Scene::createWithPhysics();
    scene->getPhysicsWorld()->setDebugDrawMask(PhysicsWorld::DEBUGDRAW_ALL);
    scene->getPhysicsWorld()->setGravity(Vect(0, -400));
    auto layer = PinJoint::create();
    layer->setPhysicWorld(scene->getPhysicsWorld());
    scene->addChild(layer);
    return scene;
}

void PinJoint::onEnter()
{
    BaseDemo::onEnter();
    
    auto ball1 = createBall(Point(100, 100), 50, PHYSICSBODY_MATERIAL_DEFAULT);
    auto ball2 = createBall(Point(400, 100), 50, PHYSICSBODY_MATERIAL_DEFAULT);
    auto box1 = createBox(Point(400, 200), Size(100, 100));
    auto box2 = createBox(Point(600, 400), Size(100, 100));
    this->addChild(ball1);
    this->addChild(ball2);
    this->addChild(box1);
    this->addChild(box2);
    
    PhysicsJointDistance* ball_joint = PhysicsJointDistance::construct(ball1->getPhysicsBody(), ball2->getPhysicsBody(), Point(25, 0), Point(-25, 0));
    PhysicsJointDistance* box_joint = PhysicsJointDistance::construct(box1->getPhysicsBody(), box2->getPhysicsBody(), Point(0, 0), Point(50, 50));
    _world->addJoint(ball_joint);
    _world->addJoint(box_joint);
    
    Sprite* box = nullptr;
    PhysicsJointDistance* chainJoint_distance = nullptr;
    int chainLen = 15;
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
            chainJoint_distance = PhysicsJointDistance::construct(boxes.at(i-1)->getPhysicsBody(), boxes.at(i)->getPhysicsBody(), Point(10, 0), Point(-10, 0));
            chainJoint_distance->setDistance(10);
            _world->addJoint(chainJoint_distance);
        }
    }
}

std::string PinJoint::demo_info() const
{
    return "03 了解PinJoint【销关节】";
}

void PinJoint::restartCallback(cocos2d::Ref *pSender)
{
    Director::getInstance()->replaceScene(PinJoint::createScene());
}