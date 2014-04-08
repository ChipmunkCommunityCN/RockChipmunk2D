//
//  Ball.h
//  RockChipmunk2D
//
//  Created by ChildhoodAndy on 14-4-7.
//
//

#ifndef __RockChipmunk2D__Ball__
#define __RockChipmunk2D__Ball__

#include "cocos2d.h"

class Ball : public cocos2d::Sprite
{
public:
    static Ball* createBall();
    virtual bool init();
    CC_SYNTHESIZE(int, _ballColor, BallColor);
    CC_SYNTHESIZE(int, _radius, Radius);

    void setRoot(Ball* root);
    Ball* getRoot();
    
    Ball* getFather();
    void setFather(Ball* father);
    CC_SYNTHESIZE(int, _linkCount, LinkCount);
    
    cocos2d::Label* linkCountLabel;
private:
    Ball* _father;
    Ball* _root;
};

#endif /* defined(__RockChipmunk2D__Ball__) */
