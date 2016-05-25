#include "Player.hpp"
using namespace std;

#define ANIM_TIME 0.7f

const vector<glm::vec2> animKeys{
	glm::vec2{ 0.0f, 0.5f },
	glm::vec2{ 0.279296875f, 0.5f },
	glm::vec2{ 0.55859375f, 0.5f },
	glm::vec2{ 0.279296875f, 0.5f },
	glm::vec2{ 0.0f, 0.5f },
	glm::vec2{ 0.0f, 0.0f },
	glm::vec2{ 0.279296875f, 0.0f },
	glm::vec2{ 0.0f, 0.0f }
};

Player::Player() : mMoveUp{ false }, mMoveDown{ false }, mMoveLeft{ false },
	mMoveRight{ false }
{
}

void Player::setMoveUp(bool state)
{
	mMoveUp = state;
}

void Player::setMoveDown(bool state)
{
	mMoveDown = state;
}

void Player::setMoveLeft(bool state)
{
	mMoveLeft = state;
}

void Player::setMoveRight(bool state)
{
	mMoveRight = state;
}

void Player::update(double deltaTime)
{
	glm::vec3 move{ 0.0f, 0.0f, 0.0f };
	move.z -= mMoveUp ? 1.0f : 0.0f;
	move.z += mMoveDown ? 1.0f : 0.0f;
	move.x -= mMoveLeft ? 1.0f : 0.0f;
	move.x += mMoveRight ? 1.0f : 0.0f;

	if ((mMoveUp || mMoveDown || mMoveLeft || mMoveRight) &&
		mAnimState < ANIM_TIME)
		mAnimState += deltaTime;
	else
		mAnimState = 0.0f;

	mPos += move * static_cast<float>(mSpeed * deltaTime);

}

void Player::setSpeed(float speed)
{
	mSpeed = speed;
}

void Player::setPos(glm::vec3 pos)
{
	mPos = pos;
}

glm::vec3 Player::getPos()
{
	return mPos;
}

glm::vec2 Player::getAnimKey()
{
	int idx;
	idx = static_cast<int>(animKeys.size() * mAnimState / ANIM_TIME);

	if (idx >= animKeys.size())
		idx -= animKeys.size();

	return animKeys[idx];
}
