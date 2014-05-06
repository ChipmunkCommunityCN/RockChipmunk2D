//
//  PhysicsLineDrawTestScene.h
//  RockChipmunk2D
//
//  Created by ChildhoodAndy on 14-5-6.
//
//

#ifndef __RockChipmunk2D__PhysicsLineDrawTestScene__
#define __RockChipmunk2D__PhysicsLineDrawTestScene__

#include "cocos2d.h"
#include "BaseDemo.h"

typedef struct Segment
{
    Point p1;
    Point p2;
}Segment;

class PhysicsLineDrawTestScene : BaseDemo
{
public:
    static Scene* createScene();
    
    virtual bool init() override;
    virtual std::string demo_info() const;
    CREATE_FUNC(PhysicsLineDrawTestScene);
    
    virtual void restartCallback(Ref* pSender);
    
    virtual bool onTouchBegan(Touch* touch, Event* event);
    virtual void onTouchMoved(Touch* touch, Event* event);
    virtual void onTouchEnded(Touch* touch, Event* event);
    virtual void draw(Renderer *renderer, const kmMat4& transform, bool transformUpdated);
    
    bool onContactBegin(PhysicsContact& contact);
    bool onContactPreSolve(PhysicsContact& contact, PhysicsContactPreSolve& solve);
    void onContactPostSolve(PhysicsContact& contact, const PhysicsContactPostSolve& solve);
    void onContactSeperate(PhysicsContact& contact);
private:
    Point _prePoint;
    Point _curPoint;
    Segment _segment;
    std::vector<Segment> _segments;
    void checkCurPoint(Point& point);
    void fallBalls(float dt);
};

#endif /* defined(__RockChipmunk2D__PhysicsLineDrawTestScene__) */
