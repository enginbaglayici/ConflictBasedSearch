#pragma once
#include <vector>
#include <set>
#include <cstdlib>
#include "TreeNode.h"
#include "util.h"

class LowLevelSolver
{
private:
	std::vector<std::vector<Cell>> optimalPaths;
	std::vector<Cell> successorCells;
	std::vector<Cell> optimalPath;
	std::vector<Cell> OPEN;
	std::vector<Cell> CLOSE;
	int time;

	inline bool isObstacle(Map map, int x, int y);

	inline bool isEmpty(const std::vector<Cell> &vec);			//Checks if a given vector is empty
	inline bool isConstraint(int agentID, int x, int y, int time, std::vector<Constraint> constraints);
	inline bool contains(std::vector<Cell> cells, Cell cell);
	int findIndex(std::vector<Cell> cells, Cell cell);
	std::vector<Cell> updatePath(std::vector<Cell> cells, std::vector<Constraint> constraints);
	bool checkStartGoalCells(Cell start, Cell goal, Map map);
	int findHeuristicDistance(Cell current_cell, Cell cell);			//Manhattan distance calculation
	bool isValid(int x, int y, Map map);						// Checks if given cells are valid
	void updateCostFunction(Cell &successor, Cell &currentNode, Cell goal, Cell start);
	Cell findMinCostCell(std::vector<Cell> OPEN);
	Cell* findParent(std::vector<Cell> OPEN);


public:
	LowLevelSolver();
	~LowLevelSolver();

	std::vector<Cell> solve(std::vector<Constraint> constraints, Map map, int agentID);
	std::vector<std::vector<Cell>> findOptimalPaths(std::vector<Constraint> constraints, Map map);

};




