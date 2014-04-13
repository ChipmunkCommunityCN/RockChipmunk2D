//
//  ContentLayer.cpp
//  PhysicsTest
//
//  Created by ChildhoodAndy on 14-3-9.
//
//

#include "ContentLayer.h"
#include "HelloWorldScene.h"
#include "PinJointScene.h"
#include "SlideJointScene.h"
#include "ContactFilterScene.h"
#include "ColorMatchScene.h"
#include "SimplePlatformerScene.h"
#include "PointQueryScene.h"

struct DEMO
{
    const std::string demo_name;
    std::function<Scene*()> callback;
}demos[] =
{
    { "01 构建一个物理世界", [](){ return HelloWorld::createScene(); } },
    { "02 构建一个物理世界2", [](){ return HelloWorld::createScene(); } },
    { "03 了解PinJoint【销关节】", [](){ return PinJoint::createScene(); } },
    { "04 了解SlideJoint【滑动关节】", [](){ return SlideJoint::createScene(); } },
    { "05 了解碰撞过滤", [](){ return ContactFilterScene::createScene(); } },
    { "06 简单的小游戏—ColorMatch", [](){ return ColorMatchScene::createScene(); } },
    { "07 简单的小游戏—SimplePlatformer", [](){ return SimplePlatformerScene::createScene(); } },
    { "08 点查询", [](){ return PointQueryScene::createScene(); } },
};

Scene* ContentLayer::createScene()
{
    auto scene = Scene::create();
    auto layer = ContentLayer::create();
    scene->addChild(layer);
    return scene;
}

bool ContentLayer::init()
{
    if (!Layer::init())
    {
        return false;
    }
    
    Size visibleSize = Director::getInstance()->getVisibleSize();
    auto bg = LayerColor::create(Color4B(255, 255, 255, 255), visibleSize.width, visibleSize.height);
    this->addChild(bg);
    
    TableView* tableView = TableView::create(this, Size(500, 500));
    tableView->setDirection(ScrollView::Direction::VERTICAL);
    tableView->setVerticalFillOrder(TableView::VerticalFillOrder::TOP_DOWN);
    tableView->setPosition(Point(visibleSize.width/6, visibleSize.height/6));
    tableView->setDelegate(this);
    this->addChild(tableView);
    tableView->reloadData();
    
    return true;
}

void ContentLayer::tableCellTouched(TableView* table, TableViewCell* cell)
{
    CCLOG("cell touched at index: %ld", cell->getIdx());
    auto scene = demos[cell->getIdx()].callback();
    Director::getInstance()->replaceScene(scene);
}

Size ContentLayer::tableCellSizeForIndex(TableView *table, ssize_t idx)
{
    return Size(60, 60);
}

void ContentLayer::tableCellHighlight(TableView* table, TableViewCell* cell)
{
    auto itemBg = static_cast<Sprite*>(cell->getChildByTag(100));
    itemBg->setTexture(Director::getInstance()->getTextureCache()->addImage("scrollItemBgSelected.png"));
}

void ContentLayer::tableCellUnhighlight(TableView* table, TableViewCell* cell)
{
    auto itemBg = static_cast<Sprite*>(cell->getChildByTag(100));
    itemBg->setTexture(Director::getInstance()->getTextureCache()->addImage("scrollItemBgNormal.png"));
}

TableViewCell* ContentLayer::tableCellAtIndex(TableView *table, ssize_t idx)
{
    auto demo_name = demos[idx].demo_name;
    TableViewCell *cell = table->dequeueCell();
    if (!cell) {
        cell = new TableViewCell();
        cell->autorelease();
        auto sprite = Sprite::create("scrollItemBgNormal.png");
        sprite->setAnchorPoint(Point::ZERO);
        sprite->setPosition(Point(0, 0));
        sprite->setTag(100);
        cell->addChild(sprite);
        
        auto label = Label::create(demo_name, "Helvetica", 20.0);
        label->setPosition(Point::ZERO + Point(10, 10));
        label->setColor(Color3B(0, 0, 0));
		label->setAnchorPoint(Point::ZERO);
        label->setTag(200);
        cell->addChild(label);
    }
    else
    {
        auto label = (Label*)cell->getChildByTag(200);
        label->setString(demo_name);
    }
    
    
    return cell;
}

ssize_t ContentLayer::numberOfCellsInTableView(TableView *table)
{
    return sizeof(demos) / sizeof(demos[0]);
}
