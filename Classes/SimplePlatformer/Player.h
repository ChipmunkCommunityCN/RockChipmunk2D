#include "cocos2d.h"



class Player : public cocos2d::Sprite
{
public:
    virtual bool init();

	void jump();
	void moveRight();

    CREATE_FUNC(Player);
};

