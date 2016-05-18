#ifndef PLAYER_HPP
#define PLAYER_HPP
#include <glm/glm.hpp>
#include <glm/vec3.hpp>


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

private:
	glm::vec3 mPos;
	double mSpeed;

	bool mMoveUp;
	bool mMoveDown;
	bool mMoveLeft;
	bool mMoveRight;
};

#endif // PLAYER_HPP

