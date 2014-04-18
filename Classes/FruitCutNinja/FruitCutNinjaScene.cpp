//
//  FruitCutNinjaScene.cpp
//  RockChipmunk2D
//
//  Created by ChildhoodAndy on 14-4-16.
//
//

#include "FruitCutNinjaScene.h"
#include "VisibleRect.h"
#include "ContentLayer.h"

Scene* FruitCutNinjaScene::createScene()
{
	auto scene = Scene::createWithPhysics();
	scene->getPhysicsWorld()->setDebugDrawMask(PhysicsWorld::DEBUGDRAW_ALL);
	scene->getPhysicsWorld()->setGravity(Point(0, -100));
    
	auto layer = FruitCutNinjaScene::create();
	layer->setPhyWorld(scene->getPhysicsWorld());
	scene->addChild(layer);
	return scene;
}

bool FruitCutNinjaScene::init()
{
    if (!Layer::init())
    {
        return false;
    }
    
    _visibleSize = VisibleRect::getVisibleRect().size;
    _deltaRemainder = 0.0f;
    _sliceTag = 1;
    
//    createBackground();
    //加入封闭的盒子，用作墙壁
    auto body = PhysicsBody::createEdgeBox(_visibleSize, PHYSICSBODY_MATERIAL_DEFAULT, 3);
    auto wallNode = Node::create();
    wallNode->setPosition(_visibleSize.width/2, _visibleSize.height/2);
    wallNode->setPhysicsBody(body);
    this->addChild(wallNode);
    
    auto backMenuItem = MenuItemImage::create("backNormal.png", "backSelected.png", CC_CALLBACK_1(FruitCutNinjaScene::backCallback, this));
    auto restartMenuItem = MenuItemImage::create("refreshNormal.png", "refreshSelected.png", CC_CALLBACK_1(FruitCutNinjaScene::restartCallback, this));
    
    restartMenuItem->setPosition(VisibleRect::rightTop() + Point(-200, -80));
    backMenuItem->setPosition(restartMenuItem->getPosition() + Point(90, 0));
    auto menu = Menu::create(backMenuItem, restartMenuItem, NULL);
    menu->setPosition(Point::ZERO);
    this->addChild(menu, 300);
    
    auto touchListener = EventListenerTouchAllAtOnce::create();
    touchListener->onTouchesBegan = CC_CALLBACK_2(FruitCutNinjaScene::onTouchesBegan, this);
    touchListener->onTouchesMoved = CC_CALLBACK_2(FruitCutNinjaScene::onTouchesMoved, this);
    touchListener->onTouchesEnded = CC_CALLBACK_2(FruitCutNinjaScene::onTouchesEnded, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(touchListener, this);
    
    auto texture = Director::getInstance()->getTextureCache()->addImage("streak.png");
    
    for (int i = 0; i < 3; i++)
    {
        Blade* blade = Blade::createWithMaximumPoint(50);
        blade->setAutoDim(false);
        blade->setTexture(texture);
        
        addChild(blade, 100);
        _blades.pushBack(blade);
    }
    
    auto box = Node::create();
    Point points[4] = {Point(-100, -100), Point(-100, 100), Point(100, 100), Point(100, -100)};
    box->setPhysicsBody(PhysicsBody::createPolygon(points, 4));
    box->setPosition(VisibleRect::center());
    box->getPhysicsBody()->setTag(_sliceTag);
    addChild(box);

    //本DEMO要做成一个切水果原型，尚未完成:)
    scheduleUpdate();
    return true;
}

void FruitCutNinjaScene::createBackground()
{
    _bg = Sprite::create("scene_bg.png");
    addChild(_bg, 0);
    _bg->setPosition(VisibleRect::center());
}

void FruitCutNinjaScene::update(float dt)
{
    if (_blade && _blade->getPath().size() > 3)
    {
        _deltaRemainder += dt * 60 * 1.2f;
        int pop = (int)roundf(_deltaRemainder);
        _deltaRemainder -= pop;
        _blade->pop(pop);
    }
}

void FruitCutNinjaScene::onTouchesBegan(const std::vector<Touch*>& touches, Event* event)
{
    for (Touch* touch : touches)
    {
        Point location = touch->getLocation();
        
        _startPoint = location;
        _endPoint = location;
        
        for (Blade* blade : _blades)
        {
            if (blade->getPath().size() == 0)
            {
                _blade = blade;
                _blade->push(location);
                break;
            }
        }
    }
}

void FruitCutNinjaScene::onTouchesMoved(const std::vector<Touch*>& touches, Event *event)
{
    for (Touch* touch : touches)
    {
        Point location = touch->getLocation();
        _endPoint = location;
        
        if ((_startPoint - _endPoint).getLength() > 25)
        {
            _startPoint = _endPoint;
        }
        
        
        _blade->push(location);
    }
}

void FruitCutNinjaScene::onTouchesEnded(const std::vector<Touch*>& touches, Event *event)
{
    for (Touch* touch : touches)
    {
        _blade->setDim(true);
        auto func = CC_CALLBACK_3(FruitCutNinjaScene::slice, this);
        _world->rayCast(func, touch->getStartLocation(), touch->getLocation(), nullptr);
    }
    
}

bool FruitCutNinjaScene::slice(PhysicsWorld &world, const PhysicsRayCastInfo& info, void *data)
{
    if (info.shape->getBody()->getTag() != _sliceTag)
    {
        return true;
    }
    
    if (!info.shape->containsPoint(info.start) && !info.shape->containsPoint(info.end))
    {
        Point normal = info.end - info.start;
        normal = normal.getPerp().normalize();
        float dist = info.start.dot(normal);
        
        clipPoly(dynamic_cast<PhysicsShapePolygon*>(info.shape), normal, dist);
        clipPoly(dynamic_cast<PhysicsShapePolygon*>(info.shape), -normal, -dist);
        
        info.shape->getBody()->removeFromWorld();
    }
    
    return true;
}

void FruitCutNinjaScene::clipPoly(PhysicsShapePolygon* shape, Point normal, float distance)
{
    PhysicsBody* body = shape->getBody();
    int count = shape->getPointsCount();
    int pointsCount = 0;
    Point* points = new Point[count + 1];
    
    for (int i=0, j=count-1; i<count; j=i, ++i)
    {
        Point a = body->local2World(shape->getPoint(j));
        float aDist = a.dot(normal) - distance;
        
        if (aDist < 0.0f)
        {
            points[pointsCount] = a;
            ++pointsCount;
        }
        
        Point b = body->local2World(shape->getPoint(i));
        float bDist = b.dot(normal) - distance;
        
        if (aDist*bDist < 0.0f)
        {
            float t = std::fabs(aDist)/(std::fabs(aDist) + std::fabs(bDist));
            points[pointsCount] = a.lerp(b, t);
            ++pointsCount;
        }
    }
    
    Point center = PhysicsShape::getPolyonCenter(points, pointsCount);
    Node* node = Node::create();
    PhysicsBody* polyon = PhysicsBody::createPolygon(points, pointsCount, PHYSICSBODY_MATERIAL_DEFAULT, -center);
    node->setPosition(center);
    node->setPhysicsBody(polyon);
    polyon->setVelocity(body->getVelocityAtWorldPoint(center));
    polyon->setAngularVelocity(body->getAngularVelocity());
    polyon->setTag(_sliceTag);
    addChild(node);
    
    delete[] points;
}

void FruitCutNinjaScene::restartCallback(Ref *pSender)
{
    Director::getInstance()->replaceScene(FruitCutNinjaScene::createScene());
}

void FruitCutNinjaScene::backCallback(Ref *pSender)
{
    Director::getInstance()->replaceScene(ContentLayer::createScene());
}

