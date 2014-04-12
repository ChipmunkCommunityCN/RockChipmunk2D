#include "cocos2d.h"
#include "Player.h"
#include "BaseDemo.h"

USING_NS_CC;

class SimplePlatformerScene : BaseDemo
{
    
public:
    enum class GameObjectType{
        kGameObjectNone,
        kGameObjectPlayer,
        kGameObjectPlatform
    };
    
public:
	static cocos2d::Scene* createScene();
	virtual bool init();
	virtual void onEnter();
    virtual std::string demo_info() const override;
    void restartCallback(Ref* pSender);
    
	void setPhyWorld(PhysicsWorld* world){ _world = world; };
	void makeBoxObjAt(Sprite* tile, Size size, bool dynamic, const PhysicsMaterial& material);
	void drawCollisionTiles();
	void addScrollingBackgroundWithTileMap();
	virtual bool onTouchBegan(Touch *touch, Event *event);
    virtual void onTouchMoved(Touch* touch, Event* event);
	bool onContactBegin(PhysicsContact& contact);

	CREATE_FUNC(SimplePlatformerScene);
private:
    PhysicsWorld* _world;
    TMXTiledMap* _tileMapNode;
    Player* _player;
    Size _size;
    Node* _bodyNode;
};