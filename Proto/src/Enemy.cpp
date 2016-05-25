#include "Enemy.hpp"

Enemy::Enemy()
{
}

void Enemy::setMoveDir(glm::vec3 moveDir)
{
	mMoveDir = moveDir;
}

void Enemy::update(double deltaTime)
{
	mPos += mMoveDir * static_cast<float>(mSpeed * deltaTime);
}

void Enemy::setSpeed(float speed)
{
	mSpeed = speed;
}

void Enemy::setStartPos(glm::vec3 pos)
{
	mPos = pos;
	mStartPos = pos;
}

glm::vec3 Enemy::getPos()
{
	return mPos;
}

glm::vec3 Enemy::getStartPos()
{
	return mStartPos;
}
