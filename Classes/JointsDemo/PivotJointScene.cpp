//
//  PivotJointScene.cpp
//  RockChipmunk2D
//
//  Created by ChildhoodAndy on 14-5-9.
// 【说明】这里的PivotJoint指的是Chipmunk中的cpPivotJoint
//        cocos2dx对此关节的封装名为PhysicsJointPin

#include "PivotJointScene.h"
#include "VisibleRect.h"

#define BAMBOO_SEGMENTS_NUM 11
#define BAMBOO_SEGMENT_SIZE Size(80, 20)

Scene* PivotJointScene::createScene()
{
    auto scene = Scene::createWithPhysics();
    scene->getPhysicsWorld()->setDebugDrawMask(PhysicsWorld::DEBUGDRAW_ALL);
    scene->getPhysicsWorld()->setGravity(Vect(0, -400));
    auto layer = PivotJointScene::create();
    layer->setPhysicWorld(scene->getPhysicsWorld());
    scene->addChild(layer);
    return scene;
}

void PivotJointScene::onEnter()
{
    BaseDemo::onEnter();
    auto visibleSize = VisibleRect::getVisibleRect().size;
    
    float pierPosY = 100.0f;
    Sprite* bridgePierL = createBox(Point(visibleSize.width/2 - 400, pierPosY), Size(40, 40));
    Sprite* bridgePierR = createBox(Point(visibleSize.width/2 + 400, pierPosY), Size(40, 40));
    bridgePierL->setRotation(0);
    bridgePierR->setRotation(0);
    bridgePierL->getPhysicsBody()->setDynamic(false);
    bridgePierR->getPhysicsBody()->setDynamic(false);
    bridgePierL->getPhysicsBody()->setTag(0);
    bridgePierR->getPhysicsBody()->setTag(0);
    this->addChild(bridgePierL);
    this->addChild(bridgePierR);
    
    float ballRadius = 40.0f;
    Sprite* fallingBall = createBall(Point(visibleSize.width/2, visibleSize.height-ballRadius), ballRadius);
    this->addChild(fallingBall);
    
    Vector<Sprite*> bamboos;
    for (int i = 0; i < BAMBOO_SEGMENTS_NUM; i++)
    {
        auto box = createBox(Point(bridgePierL->getPositionX() + BAMBOO_SEGMENT_SIZE.width * i, pierPosY), BAMBOO_SEGMENT_SIZE);
        box->setRotation(0);
        this->addChild(box);
        
        bamboos.pushBack(box);
    }
    
    PhysicsJointPin* pinJointL = PhysicsJointPin::construct(bridgePierL->getPhysicsBody(), bamboos.front()->getPhysicsBody(),bridgePierL->getPosition());
    PhysicsJointPin* pinJointR = PhysicsJointPin::construct(bridgePierR->getPhysicsBody(), bamboos.back()->getPhysicsBody(),bridgePierR->getPosition());
    pinJointL->setCollisionEnable(false);
    pinJointR->setCollisionEnable(false);
    _world->addJoint(pinJointL);
    _world->addJoint(pinJointR);

    
    for (int i = 0; i < BAMBOO_SEGMENTS_NUM - 1; i++)
    {
        auto body1 = bamboos.at(i)->getPhysicsBody();
        auto body2 = bamboos.at(i+1)->getPhysicsBody();
        
        PhysicsJointPin* pivotJointUp = PhysicsJointPin::construct(body1, body2, bamboos.at(i+1)->getPosition() + Point(-BAMBOO_SEGMENT_SIZE.width/2, BAMBOO_SEGMENT_SIZE.height/2));
        
        PhysicsJointPin* pivotJointDown = PhysicsJointPin::construct(body1, body2, bamboos.at(i+1)->getPosition() + Point(-BAMBOO_SEGMENT_SIZE.width/2, -BAMBOO_SEGMENT_SIZE.height/2));
        
        PhysicsJointRotarySpring* springJoint = PhysicsJointRotarySpring::construct(body1, body2, 2000, 0.8f);
        
        pivotJointUp->setCollisionEnable(false);
        pivotJointDown->setCollisionEnable(false);
        springJoint->setCollisionEnable(false);
        _world->addJoint(pivotJointUp);
        _world->addJoint(pivotJointDown);
        _world->addJoint(springJoint);
    }
}



std::string PivotJointScene::demo_info() const
{
    return "枢轴关节";
}

void PivotJointScene::restartCallback(Ref *pSender)
{
    Director::getInstance()->replaceScene(PivotJointScene::createScene());
}