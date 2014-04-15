//
//  HitMeScene.h
//  RockChipmunk2D
//
//  Created by ChildhoodAndy on 14-4-15.
//
//

#ifndef __RockChipmunk2D__HitMeScene__
#define __RockChipmunk2D__HitMeScene__

#include "cocos2d.h"
#include "BaseDemo.h"
USING_NS_CC;

class HitMeScene : Layer
{
public:
	static Scene* createScene();
	virtual bool init();
    std::string demo_info() const;
    void restartCallback(Ref* pSender);
    void backCallback(Ref* pSender);
    
	void setPhyWorld(PhysicsWorld* world){ _world = world; };
	bool onContactBegin(PhysicsContact& contact);
    
	CREATE_FUNC(HitMeScene);
    void hitMeFire(Ref* sender);
private:
    PhysicsWorld* _world;
    LayerColor* _bgColorLayer;
    MenuItemImage* _hitItem;
    void setPhysicsForHitItem();
};

#endif /* defined(__RockChipmunk2D__HitMeScene__) */
