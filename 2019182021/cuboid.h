#pragma once
#include "make_Shader.h"

//width = �غ�/2  height = ���� / 2
GLvoid makeCuboid(std::vector<GLfloat>& obj, const GLfloat& width, const GLfloat& hegiht)
{
	obj = {
		//�ո�
		-width, -hegiht, width,
		width, hegiht, width,
		-width, hegiht, width,

		-width, -hegiht, width,
		width, -hegiht, width,
		width, hegiht, width
			,
		//������
		width, -hegiht, width,
		width, hegiht, -width,
		width, hegiht, width,

		width, -hegiht, width,
		width, -hegiht, -width,
		width, hegiht, -width
		,
		//�޸�
		width, -hegiht, -width,
		-width, -hegiht, -width,
		-width, hegiht, -width,

		width, -hegiht, -width,
		-width, hegiht, -width,
		width, hegiht, -width
		,
		//�޸�
		-width, -hegiht, -width,
		-width, -hegiht, width,
		-width, hegiht, width,

		-width, -hegiht, -width,
		-width, hegiht, width,
		-width, hegiht, -width
		,
		//����
		width, hegiht, width,
		width, hegiht, -width,
		-width, hegiht, width,

		-width, hegiht, width,
		width, hegiht, -width,
		-width, hegiht, -width
		,
		//�޸�
		width, -hegiht, width,
		-width, -hegiht, width,
		-width, -hegiht, -width,

		width, -hegiht, width,
		-width, -hegiht, -width,
		width, -hegiht, -width
	};
}

GLvoid makeCuboid(std::vector<GLfloat>& obj, const GLfloat& width,const GLfloat& length, const GLfloat& hegiht)
{
	obj = {
		//�ո�
		-width, -hegiht, length,
		width, hegiht, length,
		-width, hegiht, length,

		-width, -hegiht, length,
		width, -hegiht, length,
		width, hegiht, length
			,
		//������
		width, -hegiht, length,
		width, hegiht, -length,
		width, hegiht, length,

		width, -hegiht, length,
		width, -hegiht, -length,
		width, hegiht, -length
		,
		//�޸�
		width, -hegiht, -length,
		-width, -hegiht, -length,
		-width, hegiht, -length,

		width, -hegiht, -length,
		-width, hegiht, -length,
		width, hegiht, -length
		,
		//�޸�
		-width, -hegiht, -length,
		-width, -hegiht, length,
		-width, hegiht, length,

		-width, -hegiht, -length,
		-width, hegiht, length,
		-width, hegiht, -length
		,
		//����
		width, hegiht, length,
		width, hegiht, -length,
		-width, hegiht, length,

		-width, hegiht, length,
		width, hegiht, -length,
		-width, hegiht, -length
		,
		//�ظ�
		width, -hegiht, length,
		-width, -hegiht, length,
		-width, -hegiht, -length,

		width, -hegiht, length,
		-width, -hegiht, -length,
		width, -hegiht, -length
	};
}

GLvoid genCol(std::vector<GLfloat>& col, const GLfloat& red, const GLfloat& green, const GLfloat& blue)
{
	for (int i = 0; i < 36; ++i)
	{
		col.push_back(red);
		col.push_back(green);
		col.push_back(blue);
	}
}

GLvoid setCol(std::vector<GLfloat>& col, const GLfloat& red, const GLfloat& green, const GLfloat& blue)
{
	for (int i = 0; i < col.size() /3; ++i)
	{
		col[3 * i] = red;
		col[3 * i + 1] = green;
		col[3 * i + 2] = blue;
	}
}
