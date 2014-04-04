#include "HelloWorldScene.h"
#include "VisibleRect.h"

USING_NS_CC;

Scene* HelloWorld::createScene()
{
    auto scene = Scene::createWithPhysics();
    scene->getPhysicsWorld()->setDebugDrawMask(PhysicsWorld::DEBUGDRAW_ALL);
    scene->getPhysicsWorld()->setGravity(Vect(0, -200));
    auto layer = HelloWorld::create();
    layer->setPhysicWorld(scene->getPhysicsWorld());
    scene->addChild(layer);
    return scene;
}

bool HelloWorld::init()
{
    if ( !BaseDemo::init() )
    {
        return false;
    }
    
    Size visibleSize = Director::getInstance()->getVisibleSize();

    for(int i = 0; i < 10; i++)
    {
        auto box = createBox(Point(CCRANDOM_0_1()*visibleSize.width, 30 + CCRANDOM_0_1()*visibleSize.height), Size(30 + CCRANDOM_0_1() * 50, 100 + CCRANDOM_0_1() * 50));
        this->addChild(box);
    }
    
    return true;
}

std::string HelloWorld::demo_info() const
{
    return "01 构建一个物理世界";
}

void HelloWorld::restartCallback(Ref *pSender)
{
    Director::getInstance()->replaceScene(HelloWorld::createScene());
}
