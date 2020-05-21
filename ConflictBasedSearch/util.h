#pragma once
#include <vector>
#include <fstream>
#include <iostream>
#include <sstream>


struct Cell {
	bool isObstacle = false;
	int x, y;
	int f = 0, g = 0, h = 0;
	Cell *parent;
	Cell() = default;
	Cell(int x, int y) : x(x), y(y) { }

	// "==" operator overload. Two cells are equal if their x and y are same.
	bool operator==(const Cell& other) const {
		return x == other.x && y == other.y;
	}

	Cell operator=(const Cell & rhs)
	{
		if (this == &rhs)
			return *this;
		x = rhs.x;
		y = rhs.y;
		f = rhs.f;
		g = rhs.g;
		h = rhs.h;
		isObstacle = rhs.isObstacle;
		return *this;
	}
};

struct Agent {
	int id;
	Cell start = Cell(0, 0);
	Cell end = Cell(0, 0);

	Agent(int id) : id(id) {

	}
};


struct Conflict {
	std::pair<int, int> conflictedAgentsID;  // ai, aj
	Cell cell1;  // v
	Cell cell2;
	int time;  // t

	Conflict(int firstAgentID, int secondAgentID, Cell cell1, Cell cell2, int time) :
		cell1(cell1), cell2(cell2), time(time) {
		conflictedAgentsID = std::make_pair(firstAgentID, secondAgentID);
	}

};


struct Constraint {
	Cell cell;
	int agentID;
	int time;

	Constraint(int agentID, Cell cell, int time) :
		agentID(agentID), cell(cell), time(time) { }
};


struct Map {
	std::vector<std::vector<Cell>> cells;
	std::vector<Agent> agents;
};


