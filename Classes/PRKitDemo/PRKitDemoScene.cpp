//
//  PRKitDemoScene.cpp
//  RockChipmunk2D
//
//  Created by ChildhoodAndy on 14-4-16.
//
//

#include "PRKitDemoScene.h"
#include "VisibleRect.h"
#include "ContentLayer.h"
#include "PRFilledPolygon.h"
#include "triangulate.h"

Scene* PRKitDemoScene::createScene()
{
    auto scene = Scene::create();
    auto layer = PRKitDemoScene::create();
    scene->addChild(layer);
    return scene;
}

bool PRKitDemoScene::init()
{
    if(!Layer::init())
    {
        return false;
    }
    
    auto color_bg = LayerColor::create(Color4B(125, 125, 125, 125));
    addChild(color_bg);
    
    auto demo_info_label = Label::create(demo_info(), "Helvetica", 25, Size(500, 200), TextHAlignment::LEFT);
    this->addChild(demo_info_label);
    demo_info_label->setAnchorPoint(Point(0.0f, 1.0f));
    demo_info_label->setColor(Color3B(255, 255, 255));
    demo_info_label->setPosition(VisibleRect::leftTop() + Point(60, -60));
    
    auto backMenuItem = MenuItemImage::create("backNormal.png", "backSelected.png", CC_CALLBACK_1(PRKitDemoScene::backCallback, this));
    auto restartMenuItem = MenuItemImage::create("refreshNormal.png", "refreshSelected.png", CC_CALLBACK_1(PRKitDemoScene::restartCallback, this));
    
    restartMenuItem->setPosition(VisibleRect::rightTop() + Point(-200, -80));
    backMenuItem->setPosition(restartMenuItem->getPosition() + Point(90, 0));
    auto menu = Menu::create(backMenuItem, restartMenuItem, NULL);
    menu->setPosition(Point::ZERO);
    this->addChild(menu, 300);
    
    Vector2dVector polygonPoints;
    polygonPoints.push_back(Vector2d(100, 100));
    polygonPoints.push_back(Vector2d(200, 100));
    polygonPoints.push_back(Vector2d(300, 200));
    polygonPoints.push_back(Vector2d(400, 300));
    polygonPoints.push_back(Vector2d(500, 500));

    Texture2D *texture = Director::getInstance()->getTextureCache()->addImage("pattern1.png");
    PRFilledPolygon *filledPolygon = PRFilledPolygon::filledPolygonWithPointsAndTexture(polygonPoints, texture);
    addChild(filledPolygon);
    
    return true;
}

std::string PRKitDemoScene::demo_info() const
{
    return "PRKit使用例子";
}

void PRKitDemoScene::restartCallback(Ref *pSender)
{
    Director::getInstance()->replaceScene(PRKitDemoScene::createScene());
}

void PRKitDemoScene::backCallback(Ref *pSender)
{
    Director::getInstance()->replaceScene(ContentLayer::createScene());
}