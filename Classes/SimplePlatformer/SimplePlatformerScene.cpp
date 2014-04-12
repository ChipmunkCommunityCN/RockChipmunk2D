#include "SimplePlatformerScene.h"
#include "ContentLayer.h"
#include "VisibleRect.h"

Scene* SimplePlatformerScene::createScene()
{
	auto scene = Scene::createWithPhysics();
	scene->getPhysicsWorld()->setDebugDrawMask(PhysicsWorld::DEBUGDRAW_ALL);
	scene->getPhysicsWorld()->setGravity(Vect(0, -200));

	auto layer = SimplePlatformerScene::create();
	layer->setPhyWorld(scene->getPhysicsWorld());
	scene->addChild(layer);

	return scene;
}

bool SimplePlatformerScene::init()
{
	if(!BaseDemo::init())
	{
		return false;
	}
	
    _size = VisibleRect::getVisibleRect().size;
    
    _bodyNode = Node::create();
    this->addChild(_bodyNode);
    _bodyNode->setPosition(100, 100);
    
	this->addScrollingBackgroundWithTileMap();
	this->drawCollisionTiles();
    
    _player = Player::create();
	_player->setPosition(100.0f, 300.0f);
	_tileMapNode->addChild(_player);
    
	return true;
}

void SimplePlatformerScene::makeBoxObjAt(Sprite* tile, Size size, bool dynamic, const PhysicsMaterial& material)
{
    auto body = PhysicsBody::createEdgeBox(size, material);
    tile->setAnchorPoint(Point::ANCHOR_MIDDLE);
	tile->setPhysicsBody(body);
}

void SimplePlatformerScene::drawCollisionTiles()
{
    TMXLayer* collisionLayer = _tileMapNode->getLayer("edgeLayer");
    auto mapSize = _tileMapNode->getMapSize();
    auto tileSize = _tileMapNode->getTileSize();
    Sprite* tile = nullptr;
    Point pos;
    for(int i = 0; i < mapSize.width; i++)
    {
        for(int j = 0; j < mapSize.height; j++)
        {
            tile = collisionLayer->getTileAt(Point(i, j));
            if(tile != nullptr)
            {
                const ValueMap property = _tileMapNode->getPropertiesForGID(collisionLayer->getTileGIDAt(Point(i, j))).asValueMap();
                bool collidable = property.at("collidable").asBool();
                if(collidable)
                {
                    pos = collisionLayer->getPositionAt(Point(i, j));
                    makeBoxObjAt(tile, tileSize, false, PhysicsMaterial(0.2f, 0.5f, 0.5f));
                }
            }
        }
    }
}

void SimplePlatformerScene::addScrollingBackgroundWithTileMap()
{
	_tileMapNode = TMXTiledMap::create("scroller.tmx");
	_tileMapNode->setAnchorPoint(Point(0, 0));
    _tileMapNode->setPosition(Point(_tileMapNode->getTileSize().width/2, _tileMapNode->getTileSize().height/2));
	this->addChild(_tileMapNode);
}

void SimplePlatformerScene::onEnter()
{
	BaseDemo::onEnter();

	auto contactListener = EventListenerPhysicsContact::create();
	contactListener->onContactBegin = CC_CALLBACK_1(SimplePlatformerScene::onContactBegin, this);

	getEventDispatcher()->addEventListenerWithSceneGraphPriority(contactListener, this);
}

bool SimplePlatformerScene::onContactBegin(PhysicsContact& contact)
{
	return true;
}

bool SimplePlatformerScene::onTouchBegan(Touch *touch, Event *event)
{
    Point location = touch->getLocation();
	if (location.x >= _size.width / 2)
	{
		_player->moveRight();
	}
	else
	{
		_player->jump();
	}
	return true;
}

void SimplePlatformerScene::onTouchMoved(Touch* touch, Event* event)
{

}

std::string SimplePlatformerScene::demo_info() const
{
    return "简单的平台游戏\n 点击屏幕左半边 物体跳跃\n 点击屏幕右半边 物体向右移动";
}


void SimplePlatformerScene::restartCallback(Ref *pSender)
{
    Director::getInstance()->replaceScene(SimplePlatformerScene::createScene());
}