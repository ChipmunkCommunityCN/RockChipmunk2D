//
//  RollingBall.cpp
//  PhysicsTest
//
//  Created by ChildhoodAndy on 14-2-28.
//
//

#include "RollingBall.h"

static const int EGG_MASK = 0x01 << 0;
static const int CAI1_MASK = 0x01 << 1;
static const int CAI2_MASK = 0x01 << 2;
static const int WALL_MASK = 0x01 << 3;
Scene* RollingBall::createScene()
{
    auto scene = Scene::createWithPhysics();
    scene->getPhysicsWorld()->setDebugDrawMask(PhysicsWorld::DEBUGDRAW_ALL);
    scene->getPhysicsWorld()->setGravity(Vect(0, -200));
    
    auto layer = RollingBall::create();
    scene->addChild(layer);
    layer->setPhysicWorld(scene->getPhysicsWorld());
    return scene;
}

RollingBall::~RollingBall()
{
    CCLOG("RollingBall  destroy!");
}

bool RollingBall::init()
{
    if(!BaseDemo::init())
    {
        return false;
    }
    
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Point origin = Director::getInstance()->getVisibleOrigin();
    
    auto closeItem = MenuItemImage::create("CloseNormal.png", "CloseSelected.png", CC_CALLBACK_1(RollingBall::menuCloseCallBack, this));
    closeItem->setPosition(Point(origin.x + visibleSize.width - closeItem->getContentSize().width/2,
                                 origin.y + closeItem->getContentSize().height/2));
    auto menu = Menu::create(closeItem, NULL);
    menu->setPosition(Point::ZERO);
    this->addChild(menu, 1);
    
    
    auto body = PhysicsBody::createEdgeBox(visibleSize, PHYSICSBODY_MATERIAL_DEFAULT, 3);
//    body->setGroup(0);
//    body->setCategoryBitmask(WALL_MASK);
    auto edgeNode = Node::create();
    edgeNode->setPosition(visibleSize.width/2, visibleSize.height/2);
    edgeNode->setPhysicsBody(body);
    this->addChild(edgeNode);
    
    _verts_egg[0] = Point(1.9f, 88.2f);
    _verts_egg[1] = Point(41.8f, 67.5f);
    _verts_egg[2] = Point(64.1f, 25.2f);
    _verts_egg[3] = Point(66.3f, -32.5f);
    _verts_egg[4] = Point(44.5f, -66.6f);
    _verts_egg[5] = Point(0.8f, -87.4f);
    _verts_egg[6] = Point(-44.1f, -68.4f);
    _verts_egg[7] = Point(-62.6f, -34.9f);
    _verts_egg[8] = Point(-62.1f, 27.8f);
    _verts_egg[9] = Point(-39.7f, 68.9f);


   
    _verts_cai1[0] = Point(9.8f, 51.9f);
    _verts_cai1[1] = Point(25.3f, 46.1f);
    _verts_cai1[2] = Point(39.7f, 40.3f);
    _verts_cai1[3] = Point(45.9f, 29.3f);
    _verts_cai1[4] = Point(54.0f, 15.0f);
    _verts_cai1[5] = Point(53.5f, 2.6f);
    _verts_cai1[6] = Point(58.2f, -18.7f);
    _verts_cai1[7] = Point(48.5f, -27.6f);
    _verts_cai1[8] = Point(29.9f, -23.0f);
    _verts_cai1[9] = Point(16.4f, -26.4f);
    _verts_cai1[10] = Point(6.7f, -22.4f);
    _verts_cai1[11] = Point(-9.4f, -29.6f);
    _verts_cai1[12] = Point(-22.7f, -41.8f);
    _verts_cai1[13] = Point(-38.6f, -56.6f);
    _verts_cai1[14] = Point(-57.5f, -42.0f);
    _verts_cai1[15] = Point(-39.6f, -24.0f);
    _verts_cai1[16] = Point(-24.4f, 4.2f);
    _verts_cai1[17] = Point(-32.7f, 13.1f);
    _verts_cai1[18] = Point(-26.5f, 27.3f);
    _verts_cai1[19] = Point(-31.7f, 40.6f);
    _verts_cai1[20] = Point(-21.2f, 56.7f);
    _verts_cai1[21] = Point(-2.3f, 57.1f);
    
    //碰撞检测回调
    auto contactListener = EventListenerPhysicsContact::create();
    contactListener->onContactBegin = CC_CALLBACK_1(RollingBall::onContactBegin, this);
    contactListener->onContactPreSolve = CC_CALLBACK_2(RollingBall::onContactPreSolve, this);
    contactListener->onContactPostSolve = CC_CALLBACK_2(RollingBall::onContactPostSolve, this);
    contactListener->onContactSeperate = CC_CALLBACK_1(RollingBall::onContactSeperate, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(contactListener, this);
    
    return true;
}

bool RollingBall::onTouchBegan(Touch *touch, Event *unused_event)
{
    return true;
}

void RollingBall::onTouchEnded(Touch *touch, Event *unused_event)
{
    auto location = touch->getLocation();
    addNewSpriteAtPosition(location);
}

bool RollingBall::onContactBegin(PhysicsContact &contact)
{
    auto sprite = (Sprite*)contact.getShapeA()->getBody()->getNode();
    auto sprite2 = (Sprite*)contact.getShapeB()->getBody()->getNode();
    CCLOG("onContactBegin: sprite1 tag:%d", sprite->getTag());
    CCLOG("onContactBegin: sprite1 tag:%d", sprite2->getTag());
    return true;
}

bool RollingBall::onContactPreSolve(PhysicsContact &contact, PhysicsContactPreSolve &solve)
{
    CCLOG("pre solve");
    return true;
}

void RollingBall::onContactPostSolve(PhysicsContact &contact, const PhysicsContactPostSolve &solve)
{
//    CCLOG("post solve");
}

void RollingBall::onContactSeperate(PhysicsContact &contact)
{
//    CCLOG("seperate solve");
}


void RollingBall::addNewSpriteAtPosition(Point& location)
{
    int randIdx = CCRANDOM_0_1()*3;
    Sprite* sprite = nullptr;
    PhysicsBody* _body = nullptr;
    switch (randIdx) {
        case 0:
            sprite = Sprite::create("egg.png");
            _body = PhysicsBody::createPolygon(_verts_egg, 10, PhysicsMaterial(0.1, 1.5, 1), Vect(0,0));
            sprite->setTag(0);
            break;
        case 1:
            sprite = Sprite::create("cai1.png");
            _body = PhysicsBody::createEdgePolygon(_verts_cai1, 22);
            sprite->setTag(1);
            break;
        case 2:
            sprite = Sprite::create("cai2.png");
            _body = PhysicsBody::createCircle(55);
            sprite->setTag(2);
        default:
            break;
    }
    sprite->setPhysicsBody(_body);
    sprite->setPosition(location);
    this->addChild(sprite);
}

std::string RollingBall::demo_info() const
{
    return "点击屏幕看看吧:) \n顶点数据建构刚体";
}

void RollingBall::restartCallback(Ref *pSender)
{
    Director::getInstance()->replaceScene(RollingBall::createScene());
}

void RollingBall::menuCloseCallBack(Ref *pSender)
{
    if(_world->getDebugDrawMask() != PhysicsWorld::DEBUGDRAW_ALL)
    {
        _world->setDebugDrawMask(PhysicsWorld::DEBUGDRAW_ALL);
    }else
    {
        _world->setDebugDrawMask(PhysicsWorld::DEBUGDRAW_NONE);
    }
}