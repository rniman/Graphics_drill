#define _USE_MATH_DEFINES
#include "axes.h"
#include "read_Obj.h"
#include "cuboid.h"
#include "floor.h"
#include "mountain.h"
//#include "maze.h"
#include "my_maze.h"

GLvoid drawScene();
GLvoid Reshape(int w, int h);
GLvoid TimeEvent(int value);
GLvoid KeyEvent(unsigned char key, int x, int y);
GLvoid KeyUpEvent(unsigned char key, int x, int y);
GLvoid MouseClick();
GLvoid MouseMove();

void initBuffer();

GLvoid convert_OpenglXY_WindowXY(int& x, int& y, const float& ox, const float& oy);
GLvoid convert_WindowXY_OpenglXY(const int& x, const int& y, float& ox, float& oy);

const GLint window_w = 1000, window_h = 800;
GLfloat rColor = 0.0f, gColor = 0.0f, bColor = 0.0f;

namespace STATE
{
	GLboolean perspective = true;;
	GLboolean mountain_animation = false;
	GLboolean makeMaze = false;
}


unsigned int modelLocation;
unsigned int viewLocation;
unsigned int projLocation;

glm::mat4 camera;
//glm::vec3 camera_eye = glm::vec3(700.0f,900.0f,700.0f);
glm::vec3 camera_eye = glm::vec3(0.0f, 1000.0f, 0.0f);

GLfloat cameraAngle = 0.0f;

glm::mat4 topViewCamera;
glm::vec3 tVCamra_eye = glm::vec3(0.0f, 1000.0f, 0.0f);

glm::mat4 projection;
glm::mat4 view;

GLuint vao;
GLuint vbo_axes[2];
axes_coordination axes;

map_floor mapFloor;
GLuint vao_floor;
GLuint vbo_floor[2];

std::vector<std::vector<mountain>> mountain_list;

maze m;

int main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
	glutInitWindowPosition(300, 50);
	glutInitWindowSize(window_w, window_h);
	glutCreateWindow("assignment");

	glewExperimental = GL_TRUE;
	glewInit();

	std::cout << "가로입력(5~25):";
	std::cin >> mountain::rNum;
	std::cout << "세로입력(5~25):";
	std::cin >> mountain::cNum;
	if (mountain::rNum < 5 || mountain::rNum > 25)
		mountain::rNum = 5;
	if (mountain::cNum < 5 || mountain::cNum > 25)
		mountain::cNum = 5;
	mapFloor.set_floor(mountain::rNum, mountain::cNum);

	m.initialize((mountain::rNum + 1) / 2, (mountain::cNum + 1) / 2);
	while(!maze::completeGenerate)
		m.generator();


	mountain::length = mapFloor.get_length();
	mountain::width = mapFloor.get_width();

	mountain_list = std::vector<std::vector<mountain>>(mountain::cNum);
	for (int i = 0; i < mountain::cNum; ++i)
	{		
		for (int j = 0; j < mountain::rNum; ++j)
		{
			mountain_list[i].push_back(mountain(j, i));
		}
	}



	//세이더 읽어와서 세이더 프로그램 만들기
	shaderID = make_shaderProgram();	//세이더 프로그램 만들기
	initBuffer();

	glutDisplayFunc(drawScene);
	glutReshapeFunc(Reshape);
	glutTimerFunc(100, TimeEvent, 0);
	glutKeyboardFunc(KeyEvent);
	glutKeyboardUpFunc(KeyUpEvent);

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	modelLocation = glGetUniformLocation(shaderID, "modelTransform");
	viewLocation = glGetUniformLocation(shaderID, "viewTransform");
	projLocation = glGetUniformLocation(shaderID, "projectionTransform");

	camera = glm::lookAt(camera_eye, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f));
	topViewCamera = glm::lookAt(tVCamra_eye, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f));
	
	projection = glm::mat4(1.0f);
	//근평면은 포함이고 원평면은 포함X
	projection = glm::perspective(glm::radians(90.0f), 1.0f, 50.0f, 3000.0f);


	glutMainLoop();
}


