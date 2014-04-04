//
//  ContactFilterScene.cpp
//  RockChipmunkWithCocos2DX
//
//  Created by ChildhoodAndy on 14-4-4.
//
//

#include "ContactFilterScene.h"
#include "CCPhysicsBody.h"

Scene* ContactFilterScene::createScene()
{
    auto scene = Scene::createWithPhysics();
    scene->getPhysicsWorld()->setDebugDrawMask(PhysicsWorld::DEBUGDRAW_ALL);
    scene->getPhysicsWorld()->setGravity(Vect(0, -400));
    auto layer = ContactFilterScene::create();
    layer->setPhysicWorld(scene->getPhysicsWorld());
    scene->addChild(layer);
    return scene;
}

void ContactFilterScene::onEnter()
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
    
    PhysicsBody *ball1_body = ball1->getPhysicsBody();
    ball1_body->setCategoryBitmask(0x01);
    ball1_body->setContactTestBitmask(0x00);
    ball1_body->setCollisionBitmask(0x01);
    
    PhysicsBody *ball2_body = ball2->getPhysicsBody();
    ball2_body->setCategoryBitmask(0x01);
    ball2_body->setContactTestBitmask(0x00);
    ball2_body->setCollisionBitmask(0x01);
    
    PhysicsBody *box1_body = box1->getPhysicsBody();
    box1_body->setCategoryBitmask(0x02);
    box1_body->setCollisionBitmask(0x04);
    PhysicsBody *box2_body = box2->getPhysicsBody();
    box2_body->setCategoryBitmask(0x02);
    
    auto contactListener = EventListenerPhysicsContactWithBodies::create(ball1_body, ball2_body);
    contactListener->onContactBegin = CC_CALLBACK_1(ContactFilterScene::onContactBegin, this);
    contactListener->onContactPreSolve = CC_CALLBACK_2(ContactFilterScene::onContactPreSolve, this);
    contactListener->onContactPostSolve = CC_CALLBACK_2(ContactFilterScene::onContactPostSolve, this);
    contactListener->onContactSeperate = CC_CALLBACK_1(ContactFilterScene::onContactSeperate, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(contactListener, this);
}

bool ContactFilterScene::onContactBegin(PhysicsContact& contact)
{
    CCLOG("ball1_body ball2_body contact begin");
    return true;
}

bool ContactFilterScene::onContactPreSolve(PhysicsContact& contact, PhysicsContactPreSolve& solve)
{
    CCLOG("ball1_body ball2_body presolve");
    return true;
}

void ContactFilterScene::onContactPostSolve(PhysicsContact& contact, const PhysicsContactPostSolve& solve)
{
    CCLOG("ball1_body ball2_body post solve");
}

void ContactFilterScene::onContactSeperate(PhysicsContact& contact)
{
    CCLOG("ball1_body ball2_body separate");
}


std::string ContactFilterScene::demo_info() const
{
    return "05 了解碰撞过滤 \n 详情查看控制台输出";
}

void ContactFilterScene::restartCallback(Ref *pSender)
{
    Director::getInstance()->replaceScene(ContactFilterScene::createScene());
}