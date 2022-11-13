#pragma once

#pragma once
#include <stack>
#include <vector>
#include <random>

using namespace std;

enum DIRECTION
{
	TOP = 0,
	BOTTOM,
	LEFT,
	RIGHT,
};

const int gTileSize = 40;

class cTile {
public:
	int mX, mY;
	bool bIsWallOpen[4] = { true, true, true, true };
	bool bIsVisited = false;

	cTile(int _x, int _y) {
		mX = _x;
		mY = _y;
	};

	cTile() {
		mX = 0;
		mY = 0;
	};

};

class cMaze
{

private:
	int mMazeWidth;
	int mMazeHeight;

public:
	cTile** Maze;
	cTile* curTile;
	bool isReset;

	//for sidewinder member
	int SideWinderCount = 0;

	int visitedCellCount;

	//for RecursiveBacktracking member
	stack<cTile*> tileStack;

	int mazeAlgorithmType = 0;
	bool bIsCompleteGenerated = false;

public:
	cMaze();
	~cMaze();

	void Initialize();

	void MazeGenerator_RecursiveBacktracking();
	void Astar();

	cTile* checkNeighborTiles(const cTile curTile) {
		//현재 타일 위치
		int x = curTile.mX;
		int y = curTile.mY;

		bool top = true,
			right = true,
			bottom = true,
			left = true;

		if (y == 0) top = false;
		if (x == mMazeWidth - 1) right = false;
		if (y == mMazeHeight - 1) bottom = false;
		if (x == 0) left = false;

		//방문하지 않은 이웃 타일 탐색

		//이웃 타일 저장용 벡터
		vector <cTile*> nbTiles;

		if (top)
			if (!Maze[y - 1][x].bIsVisited)
				nbTiles.push_back(&Maze[y - 1][x]);

		if (right)
			if (!Maze[y][x + 1].bIsVisited)
				nbTiles.push_back(&Maze[y][x + 1]);

		if (bottom)
			if (!Maze[y + 1][x].bIsVisited)
				nbTiles.push_back(&Maze[y + 1][x]);

		if (left)
			if (!Maze[y][x - 1].bIsVisited)
				nbTiles.push_back(&Maze[y][x - 1]);

		//이웃한 타일중 방문하지 않은 타일이 있다면
		if (!nbTiles.empty()) {
			//타일 중 랜덤한 타일 리턴
			int r = GetRandom(0, nbTiles.size() - 1);
			return nbTiles[r];
		}
		//없다면
		else {
			return nullptr;
		}
	}

	int GetRandom(int min, int max) {
		random_device ranDevice;
		mt19937 gen(ranDevice());
		uniform_int_distribution<int> dis(min, max);

		return (int)dis(gen);
	}

	void OpenWall(const int dir, int x, int y) {

		switch (dir)
		{
		case TOP:
			Maze[y][x].bIsWallOpen[0] = false;
			Maze[y - 1][x].bIsWallOpen[2] = false;
			break;
		case BOTTOM:
			Maze[y][x].bIsWallOpen[2] = false;
			Maze[y + 1][x].bIsWallOpen[0] = false;
			break;
		case RIGHT:
			Maze[y][x].bIsWallOpen[1] = false;
			Maze[y][x + 1].bIsWallOpen[3] = false;
			break;
		case LEFT:
			Maze[y][x].bIsWallOpen[3] = false;
			Maze[y][x - 1].bIsWallOpen[1] = false;
			break;
		}
	}

	void OpenWall(const cTile* firstTile, const cTile* secondTile) {

		int checkDirection;

		if (firstTile->mX == secondTile->mX) {
			checkDirection = firstTile->mY - secondTile->mY;
			if (checkDirection < 0) { OpenWall(BOTTOM, firstTile->mX, firstTile->mY); }
			else { OpenWall(TOP, firstTile->mX, firstTile->mY); }
		}

		else {
			checkDirection = firstTile->mX - secondTile->mX;
			if (checkDirection < 0) { OpenWall(RIGHT, firstTile->mX, firstTile->mY); }
			else { OpenWall(LEFT, firstTile->mX, firstTile->mY); }
		}
	}

	void ResetMaze() {
		if (isReset) return;
		while (!tileStack.empty()) {
			tileStack.pop();
		}
		Initialize();

		isReset = true;
	}

};