GLvoid drawScene()
{
	glClearColor(rColor, gColor, bColor, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	//랜더링 파이프라인에 세이더 불러오기
	
	glUseProgram(shaderID);
	glViewport(0, 0, window_w, window_h);

	//카메라 변환 적용
	glUniformMatrix4fv(viewLocation, 1, GL_FALSE, glm::value_ptr(camera));

	//투영 변환 적용
	glUniformMatrix4fv(projLocation, 1, GL_FALSE, glm::value_ptr(projection));
	
	//좌표축 그리기
	modelLocation = glGetUniformLocation(shaderID, "modelTransform");
	glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(axes.transformation));
	glBindVertexArray(vao);
	glDrawArrays(GL_LINES, 0, 6);

	//바닥 그리기
	glUniformMatrix4fv(modelLocation, 1, GL_FALSE, mapFloor.get_ptr_transformation());
	glBindVertexArray(vao_floor);
	glDrawArrays(GL_TRIANGLES, 0, mapFloor.get_vertex().size() / 3);
	

	
	if (STATE::makeMaze)
	{
		for (int i = 0; i < mountain::cNum; ++i)
		{
			for (int j = 0; j < mountain::rNum; ++j)
				mountain_list[i][j].drawMaze(modelLocation);
		}
	}
	else
	{
		for (int i = 0; i < mountain::cNum; ++i)
		{
			for (int j = 0; j < mountain::rNum; ++j)
				mountain_list[i][j].draw(modelLocation);
		}
	}

	glViewport(800, 600, 200, 200);
	glUniformMatrix4fv(viewLocation, 1, GL_FALSE, glm::value_ptr(topViewCamera));
	if (STATE::makeMaze)
	{
		for (int i = 0; i < mountain::cNum; ++i)
		{
			for (int j = 0; j < mountain::rNum; ++j)
				mountain_list[i][j].drawMaze(modelLocation);
		}
	}
	else
	{
		for (int i = 0; i < mountain::cNum; ++i)
		{
			for (int j = 0; j < mountain::rNum; ++j)
				mountain_list[i][j].draw(modelLocation);
		}
	}

	glutSwapBuffers();
}

GLvoid Reshape(int w, int h)
{
	glViewport(0, 0, w, h);
}


GLvoid TimeEvent(int value)
{
	camera = glm::mat4(1.0f);
	camera = glm::lookAt(camera_eye, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	camera = glm::rotate(camera, glm::radians(cameraAngle), glm::vec3(0.0f, 1.0f, 0.0f));

	if (!mountain::initAni)
	{
		for (int i = 0; i < mountain::cNum; ++i)
		{
			for (int j = 0; j < mountain::rNum; ++j)
				mountain_list[i][j].init_animation();
		}
	}
	else if(STATE::mountain_animation)
	{
		for (int i = 0; i < mountain::cNum; ++i)
		{
			for (int j = 0; j < mountain::rNum; ++j)
				mountain_list[i][j].animation();
		}
	}
	

	glutPostRedisplay();
	glutTimerFunc(100, TimeEvent, 0);
}

GLvoid KeyEvent(unsigned char key, int x, int y)
{
	if (key == 'q')
		glutExit();
	else if (key == 'y')
	{
		cameraAngle += 10.0f;
	}
	else if (key == 'Y')
	{
		cameraAngle -= 10.0f;
	}
	else if (key == 'm')
	{
		STATE::mountain_animation = true;
	}
	else if (key == 'M')
	{
		STATE::mountain_animation = false;
	}
	else if (key == 'o')
	{
		STATE::perspective = false;
		projection = glm::mat4(1.0f);
		projection = glm::ortho(-1000.0f, 1000.0f, -1000.0f, 1000.0f, 50.0f, 2000.0f);
	}
	else if (key == 'p')
	{
		STATE::perspective = true;
		projection = glm::mat4(1.0f);
		projection = glm::perspective(glm::radians(90.0f), 1.0f, 50.0f, 2000.0f);
	}
	else if (key == 'r' && mountain::initAni)
	{
		STATE::makeMaze = true;
		set_maze(m, mountain_list);
	}
}

GLvoid KeyUpEvent(unsigned char key, int x, int y)
{

}

void initBuffer()
{
	glGenVertexArrays(1, &vao);
	glGenBuffers(2, vbo_axes);

	glBindVertexArray(vao);
	
	glBindBuffer(GL_ARRAY_BUFFER, vbo_axes[1]);
	glBufferData(GL_ARRAY_BUFFER, axes.axes_color.size() * sizeof(GLfloat), axes.axes_color.data(), GL_STATIC_DRAW);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, vbo_axes[0]);
	glBufferData(GL_ARRAY_BUFFER, axes.axes_vertex.size() * sizeof(GLfloat), axes.axes_vertex.data(), GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);

	glGenVertexArrays(1, &vao_floor);
	glGenBuffers(2, vbo_floor);

	glBindVertexArray(vao_floor);

	glBindBuffer(GL_ARRAY_BUFFER, vbo_floor[1]);
	glBufferData(GL_ARRAY_BUFFER, mapFloor.get_color().size() * sizeof(GLfloat), mapFloor.get_color().data(), GL_STATIC_DRAW);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, vbo_floor[0]);
	glBufferData(GL_ARRAY_BUFFER, mapFloor.get_vertex().size() * sizeof(GLfloat), mapFloor.get_vertex().data(), GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);

}