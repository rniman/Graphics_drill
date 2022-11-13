#pragma once
#include "make_Shader.h"
#include "cuboid.h"
//#include <random>
//
//std::random_device rd;
//std::mt19937 gen(rd);

class mountain
{
private:
	GLuint vao;
	GLuint vbo[2];

	std::vector<GLfloat> vertex = std::vector<GLfloat>(108);
	std::vector<GLfloat> color = std::vector<GLfloat>(108);;
	glm::mat4 transformation;
	
	GLfloat index_r;
	GLfloat index_c;
	GLfloat max_height;
	GLfloat speed;
	GLfloat now_height;
	GLboolean state_up;

public:
	static GLfloat width;
	static GLfloat length;
	static GLboolean initAni;


	mountain(const GLint& r, const GLint& c)
	{
		std::uniform_int_distribution<int> dis(50, 300);
		std::uniform_int_distribution<int> dis_speed(1, 10);
		max_height = static_cast<GLfloat>(dis(gen));
		speed = static_cast<GLfloat>(dis_speed(gen)) * 0.01;
		now_height = 0.0f;
		state_up = true;

		index_r = r;
		index_c = c;

		makeCuboid(vertex, mountain::width / 2, mountain::length / 2, max_height);

		GLfloat tempCol = dis(gen) * 0.001;

		setCol(color, tempCol, tempCol, tempCol);

		transformation = glm::mat4(1.0f);
		transformation = glm::translate(transformation,
						 glm::vec3((-500.0f + mountain::width / 2) + mountain::width * index_r, 0.0f, (-500.0f + mountain::length / 2) + mountain::length * index_c));

		transformation = glm::scale(transformation, glm::vec3(1.0f, now_height, 1.0f));

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
	}

	GLvoid draw(unsigned int& modelLocation);
	GLvoid init_animation();
	GLvoid animation();
};

GLfloat mountain::width = 0.0f;
GLfloat mountain::length = 0.0f;
GLboolean mountain::initAni = false;


GLvoid mountain::draw(unsigned int& modelLocation)
{
	glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(transformation));
	glBindVertexArray(vao);
	glDrawArrays(GL_TRIANGLES, 0, vertex.size() / 3);
}

GLvoid mountain::init_animation()
{
	now_height += 0.1f;
	if (now_height >= 1.0f)
	{
		state_up = false;
		mountain::initAni = true;
		now_height = 1.0f;
	}
	transformation = glm::mat4(1.0f);
	transformation = glm::translate(transformation,
		glm::vec3((-500.0f + mountain::width / 2) + mountain::width * index_r, max_height * now_height, (-500.0f + mountain::length / 2) + mountain::length * index_c));
	transformation = glm::scale(transformation, glm::vec3(1.0f, now_height, 1.0f));
}

GLvoid mountain::animation()
{
	if (state_up)
	{
		now_height += speed;
		if (now_height >= 1.0f)
		{
			now_height = 1.0f;
			state_up = false;
		}
	}
	else if(!state_up)
	{
		now_height -= speed;
		if (now_height <= 0.0f)
		{
			now_height = 0.0f;
			state_up = true;
		}
	}
	transformation = glm::mat4(1.0f);
	transformation = glm::translate(transformation,
		glm::vec3((-500.0f + mountain::width / 2) + mountain::width * index_r, max_height * now_height, (-500.0f + mountain::length / 2) + mountain::length * index_c));
	transformation = glm::scale(transformation, glm::vec3(1.0f, now_height, 1.0f));
}
