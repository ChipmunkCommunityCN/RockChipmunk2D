//
//  HitMeScene.cpp
//  RockChipmunk2D
//
//  Created by ChildhoodAndy on 14-4-15.
//
//

#include "HitMeScene.h"
#include "VisibleRect.h"
#include "ContentLayer.h"

Scene* HitMeScene::createScene()
{
	auto scene = Scene::createWithPhysics();
	scene->getPhysicsWorld()->setDebugDrawMask(PhysicsWorld::DEBUGDRAW_ALL);
	scene->getPhysicsWorld()->setGravity(Point::ZERO);
    
	auto layer = HitMeScene::create();
	layer->setPhyWorld(scene->getPhysicsWorld());
	scene->addChild(layer);
    
	return scene;
}

bool HitMeScene::init()
{
    if (!Layer::init())
    {
        return false;
    }
    
    Size size = VisibleRect::getVisibleRect().size;
    
    _bgColorLayer = LayerColor::create(Color4B(0, 0, 0, 255));
    addChild(_bgColorLayer);
    
    //加入封闭的盒子，用作墙壁
    auto body = PhysicsBody::createEdgeBox(size, PHYSICSBODY_MATERIAL_DEFAULT, 3);
    body->setContactTestBitmask(0x000001);
    auto wallNode = Node::create();
    wallNode->setPosition(size.width/2, size.height/2);
    wallNode->setPhysicsBody(body);
    
    this->addChild(wallNode);
    
    auto demo_info_label = Label::create(demo_info(), "Helvetica", 25, Size(500, 200), TextHAlignment::LEFT);
    this->addChild(demo_info_label);
    demo_info_label->setAnchorPoint(Point(0.0f, 1.0f));
    demo_info_label->setColor(Color3B(255, 255, 255));
    demo_info_label->setPosition(VisibleRect::leftTop() + Point(60, -60));
    
    _hitItem = MenuItemImage::create("hit_me.png", "hit_me.png", CC_CALLBACK_1(HitMeScene::hitMeFire, this));
    auto backMenuItem = MenuItemImage::create("backNormal.png", "backSelected.png", CC_CALLBACK_1(HitMeScene::backCallback, this));
    auto restartMenuItem = MenuItemImage::create("refreshNormal.png", "refreshSelected.png", CC_CALLBACK_1(HitMeScene::restartCallback, this));
    
    _hitItem->setPosition(VisibleRect::center());
    restartMenuItem->setPosition(VisibleRect::rightTop() + Point(-200, -80));
    backMenuItem->setPosition(restartMenuItem->getPosition() + Point(90, 0));
    auto menu = Menu::create(backMenuItem, restartMenuItem, _hitItem, NULL);
    menu->setPosition(Point::ZERO);
    this->addChild(menu, 300);
    
    setPhysicsForHitItem();
    
    auto contactListener = EventListenerPhysicsContact::create();
    contactListener->onContactBegin = CC_CALLBACK_1(HitMeScene::onContactBegin, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(contactListener, this);
    
    return true;
}

void HitMeScene::setPhysicsForHitItem()
{
    PhysicsBody* body = PhysicsBody::createBox(_hitItem->getContentSize(), PhysicsMaterial(0.3f, 0.3f, 0.3f));
    body->setContactTestBitmask(0x000001);
    _hitItem->setPhysicsBody(body);
}

bool HitMeScene::onContactBegin(PhysicsContact& contact)
{
    Color3B randomColor(CCRANDOM_0_1()*255, CCRANDOM_0_1()*255, CCRANDOM_0_1()*255);
    _bgColorLayer->setColor(randomColor);
    return true;
}

void HitMeScene::hitMeFire(Ref* sender)
{
    Point velocity_delta = Point(CCRANDOM_0_1(), CCRANDOM_0_1()) * 300;
    velocity_delta = (CCRANDOM_0_1() < 0.5f)? velocity_delta : -velocity_delta;
    PhysicsBody* body = _hitItem->getPhysicsBody();
    body->setVelocity(body->getVelocity() + velocity_delta);
    body->setAngularVelocity(body->getAngularVelocity() + 5.0f * CCRANDOM_0_1());
}


std::string HitMeScene::demo_info() const
{
    return "点击图片，图片将会向随机方向运动";
}


void HitMeScene::restartCallback(Ref *pSender)
{
    Director::getInstance()->replaceScene(HitMeScene::createScene());
}

void HitMeScene::backCallback(Ref *pSender)
{
    Director::getInstance()->replaceScene(ContentLayer::createScene());
}