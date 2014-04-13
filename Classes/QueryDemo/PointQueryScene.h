//
//  PointQueryScene.h
//  RockChipmunk2D
//
//  Created by ChildhoodAndy on 14-4-13.
//
//

#ifndef __RockChipmunk2D__PointQueryScene__
#define __RockChipmunk2D__PointQueryScene__

#include "cocos2d.h"
#include "BaseDemo.h"
USING_NS_CC;
class PointQueryScene : BaseDemo
{
public:
    static Scene* createScene();
    virtual bool init() override;
    virtual std::string demo_info() const override;
    
    virtual void update(float dt) override;
    virtual void restartCallback(Ref* pSender);
    virtual bool onTouchBegan(Touch* touch, Event* event) override;
    virtual void onTouchEnded(Touch* touch, Event* event) override;
    
    void changeModeCallback(Ref* sender);
    CREATE_FUNC(PointQueryScene);
private:
    DrawNode* _canvas;
    float _roateAngle;
    Point _center;
    int _mode;
    Label* _bodyInfo;
};

#endif /* defined(__RockChipmunk2D__PointQueryScene__) */
