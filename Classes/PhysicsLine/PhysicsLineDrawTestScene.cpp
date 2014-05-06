//
//  PhysicsLineDrawTestScene.cpp
//  RockChipmunk2D
//
//  Created by ChildhoodAndy on 14-5-6.
//
//

#include "PhysicsLineDrawTestScene.h"
#include "VisibleRect.h"

#define SEGMENT_UNIT_LENGTH (20)
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
    
    this->removeChild(_bg);
    
    auto contactListener = EventListenerPhysicsContact::create();
    contactListener->onContactBegin = CC_CALLBACK_1(PhysicsLineDrawTestScene::onContactBegin, this);
    contactListener->onContactPreSolve = CC_CALLBACK_2(PhysicsLineDrawTestScene::onContactPreSolve, this);
    contactListener->onContactPostSolve = CC_CALLBACK_2(PhysicsLineDrawTestScene::onContactPostSolve, this);
    contactListener->onContactSeperate = CC_CALLBACK_1(PhysicsLineDrawTestScene::onContactSeperate, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(contactListener, this);
    
    this->schedule(schedule_selector(PhysicsLineDrawTestScene::fallBalls), 2.0);
    
    return true;
}

void PhysicsLineDrawTestScene::fallBalls(float dt)
{
    auto size = VisibleRect::getVisibleRect().size;
    auto ball = createBall(Point(CCRANDOM_0_1() * size.width + 100, size.height - 100), 30);
    addChild(ball);
}

bool PhysicsLineDrawTestScene::onContactBegin(PhysicsContact& contact)
{
    log("contact begin");
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
    return true;
}

void PhysicsLineDrawTestScene::onTouchMoved(Touch* touch, Event* event)
{
    auto location = touch->getLocation();
    this->checkCurPoint(location);
}

void PhysicsLineDrawTestScene::onTouchEnded(Touch* touch, Event* event)
{
    auto location = touch->getLocation();
    this->checkCurPoint(location);
    
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

void PhysicsLineDrawTestScene::checkCurPoint(Point& point)
{
    _curPoint = point;
    if ( (point - _prePoint).getLengthSq() > SEGMENT_UNIT_LENGTH * SEGMENT_UNIT_LENGTH )
    {
        _segment.p1 = _prePoint;
        _segment.p2 = _curPoint;
        _segments.push_back(_segment);
        
        _prePoint = _curPoint;
    }
}



void PhysicsLineDrawTestScene::restartCallback(Ref* pSender)
{
    Director::getInstance()->replaceScene(PhysicsLineDrawTestScene::createScene());
}

std::string PhysicsLineDrawTestScene::demo_info() const
{
    return "绘制刚体线条";
}

void PhysicsLineDrawTestScene::draw(Renderer *renderer, const kmMat4& transform, bool transformUpdated)
{
    Node::draw(renderer, transform, transformUpdated);
    DrawPrimitives::setDrawColor4B(0, 255, 255, 255);
    glLineWidth(4);

    for (auto seg : _segments)
    {

        DrawPrimitives::drawLine(seg.p1, seg.p2);
    }
}