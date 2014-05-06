//
//  PhysicsLineDrawTestScene.cpp
//  RockChipmunk2D
//
//  Created by ChildhoodAndy on 14-5-6.
//
//

#include "PhysicsLineDrawTestScene.h"
#include "VisibleRect.h"

// how long we construct a segment
#define SEGMENT_UNIT_LENGTH (10)
// fire degree
#define FIRE_DEGREE (45)
#define FIRE_ANGLE  (CC_DEGREES_TO_RADIANS(FIRE_DEGREE))
// ball's velocity apply
#define BALL_VELOCITY_APPLY 600

static const int BALL_CONTACT_TEST_MASK = 0x01; //发射球粒子的接触测试掩码
static const int TARGET_CONTACT_TEST_MASK = 0x01; //目标吉祥物接触测试掩码

Scene* PhysicsLineDrawTestScene::createScene()
{
    auto scene = Scene::createWithPhysics();
    scene->getPhysicsWorld()->setDebugDrawMask(PhysicsWorld::DEBUGDRAW_ALL);
    scene->getPhysicsWorld()->setGravity(Vect(0, -200));
    
    auto layer = PhysicsLineDrawTestScene::create();
    scene->addChild(layer);
    layer->setPhysicWorld(scene->getPhysicsWorld());
    return scene;
}

bool PhysicsLineDrawTestScene::init()
{
    if ( !BaseDemo::init()) {
        return false;
    }
    _canvas = DrawNode::create();
    addChild(_canvas);

    //发射器及其刚体
    auto shooter = Sprite::create("egg.png");
    shooter->setScale(0.4, 0.4);
    _shooterBody = PhysicsBody::createCircle(shooter->getBoundingBox().size.width/2);
    _shooterBody->setDynamic(false);
    shooter->setPhysicsBody(_shooterBody);
    shooter->setPosition(Point(100, 100));
    addChild(shooter);
    
    //碰撞监听
    auto contactListener = EventListenerPhysicsContact::create();
    contactListener->onContactBegin = CC_CALLBACK_1(PhysicsLineDrawTestScene::onContactBegin, this);
    contactListener->onContactPreSolve = CC_CALLBACK_2(PhysicsLineDrawTestScene::onContactPreSolve, this);
    contactListener->onContactPostSolve = CC_CALLBACK_2(PhysicsLineDrawTestScene::onContactPostSolve, this);
    contactListener->onContactSeperate = CC_CALLBACK_1(PhysicsLineDrawTestScene::onContactSeperate, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(contactListener, this);
    
    //生成目标吉祥物
    generateTarget();
    
    return true;
}

void PhysicsLineDrawTestScene::fireBalls(float dt)
{
    auto cos = cosf(FIRE_ANGLE);
    auto sin = sinf(FIRE_ANGLE);
    auto ball = createBall(Point(_shooterBody->getNode()->getPositionX() + 50 * cos, _shooterBody->getNode()->getPositionY() + 50 * sin), 6, PhysicsMaterial(0.02, 0.5, 0.5));
    addChild(ball);
    ball->getPhysicsBody()->setContactTestBitmask(BALL_CONTACT_TEST_MASK);
    
    //为球体设置一个速度，分别计算xy坐标方向的速度
    auto velocityX = BALL_VELOCITY_APPLY * cos;
    auto velocityY = BALL_VELOCITY_APPLY * sin;
    ball->getPhysicsBody()->setVelocity(Point(velocityX, velocityY));
}

void PhysicsLineDrawTestScene::generateTarget()
{
    auto target = Sprite::create("steroidtocat.png");
    target->setScale(0.3);
    _targetBody = PhysicsBody::createBox(target->getBoundingBox().size);
    _targetBody->setDynamic(false);
    _targetBody->setContactTestBitmask(TARGET_CONTACT_TEST_MASK);
    target->setPhysicsBody(_targetBody);
    
    int posx = arc4random() % 600 + 200;
    int posy = arc4random() % 400 + 200;
    target->setPosition(posx, posy);
    addChild(target);
}

bool PhysicsLineDrawTestScene::onContactBegin(PhysicsContact& contact)
{
    auto bodyA = contact.getShapeA()->getBody();
    auto bodyB = contact.getShapeB()->getBody();
    if (bodyA == _targetBody || bodyB == _targetBody)
    {
        auto target = (Sprite*)_targetBody->getNode();
        auto alpha = (target->getOpacity() - 10) <= 0 ? 0 : (target->getOpacity() - 10);
        target->setOpacity(alpha);
        if (alpha == 0)
        {
            target->removeFromParentAndCleanup(true);
            _targetBody = NULL;
            
            generateTarget();
        }
    }
    
    return true;
}

bool PhysicsLineDrawTestScene::onContactPreSolve(PhysicsContact& contact, PhysicsContactPreSolve& solve)
{
    return true;
}

void PhysicsLineDrawTestScene::onContactPostSolve(PhysicsContact& contact, const PhysicsContactPostSolve& solve)
{
    
}

void PhysicsLineDrawTestScene::onContactSeperate(PhysicsContact& contact)
{
    
}

bool PhysicsLineDrawTestScene::onTouchBegan(Touch* touch, Event* event)
{
    auto location = touch->getLocation();
    _prePoint = _curPoint = location;
    
    if ( _shooterBody->getFirstShape()->containsPoint(location) )
    {
        this->schedule(schedule_selector(PhysicsLineDrawTestScene::fireBalls), 0.1);
    }
    return true;
}

void PhysicsLineDrawTestScene::onTouchMoved(Touch* touch, Event* event)
{
    auto location = touch->getLocation();
    this->drawPath(location);
}

void PhysicsLineDrawTestScene::onTouchEnded(Touch* touch, Event* event)
{
    this->unschedule(schedule_selector(PhysicsLineDrawTestScene::fireBalls));
    
    auto location = touch->getLocation();
    this->drawPath(location);
    
    PhysicsBody* lineBody;
    Node* edgeNode;
    for (auto seg : _segments)
    {
        lineBody = PhysicsBody::createEdgeSegment(seg.p1, seg.p2);
        edgeNode = Node::create();
        edgeNode->setPhysicsBody(lineBody);
        addChild(edgeNode);
    }
    _segments.clear();
}

void PhysicsLineDrawTestScene::drawPath(Point& point)
{
    _curPoint = point;
    if ( (point - _prePoint).getLengthSq() > SEGMENT_UNIT_LENGTH * SEGMENT_UNIT_LENGTH )
    {
        _segment.p1 = _prePoint;
        _segment.p2 = _curPoint;
        _segments.push_back(_segment);
        
        _prePoint = _curPoint;
    }
    
    for (auto seg : _segments)
    {
        _canvas->drawSegment(seg.p1, seg.p2, 2, Color4F(1.0f, 1.0f, 0.0f, 0.8f));
    }
}



void PhysicsLineDrawTestScene::restartCallback(Ref* pSender)
{
    Director::getInstance()->replaceScene(PhysicsLineDrawTestScene::createScene());
}

std::string PhysicsLineDrawTestScene::demo_info() const
{
    return "绘制刚体线条 \n 1.按住鸡蛋不放，可以持续发射例子 \n 2.触摸绘制静态刚体线条 \n 3.粒子通过刚体线条阻挡引导接触到吉祥物时，吉祥物透明度降低 \n 4.当吉祥物透明度为0时，再次随机出现";
}