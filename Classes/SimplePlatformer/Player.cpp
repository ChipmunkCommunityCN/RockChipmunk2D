#include "Player.h"

USING_NS_CC;
const Vect MOVE_SPEED(10000.0f, 0.0f);
const Vect JUMP_SPEED(0.0f, 20000.0f);
bool Player::init()
{
	if (!Sprite::init())
	{
		return false;
	}
    setTexture("YellowSquare.png");
    
    auto body = PhysicsBody::createBox(Size(50, 50), PhysicsMaterial(0.1f, 1.0f, 0.8f));
    setPhysicsBody(body);

    setScale(0.5f);
	return true;
}

void Player::moveRight()
{
    getPhysicsBody()->applyForce(MOVE_SPEED);
}

void Player::jump()
{
    getPhysicsBody()->applyImpulse(JUMP_SPEED);
}