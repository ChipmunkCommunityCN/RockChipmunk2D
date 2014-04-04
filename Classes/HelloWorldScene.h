#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "cocos2d.h"
#include <map>
#include "BaseDemo.h"

USING_NS_CC;

class HelloWorld : BaseDemo
{
public:
    static cocos2d::Scene* createScene();
    virtual bool init() override;
    virtual std::string demo_info() const override;
    
    CREATE_FUNC(HelloWorld);
    
    virtual void restartCallback(Ref* pSender);
};

#endif // __HELLOWORLD_SCENE_H__
