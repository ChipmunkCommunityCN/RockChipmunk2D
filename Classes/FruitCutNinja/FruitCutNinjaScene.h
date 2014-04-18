//
//  FruitCutNinjaScene.h
//  RockChipmunk2D
//
//  Created by ChildhoodAndy on 14-4-16.
//
//

#ifndef __RockChipmunk2D__FruitCutNinjaScene__
#define __RockChipmunk2D__FruitCutNinjaScene__

#include "cocos2d.h"
#include "CCBlade.h"
USING_NS_CC;

class FruitCutNinjaScene : Layer
{
public:
    static Scene* createScene();
    virtual bool init() override;
    
    void setPhyWorld(PhysicsWorld* world){ _world = world; };
    
    virtual void update(float dt);
    void restartCallback(Ref* pSender);
    void backCallback(Ref* pSender);
    bool slice(PhysicsWorld& world, const PhysicsRayCastInfo& info, void* data);
    void clipPoly(PhysicsShapePolygon* shape, Point normal, float distance);
    
    void onTouchesBegan(const std::vector<Touch*>& touches, Event* event);
    void onTouchesMoved(const std::vector<Touch*>& touches, Event* event);
    void onTouchesEnded(const std::vector<Touch*>& touches, Event* event);
    CREATE_FUNC(FruitCutNinjaScene);
private:
    PhysicsWorld* _world;
    Sprite* _bg;
    Blade* _blade;
    Vector<Blade*> _blades;
    Point _startPoint;
    Point _endPoint;
    Size _visibleSize;
    float _deltaRemainder;
    int _sliceTag;
    
    void createBackground();
};

#endif /* defined(__RockChipmunk2D__FruitCutNinjaScene__) */
