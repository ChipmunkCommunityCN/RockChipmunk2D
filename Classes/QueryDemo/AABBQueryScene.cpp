//
//  AABBQueryScene.cpp
//  RockChipmunk2D
//
//  Created by ChildhoodAndy on 14-4-18.
//
//

#include "AABBQueryScene.h"
#include "VisibleRect.h"

USING_NS_CC;

const int ROTATE_LINE_LENGTH = 400;
static const Color4F LINE_COLOR = Color4F(0.0f, 1.0f, 1.0f, 1.0f);
static const Color4F DOT_COLOR = Color4F(0.0f, 1.0f, 0.0f, 1.0f);
Scene* AABBQueryScene::createScene()
{
    auto scene = Scene::createWithPhysics();
    scene->getPhysicsWorld()->setDebugDrawMask(PhysicsWorld::DEBUGDRAW_ALL);
    scene->getPhysicsWorld()->setGravity(Point::ZERO);
    
    auto layer = AABBQueryScene::create();
    scene->addChild(layer);
    layer->setPhysicWorld(scene->getPhysicsWorld());
    return scene;
}

bool AABBQueryScene::init()
{
    if (!BaseDemo::init())
    {
        return false;
    }
    
    // to be finished, will you help me :) ?
    
    scheduleUpdate();
    
    return true;
}

void AABBQueryScene::update(float dt)
{
}

bool AABBQueryScene::onTouchBegan(cocos2d::Touch *touch, cocos2d::Event *event)
{
    return true;
}

void AABBQueryScene::onTouchEnded(Touch *touch, Event *event)
{

}

std::string AABBQueryScene::demo_info() const
{
    return "AABB包围盒查询";
}

void AABBQueryScene::restartCallback(Ref *pSender)
{
    Director::getInstance()->replaceScene(AABBQueryScene::createScene());
}