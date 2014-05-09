//
//  SpringJointScene.cpp
//  RockChipmunk2D
//
//  Created by ChildhoodAndy on 14-5-9.
//  1. 阻尼弹簧
//  2. 软体

#include "SpringJointScene.h"

#define ARROUND_BALLS_NUM 16
#define ARROUND_BALLS_RADIUS 4.0f
#define CENTER_BALL_POS Point(200, 200)
#define SOFT_BODY_RADIUS 100.0f

Scene* SpringJointScene::createScene()
{
    auto scene = Scene::createWithPhysics();
    scene->getPhysicsWorld()->setDebugDrawMask(PhysicsWorld::DEBUGDRAW_ALL);
    scene->getPhysicsWorld()->setGravity(Vect(0, -100));
    auto layer = SpringJointScene::create();
    layer->setPhysicWorld(scene->getPhysicsWorld());
    scene->addChild(layer);
    return scene;
}

void SpringJointScene::onEnter()
{
    BaseDemo::onEnter();
    
    // box1 box2 阻尼弹簧
    {
        auto box1 = createBox(Point(400, 200), Size(100, 100));
        auto box2 = createBox(Point(600, 400), Size(40, 40));
        this->addChild(box1);
        this->addChild(box2);
        
        //将box1和box2的刚体摩擦力设置为0，我们看下在无阻力的世界中，阻尼弹簧的表现效果
        box1->getPhysicsBody()->getFirstShape()->setFriction(0.0f);
        box2->getPhysicsBody()->getFirstShape()->setFriction(0.0f);
        
        PhysicsJointSpring* joint = PhysicsJointSpring::construct(box1->getPhysicsBody(), box2->getPhysicsBody(), Point::ANCHOR_MIDDLE, Point::ANCHOR_MIDDLE, 1000.0f, 0.8f);
        _world->addJoint(joint);
    }
    
    // 阻尼弹簧配合滑动限位关节构造软体
    // to be finished 
    {
        auto centerBall = createBall(CENTER_BALL_POS, SOFT_BODY_RADIUS);
        centerBall->getPhysicsBody()->setCategoryBitmask(0x02);
        centerBall->getPhysicsBody()->setContactTestBitmask(0x01);
        centerBall->getPhysicsBody()->setMass(1.0f/(float)ARROUND_BALLS_NUM);
        this->addChild(centerBall);

        auto test = 2*SOFT_BODY_RADIUS*sinf(2*M_PI/(float)ARROUND_BALLS_NUM);
        Vector<Sprite*> ballsArround;
        for (int i = 0; i < ARROUND_BALLS_NUM; i++)
        {
            auto angle = 2 * M_PI / ARROUND_BALLS_NUM * i;
            Point pos = CENTER_BALL_POS + Point::forAngle(angle) * SOFT_BODY_RADIUS;
            auto ball = createBall(pos, test/2);
            ball->getPhysicsBody()->setMoment(PHYSICS_INFINITY);
            ball->getPhysicsBody()->setMass(1.0f/(float)ARROUND_BALLS_NUM);
            ball->getPhysicsBody()->setCategoryBitmask(0x02);
            ball->getPhysicsBody()->setContactTestBitmask(0x01);
            ball->getPhysicsBody()->getFirstShape()->setFriction(1.0f);
            ball->getPhysicsBody()->getFirstShape()->setRestitution(0.5f);
            this->addChild(ball);
            ballsArround.pushBack(ball);
            
            auto body = ballsArround.at(i)->getPhysicsBody();
            PhysicsJointLimit* limitJoint = PhysicsJointLimit::construct(centerBall->getPhysicsBody(), body, Point::forAngle(angle) * SOFT_BODY_RADIUS, Point::ZERO, 0, SOFT_BODY_RADIUS*0.1);
            
            PhysicsJointSpring* springJoint = PhysicsJointSpring::construct(centerBall->getPhysicsBody(), body, Point::forAngle(angle) * (SOFT_BODY_RADIUS + test/2+4), Point::ZERO, 60, 0.75);
            springJoint->setRestLength(20);
            _world->addJoint(limitJoint);
            _world->addJoint(springJoint);
        }
        
        for (int i = 0; i < ARROUND_BALLS_NUM; i++)
        {
            auto body = ballsArround.at(i)->getPhysicsBody();
            auto nextArroundBody = ballsArround.at((i+1)%ARROUND_BALLS_NUM)->getPhysicsBody();
            
            PhysicsJointLimit* arroundEachLimitJoint = PhysicsJointLimit::construct(body, nextArroundBody, Point::ZERO, Point::ZERO, 0, test);
            _world->addJoint(arroundEachLimitJoint);
        }
        
        centerBall->getPhysicsBody()->applyImpulse(Point(400,400));
    }
}

std::string SpringJointScene::demo_info() const
{
    return "阻尼弹簧";
}

void SpringJointScene::restartCallback(Ref *pSender)
{
    Director::getInstance()->replaceScene(SpringJointScene::createScene());
}