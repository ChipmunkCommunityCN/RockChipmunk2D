//
//  PointQueryScene.cpp
//  RockChipmunk2D
//
//  Created by ChildhoodAndy on 14-4-13.
//
//

#include "PointQueryScene.h"
#include "VisibleRect.h"

USING_NS_CC;

const int ROTATE_LINE_LENGTH = 400;
static const Color4F LINE_COLOR = Color4F(0.0f, 1.0f, 1.0f, 1.0f);
static const Color4F DOT_COLOR = Color4F(0.0f, 1.0f, 0.0f, 1.0f);
Scene* PointQueryScene::createScene()
{
    auto scene = Scene::createWithPhysics();
    scene->getPhysicsWorld()->setDebugDrawMask(PhysicsWorld::DEBUGDRAW_ALL);
    // 此demo不需要重力，建构一个失重世界
    scene->getPhysicsWorld()->setGravity(Point::ZERO);
    
    auto layer = PointQueryScene::create();
    scene->addChild(layer);
    layer->setPhysicWorld(scene->getPhysicsWorld());
    return scene;
}

bool PointQueryScene::init()
{
    if (!BaseDemo::init())
    {
        return false;
    }
    
    _canvas = nullptr;
    _center = VisibleRect::center();
    _roateAngle = 0.0f;
    
    MenuItemFont::setFontSize(25);
    auto item = MenuItemFont::create("切换模式：任意", CC_CALLBACK_1(PointQueryScene::changeModeCallback, this));
    
    auto menu = Menu::create(item, NULL);
    this->addChild(menu);
    menu->setPosition(Point(VisibleRect::left().x + 100, VisibleRect::top().y - 10));
    
    _bodyInfo = Label::create("", "Helvetica", 25, Size(500, 200), TextHAlignment::LEFT);
    this->addChild(_bodyInfo, 1000);
    _bodyInfo->setAnchorPoint(Point::ANCHOR_BOTTOM_LEFT);
    _bodyInfo->setColor(Color3B(255, 255, 255));
    _bodyInfo->setPosition(VisibleRect::leftBottom() + Point(60, 60));
    
    scheduleUpdate();
    
    return true;
}

void PointQueryScene::changeModeCallback(Ref* sender)
{
    _mode = (_mode + 1) % 3;
    
    switch (_mode)
    {
        case 0:
            ((MenuItemFont*)sender)->setString("切换模式：任意");
            break;
        case 1:
            ((MenuItemFont*)sender)->setString("切换模式：最近点");
            break;
        case 2:
            ((MenuItemFont*)sender)->setString("切换模式：多个点");
            break;
        default:
            break;
    }
}

void PointQueryScene::update(float dt)
{
    removeChild(_canvas);
    _bodyInfo->setString("");
    _canvas = DrawNode::create();
    
    Point d(ROTATE_LINE_LENGTH * cosf(_roateAngle), ROTATE_LINE_LENGTH * sinf(_roateAngle));
    Point point2 = _center + d;
    
    switch (_mode)
    {
        case 0:
        {
            Point point3 = point2;
            PhysicsRayCastCallbackFunc func = [this](PhysicsWorld& world, const PhysicsRayCastInfo& info, void* data)->bool
            {
                *((Point*)data) = info.contact;
                
                Point pos = info.shape->getBody()->getNode()->getPosition();
                char labelStr[30];
                sprintf(labelStr, "位置: %.1f, %.1f", pos.x, pos.y);
                _bodyInfo->setString(labelStr);
                
                return false;
            };
            
            _world->rayCast(func, _center, point2, &point3);
            _canvas->drawSegment(_center, point3, 1, LINE_COLOR);
            
            if (point2 != point3)
            {
                _canvas->drawDot(point3, 5, DOT_COLOR);
            }
            addChild(_canvas);
            
            break;
        }
        case 1:
        {
            Point point3 = point2;
            float fraction = 1.0f;
            PhysicsRayCastCallbackFunc func = [&point3, &fraction](PhysicsWorld& world, const PhysicsRayCastInfo& info, void* data)->bool
            {
                if (fraction > info.fraction)
                {
                    point3 = info.contact;
                    fraction = info.fraction;
                }
                
                return true;
            };
            
            _world->rayCast(func, _center, point2, nullptr);
            _canvas->drawSegment(_center, point3, 1, LINE_COLOR);
            
            if (point2 != point3)
            {
                _canvas->drawDot(point3, 5, DOT_COLOR);
            }
            addChild(_canvas);
            
            break;
        }
        case 2:
        {
#define MAX_MULTI_RAYCAST_NUM 5
            Point points[MAX_MULTI_RAYCAST_NUM];
            int num = 0;
            
            PhysicsRayCastCallbackFunc func = [&points, &num](PhysicsWorld& world, const PhysicsRayCastInfo& info, void* data)->bool
            {
                if (num < MAX_MULTI_RAYCAST_NUM)
                {
                    points[num++] = info.contact;
                }
                
                return true;
            };
            
            _world->rayCast(func, _center, point2, nullptr);
            _canvas->drawSegment(_center, point2, 1, LINE_COLOR);
            
            for (int i = 0; i < num; ++i)
            {
                _canvas->drawDot(points[i], 5, DOT_COLOR);
            }
            
            addChild(_canvas);
            
            break;
        }
            
        default:
            break;
    }
    
    _roateAngle += CC_DEGREES_TO_RADIANS(0.30f);
}

bool PointQueryScene::onTouchBegan(cocos2d::Touch *touch, cocos2d::Event *event)
{
    return true;
}

///*
void PointQueryScene::onTouchEnded(Touch *touch, Event *event)
{
    auto location = touch->getLocation();
    auto p = CCRANDOM_0_1();
    if (p < 1.0f / 3.0f)
    {
        addChild(createBall(location, CCRANDOM_0_1() * 10 + 30));
    }
    else if (p < 2.0f / 3.0f)
    {
        addChild(createBox(location, Size(CCRANDOM_0_1() * 20 + 30, CCRANDOM_0_1() * 20 + 30)));
    }
    else
    {
        addChild(createTriangle(location, Size(CCRANDOM_0_1() * 20 + 30, CCRANDOM_0_1() * 20 + 30)));
    }
}
// */

std::string PointQueryScene::demo_info() const
{
    return "点查询";
}

void PointQueryScene::restartCallback(Ref *pSender)
{
    Director::getInstance()->replaceScene(PointQueryScene::createScene());
}