#ifndef ENEMY_HPP
#define ENEMY_HPP
#include <glm/glm.hpp>
#include <glm/vec3.hpp>

class Enemy
{
public:
	Enemy();

	void setMoveDir(glm::vec3 moveDir);

	void update(double deltaTime);

	void setSpeed(float speed);
	void setStartPos(glm::vec3 pos);
	glm::vec3 getPos();
	glm::vec3 getStartPos();

private:
	glm::vec3 mStartPos;
	glm::vec3 mPos;
	double mSpeed;

	glm::vec3 mMoveDir;
};

#endif // ENEMY_HPP

