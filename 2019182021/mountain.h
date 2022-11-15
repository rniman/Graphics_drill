#pragma once
#include "make_Shader.h"
#include "cuboid.h"
#include "my_maze.h"

class mountain
{
private:
	GLuint vao;
	GLuint vbo[2];

	std::vector<GLfloat> vertex = std::vector<GLfloat>(108);
	std::vector<GLfloat> color = std::vector<GLfloat>(108);;
	glm::mat4 transformation;
	
	GLint index_r;
	GLint index_c;
	GLfloat max_height;
	GLfloat speed;
	GLfloat now_height;
	GLboolean state_up;

public:
	GLboolean maze_state;

	static GLfloat width;
	static GLfloat length;
	static GLboolean initAni;
	static GLint rNum;
	static GLint cNum;

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
		//true라면 미로의 길이다 즉 바닥으로 변한다
		maze_state = false;

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
	GLvoid drawMaze(unsigned int& modelLocation);
	GLvoid init_animation();
	GLvoid animation();
	friend GLvoid set_maze(const maze& completeMaze, std::vector<std::vector<mountain>>& mountainList);

};

GLfloat mountain::width = 0.0f;
GLfloat mountain::length = 0.0f;
GLboolean mountain::initAni = false;
GLint mountain::rNum = 0;
GLint mountain::cNum = 0;


GLvoid mountain::draw(unsigned int& modelLocation)
{

	glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(transformation));
	glBindVertexArray(vao);
	glDrawArrays(GL_TRIANGLES, 0, vertex.size() / 3);
}

GLvoid mountain::drawMaze(unsigned int& modelLocation)
{
	if (maze_state)
		return;
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

GLvoid set_maze(const maze& completeMaze, std::vector<std::vector<mountain>>& mountainList)
{
	for (int i = 0; i < mountain::cNum; ++i)
	{
		for (int j = 0; j < mountain::rNum; ++j)
		{
			if (i % 2 == 1 || j % 2 == 1)
				mountainList[i][j].maze_state = false;
			else
				mountainList[i][j].maze_state = true;
		}
	}

	for (int i = 0; i < (mountain::cNum + 1) / 2; ++i)
	{
		for (int j = 0; j < (mountain::rNum + 1) / 2; ++j)
		{			
			//0 top  1 right  2 bottom  3 left
			if(!completeMaze.Maze[i][j].wallOpen[0])
				mountainList[i * 2 - 1][j * 2].maze_state = true;
			
			if (!completeMaze.Maze[i][j].wallOpen[1])
				mountainList[i * 2][j * 2 + 1].maze_state = true;
			
			if (!completeMaze.Maze[i][j].wallOpen[2])
				mountainList[i * 2 + 1][j * 2].maze_state = true;
			
			if (!completeMaze.Maze[i][j].wallOpen[3])
				mountainList[i * 2][j * 2 - 1].maze_state = true;
		}
	}

	mountainList[mountain::cNum - 1][mountain::rNum - 1].maze_state = true;

	//std::cout << completeMaze.Maze[(mountain::cNum + 1) / 2 - 1][(mountain::rNum + 1) / 2 - 1].wallOpen[0];
	//std::cout << completeMaze.Maze[(mountain::cNum + 1) / 2 - 1][(mountain::rNum + 1) / 2 - 1].wallOpen[2];
	//std::cout << completeMaze.Maze[(mountain::cNum + 1) / 2 - 1][(mountain::rNum + 1) / 2 - 1].wallOpen[1];
	//std::cout << completeMaze.Maze[(mountain::cNum + 1) / 2 - 1][(mountain::rNum + 1) / 2 - 1].wallOpen[3];
	//왼쪽 top 아래 left 위 right 오른쪽 bottom
}