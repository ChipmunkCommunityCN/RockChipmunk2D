//
//  ColorMatchScene.cpp
//  RockChipmunk2D
//
//  Created by ChildhoodAndy on 14-4-7.
//
//

#include "ColorMatchScene.h"
#include "CCPhysicsBody.h"
#include "SimpleAudioEngine.h"
#include "VisibleRect.h"
#include "CCPhysicsWorld.h"

const Point WALL_ORIGIN(124, 99);
const Size WALL_SIZE(720, 530);
const int BALL_TAG = 1;
Scene* ColorMatchScene::createScene()
{
    auto scene = Scene::createWithPhysics();
    scene->getPhysicsWorld()->setDebugDrawMask(PhysicsWorld::DEBUGDRAW_ALL);
    scene->getPhysicsWorld()->setGravity(Vect(0, -500));
//    scene->setPhysicsSimulateEnabled(false);
    
    auto layer = ColorMatchScene::create();
    scene->addChild(layer);
    layer->setPhysicWorld(scene->getPhysicsWorld());

    return scene;
}

bool ColorMatchScene::init()
{
    if(!Layer::init())
    {
        return false;
    }
    
    Size visibleSize = Director::getInstance()->getVisibleSize();
    auto bg = Sprite::create("bg.png");
    this->addChild(bg);
    bg->setPosition(Point(visibleSize.width/2, visibleSize.height/2));
    
    PhysicsBody* wall_body = PhysicsBody::create();
    PhysicsShape* l_shape = PhysicsShapeEdgeSegment::create(WALL_ORIGIN, WALL_ORIGIN+Point(0, WALL_SIZE.height));
    PhysicsShape* b_shape = PhysicsShapeEdgeSegment::create(WALL_ORIGIN, WALL_ORIGIN+Point(WALL_SIZE.width, 0));
    PhysicsShape* r_shape = PhysicsShapeEdgeSegment::create(WALL_ORIGIN+Point(WALL_SIZE.width, 0), WALL_ORIGIN+Point(WALL_SIZE.width, WALL_SIZE.height));
    wall_body->addShape(l_shape);
    wall_body->addShape(b_shape);
    wall_body->addShape(r_shape);
    wall_body->setDynamic(false);
    
    auto wall_node = Node::create();
    wall_node:setPhysicsBody(wall_body);
    this->addChild(wall_node);
    
    MenuItemFont::setFontSize(18);
    auto item = MenuItemFont::create("Toggle debug", CC_CALLBACK_1(ColorMatchScene::toggleDebugCallback, this));
    
    auto menu = Menu::create(item, NULL);
    this->addChild(menu);
    menu->setPosition(Point(VisibleRect::right().x-50, VisibleRect::top().y-10));
    
    this->scheduleUpdate();
    
    auto touchListener = EventListenerTouchOneByOne::create();
    touchListener->onTouchBegan = CC_CALLBACK_2(ColorMatchScene::onTouchBegan, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(touchListener, this);
    
    auto contactListener = EventListenerPhysicsContact::create();
    contactListener->onContactBegin = CC_CALLBACK_1(ColorMatchScene::onContactBegin, this);
    contactListener->onContactPreSolve = CC_CALLBACK_2(ColorMatchScene::onContactPreSolve, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(contactListener, this);
    
    return true;
}

Ball* ColorMatchScene::addBall()
{
    Ball* ball = Ball::createBall();
    this->addChild(ball);
    


    ball->setTag(BALL_TAG);
    
    auto bornPosX = CCRANDOM_0_1() * (WALL_SIZE.width - ball->getRadius() * 2) + WALL_ORIGIN.x + ball->getRadius();
    ball->setPosition(Point(bornPosX, 660));
    
    _balls.pushBack(ball);
    
    return ball;
}

void ColorMatchScene::removeBall(Ball *ball)
{
    ball->removeFromParentAndCleanup(true);
    
    ParticleSystem *particle = ParticleSystemQuad::create("pop.plist");
    particle->setPosition(ball->getPosition());
    particle->setAutoRemoveOnFinish(true);
    this->addChild(particle, 10);
    
    _balls.eraseObject(ball);
    
    CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("pop.wav");
}

const float TICKS_PER_SECOND = 120.0f;
void ColorMatchScene::update(float delta)
{
    if (_ticks % 6 == 0 && _balls.size() < 70)
    {
        addBall();
    }
    
    
    Vector<Ball*> _ballsCopy = _balls;
    for(auto& ball : _ballsCopy)
    {
        Ball* root = ball->getRoot();
        char count[2];
        sprintf(count, "%d", root->getLinkCount());
        ball->linkCountLabel->setString(count);
        if (root->getLinkCount() >= 4)
        {
            removeBall(ball);
            CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("ploop.wav");
        }
    }
    for(auto& ball : _balls)
    {
        ball->setLinkCount(1);
        ball->setRoot(ball);
    }
      
    _ticks ++;
}

bool ColorMatchScene::onTouchBegan(Touch* touch, Event* event)
{
    auto location = touch->getLocation();
    auto shape = _world->getShape(location);
    if(shape)
    {
        Ball *ball = dynamic_cast<Ball*>(shape->getBody()->getNode());
        if(ball->getTag() == BALL_TAG)
        {
            removeBall(ball);
        }
    }
    
    return true;
}

bool ColorMatchScene::onContactBegin(PhysicsContact& contact)
{
    return true;
}

bool ColorMatchScene::onContactPreSolve(PhysicsContact &contact, PhysicsContactPreSolve &solve)
{
    PhysicsBody* a = contact.getShapeA()->getBody();
    PhysicsBody* b = contact.getShapeB()->getBody();
    Ball* ballA = dynamic_cast<Ball*>(a->getNode());
    Ball* ballB = dynamic_cast<Ball*>(b->getNode());
    
    if (a->getTag() == b->getTag())
    {
        Ball* rootA = ballA->getRoot();
        Ball* rootB = ballB->getRoot();
        
        if (rootA != rootB)
        {
            rootA->setRoot(rootB->getRoot());
            int linkCount = rootA->getLinkCount() + rootB->getLinkCount();
            rootA->setLinkCount(linkCount);
            rootB->setLinkCount(linkCount);
        }
    }
    return true;
}

void ColorMatchScene::toggleDebugCallback(Ref* sender)
{
    if (_world->getDebugDrawMask() == PhysicsWorld::DEBUGDRAW_ALL)
    {
        _world->setDebugDrawMask(PhysicsWorld::DEBUGDRAW_NONE);
    }
    else
    {
        _world->setDebugDrawMask(PhysicsWorld::DEBUGDRAW_ALL);
    }
}