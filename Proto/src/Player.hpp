#ifndef PLAYER_HPP
#define PLAYER_HPP
#include <vector>
#include <glm/glm.hpp>
#include <glm/vec3.hpp>
#include <glm/vec2.hpp>


class Player
{
public:
	Player();

	void setMoveUp(bool state);
	void setMoveDown(bool state);
	void setMoveLeft(bool state);
	void setMoveRight(bool state);

	void update(double deltaTime);

	void setSpeed(float speed);
	void setPos(glm::vec3 pos);
	glm::vec3 getPos();
	glm::vec2 getAnimKey();

private:
	glm::vec3 mPos;
	double mSpeed;
	float mAnimState;

	bool mMoveUp;
	bool mMoveDown;
	bool mMoveLeft;
	bool mMoveRight;
};

#endif // PLAYER_HPP

