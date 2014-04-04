//
//  IntroScene.cpp
//  PhysicsTest
//
//  Created by ChildhoodAndy on 14-3-9.
//
//

#include "IntroScene.h"
#include "ContentLayer.h"
#include "SimpleAudioEngine.h"

Scene* IntroScene::createScene()
{
    auto scene = Scene::create();
    auto layer = IntroScene::create();
    scene->addChild(layer);
    return scene;
}

bool IntroScene::init()
{
    if(!Layer::init())
    {
        return false;
    }
    
    Size visibleSize = Director::getInstance()->getVisibleSize();
    auto bg = LayerColor::create(Color4B(255, 255, 255, 255), visibleSize.width, visibleSize.height);
    this->addChild(bg);
    
    auto chipmunk_logo = Sprite::create("chipmunkLogo.png");
    this->addChild(chipmunk_logo);
    chipmunk_logo->setPosition(Point(visibleSize.width / 2, visibleSize.height * 4 / 5));
    
    auto github_logo = Sprite::create("steroidtocat.png");
    this->addChild(github_logo);
    github_logo->setPosition(Point(visibleSize.width / 3, visibleSize.height / 2));
    
    auto chipmunkCN_logo = Sprite::create("ChipmunkCN_logo_200x200.png");
    this->addChild(chipmunkCN_logo);
    chipmunkCN_logo->setPosition(github_logo->getPosition() + Point(200, 20));
    
    auto chipmunkCN_url = LabelTTF::create("https://github.com/ChipmunkCommunityCN/", "Marker Felt", 20);
    this->addChild(chipmunkCN_url);
    chipmunkCN_url->setAnchorPoint(Point(0.0f, 0.5f));
    chipmunkCN_url->setColor(Color3B(155, 5, 5));
    chipmunkCN_url->setPosition(chipmunkCN_logo->getPosition() + Point(-100, -100));
    
    _playMenuItem = MenuItemImage::create("goNormal.png", "goSelected.png", CC_CALLBACK_1(IntroScene::play, this));
    _playMenuItem->setPosition(Point(visibleSize.width * 4 / 5, visibleSize.height / 5));
    auto _playMenu = Menu::create(_playMenuItem, NULL);
    _playMenu->setPosition(Point::ZERO);
    this->addChild(_playMenu);
    
    return true;
}

void IntroScene::onEnter()
{
    Layer::onEnter();
    this->scheduleUpdate();
//    CocosDenshion::SimpleAudioEngine::getInstance()->playBackgroundMusic("tabletop-battles.mp3", true);
}

void IntroScene::onExit()
{
    Layer::onExit();
    this->unscheduleUpdate();
}

void IntroScene::update(float delta)
{
    _counter += delta;
    _playMenuItem->setScaleX((sinf(_counter * 10) + 1) / 2 * 0.1 + 1);
    _playMenuItem->setScaleY((cosf(_counter * 10) + 1) / 2 * 0.1 + 1);
    
    if (_counter > 10 * M_PI)
    {
        _counter = 0;
    }
}

void IntroScene::play(Ref *pSender)
{
    Director::getInstance()->replaceScene(ContentLayer::createScene());
}