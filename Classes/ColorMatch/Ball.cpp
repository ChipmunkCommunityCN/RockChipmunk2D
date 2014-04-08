//
//  Ball.cpp
//  RockChipmunk2D
//
//  Created by ChildhoodAndy on 14-4-7.
//
//

#include "Ball.h"
#include "CCPhysicsBody.h"

USING_NS_CC;
Ball* Ball::createBall()
{
    Ball* ball = new Ball();
    if (ball && ball->init())
    {
        ball->autorelease();
        return ball;
    }
    
    CC_SAFE_DELETE(ball);
    return nullptr;
}


bool Ball::init()
{
    if(Sprite::init())
    {
        _ballColor = arc4random() % 6;
        _radius = arc4random() % 10 + 30;
        
        PhysicsBody* ball_body = PhysicsBody::createCircle(_radius, PhysicsMaterial(1, 0.5, 0.2));
        ball_body->setCategoryBitmask(0x0001);
        ball_body->setContactTestBitmask(0x0001);
        ball_body->setTag(_ballColor);
        
        linkCountLabel = Label::create("", "Arial", 40);
        linkCountLabel->setColor(Color3B(255, 0, 0));
        linkCountLabel->setPosition(Point(_radius, _radius));
        addChild(linkCountLabel);
        
        char resName[11];
        sprintf(resName, "ball_%d.png", _ballColor);
        setTexture(resName);
        setPhysicsBody(ball_body);
        setScale(_radius / 62.0f);
        
        setLinkCount(1);
        setRoot(this);
        return true;
    }
    
    return false;
}

Ball* Ball::getRoot()
{
    if (_father != this) {
        _father = _father->getRoot();
    }
    
    return _father;
}

void Ball::setRoot(Ball* root)
{
    if (root == this) {
        _father = this;
    }
    else
    {
        _father = root->getRoot();
    }
}
