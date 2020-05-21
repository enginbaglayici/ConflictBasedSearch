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
	int time{};

	static inline bool isObstacle(const Map &map, int x, int y);
	static inline bool isConstraint(int agentID, int x, int y, int time, const std::vector<Constraint> &constraints);
	static inline bool contains(std::vector<Cell> cells, Cell cell);
	static int findIndex(std::vector<Cell> cells, Cell cell);
	static bool checkStartGoalCells(const Cell &start, const Cell &goal, const Map &map);
	static int findHeuristicDistance(Cell current_cell, Cell cell);			//Manhattan distance calculation
	static bool isValid(int x, int y, const Map &map);						// Checks if given cells are valid
	static void updateCostFunction(Cell &successor, Cell goal, Cell start);
	static Cell findMinCostCell(const std::vector<Cell> &OPEN);
	static Cell* findParent(const std::vector<Cell> &OPEN);


public:
	LowLevelSolver();
	~LowLevelSolver();

	std::vector<Cell> solve(const std::vector<Constraint> &constraints, const Map &map, int agentID);
	std::vector<std::vector<Cell>> findOptimalPaths(const std::vector<Constraint> &constraints, const Map &map);

};




