#pragma once
#include "make_Shader.h"
#include "cuboid.h"
#include "mountain.h"

class move_obj
{
private:
	GLuint vao;
	GLuint vbo[2];
		
	std::vector<GLfloat> vertex;
	std::vector<GLfloat> color;
	glm::mat4 transformation;

	glm::vec3 pos;
	glm::vec3 oldPos;
	GLfloat speed;
	GLboolean state;

	glm::vec3 look;
	glm::vec2 direction;

	glm::mat4 camera;
	glm::vec3 camera_eye;


public:
	move_obj()
	{
		pos = glm::vec3(-500.0f + mountain::width / 2, 10.0f, -500.0f + mountain::length / 2);
		oldPos = pos;
		speed = 7.0f;
		state = false;
		direction = glm::vec2(0.0f, 0.0f);
		look = glm::vec3(0.0f, 0.0f, 1.0f);

		vertex = std::vector<GLfloat>(108);
		color = std::vector<GLfloat>(108);
		makeCuboid(vertex, mountain::width / 4, mountain::length / 4, 10.0f);
		setCol(color, 0.3f, 0.5f, 0.8f);

		camera_eye = glm::vec3(pos.x, pos.y, pos.z + mountain::width / 4);

		glGenVertexArrays(1, &vao);
		glGenBuffers(2, vbo);

		glBindVertexArray(vao);

		glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
		glBufferData(GL_ARRAY_BUFFER, color.size() * sizeof(GLfloat), color.data(), GL_STATIC_DRAW);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray(1);

		glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
		glBufferData(GL_ARRAY_BUFFER, vertex.size() * sizeof(GLfloat), vertex.data(), GL_STATIC_DRAW);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray(0);

		transformation = glm::mat4(1.0f);
		transformation = glm::translate(transformation, pos);


	}
	GLvoid setDirection(int key, GLboolean down);
	GLvoid set_speed(const GLfloat& delta);

	GLboolean get_state() const { return state; }
	GLfloat get_speed() const { return speed; }
	glm::vec3 get_look() const { return look; }
	glm::mat4 get_camera() const { return camera; }

	GLvoid draw(unsigned int& modelLocation);
	GLvoid reveal();
	GLvoid move();
};

GLvoid move_obj::setDirection(int key, GLboolean down)
{
	if (down)
	{
		if (key == GLUT_KEY_LEFT)
		{
			direction.x -= 1;
		}
		else if (key == GLUT_KEY_RIGHT)
		{
			direction.x += 1;
		}
		else if (key == GLUT_KEY_UP)
		{
			direction.y -= 1;
		}
		else if (key == GLUT_KEY_DOWN)
		{
			direction.y += 1;
		}
	}
	else
	{
		if (key == GLUT_KEY_LEFT)
		{
			direction.x += 1;
		}
		else if (key == GLUT_KEY_RIGHT)
		{
			direction.x -= 1;
		}
		else if (key == GLUT_KEY_UP)
		{
			direction.y += 1;
		}
		else if (key == GLUT_KEY_DOWN)
		{
			direction.y -= 1;
		}
	}
}
GLvoid move_obj::set_speed(const GLfloat& delta)
{
	speed += delta;
}


GLvoid move_obj::draw(unsigned int& modelLocation)
{
	glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(transformation));
	glBindVertexArray(vao);
	glDrawArrays(GL_TRIANGLES, 0, vertex.size() / 3);
}


GLvoid move_obj::reveal()
{
	state = state ? false : true;
}

GLvoid move_obj::move()
{
	oldPos = pos;
	pos.x += direction.x * speed;
	pos.z += direction.y * speed;
	camera_eye = glm::vec3(pos.x, pos.y+ 20.0f, pos.z + mountain::width / 4);

	glm::vec3 temp(0.0f, 0.0f, 1.0f);
	if (oldPos.x != pos.x || oldPos.z != pos.z)
	{	
		look.x = pos.x - oldPos.x;
		look.z = pos.z - oldPos.z;
	}
	look = glm::normalize(look);
	GLfloat cos_rotate = glm::dot(glm::vec3(0.0f, 0.0f, 1.0f), look);

	transformation = glm::mat4(1.0f);
	transformation = glm::translate(transformation, pos);
	transformation = glm::rotate(transformation, glm::acos(cos_rotate), glm::vec3(0.0f, 1.0f, 0.0f));

	camera = glm::mat4(1.0f);
	camera = glm::lookAt(camera_eye, camera_eye + look, glm::vec3(0.0f, 1.0f, 0.0f));
}
