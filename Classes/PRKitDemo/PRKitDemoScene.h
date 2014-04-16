//
//  PRKitDemoScene.h
//  RockChipmunk2D
//
//  Created by ChildhoodAndy on 14-4-16.
//
//

#ifndef __RockChipmunk2D__PRKitDemoScene__
#define __RockChipmunk2D__PRKitDemoScene__

#include "cocos2d.h"
USING_NS_CC;

class PRKitDemoScene : Layer
{
public:
    static cocos2d::Scene* createScene();
    virtual bool init();
    CREATE_FUNC(PRKitDemoScene);
    
    std::string demo_info() const;
    void backCallback(Ref* pSender);
    void restartCallback(Ref* pSender);
private:
    PhysicsWorld* _world;
};

#endif /* defined(__RockChipmunk2D__PRKitDemoScene__) */
