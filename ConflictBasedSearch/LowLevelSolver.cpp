#include <algorithm>
#include "LowLevelSolver.h"
#include <iostream>
#include <sstream>
#include <map>
#include <iterator>


LowLevelSolver::LowLevelSolver() = default;

LowLevelSolver::~LowLevelSolver() = default;

bool LowLevelSolver::checkStartGoalCells(Cell start, Cell goal, Map map)
{
	if (!isValid(start.x, start.y, map)) {
		std::cout << "Start cell is invalid";
		return false;
	}
	if (!isValid(goal.x, goal.y, map)) {
		std::cout << "Goal cell is invalid";
		return false;
	}
	if (start.isObstacle) {
		std::cout << "Obstacle in start cell";
		return false;
	}
	if (goal.isObstacle) {
		std::cout << "Obstacle in goal cell";
		return false;
	}
	return true;
}

inline bool LowLevelSolver::isEmpty(const std::vector<Cell> &vec) {
	return vec.empty();
}

inline bool LowLevelSolver::isObstacle(Map map, int x, int y) {
	/*
	try {
		return map.cells[x][y].isObstacle;
	}
	catch (const std::out_of_range& e) {
		return false;
	}
	*/
	return map.cells[x][y].isObstacle;
}

inline bool LowLevelSolver::isValid(int x, int y, Map map) {
	size_t a = x;
	size_t b = y;
	return (a >= 0) && (a < map.cells.size()) && (b >= 0) && (b < map.cells[0].size()) && !isObstacle(map, a, b);
}

//checks if the visited cell has a constraint
bool LowLevelSolver::isConstraint(int agentID, int x, int y, int time, std::vector<Constraint> constraints) {
	for (size_t i = 0; i < constraints.size(); i++) {
		if (agentID == constraints[i].agentID) {
			if (time == constraints[i].time && constraints[i].cell.x == x && constraints[i].cell.y == y) {
				return true;
			}
		}
	}
	return false;
}

bool compareF(Cell cell1, Cell cell2) {
	return cell1.f < cell2.f;
}

inline int LowLevelSolver::findHeuristicDistance(Cell current_cell, Cell goal)
{
	return abs(current_cell.x - goal.x) + abs(current_cell.y - goal.y);   //Manhattan distance
}

void LowLevelSolver::updateCostFunction(Cell &successor, Cell &currentNode, Cell goal, Cell start) {
	successor.h = 0;
	successor.g = 0;
	successor.f = 0;
	successor.g = currentNode.g + 1;
	successor.g = findHeuristicDistance(successor, start);
	successor.h = findHeuristicDistance(successor, goal);
	successor.f = successor.g + successor.h;
}

Cell LowLevelSolver::findMinCostCell(std::vector<Cell> OPEN) {
	Cell min_cell;
	int min_value = INT_MAX;

	for (Cell c : OPEN)
	{
		if (c.f < min_value)
		{
			min_value = c.f;
			min_cell = c;
		}
	}
	return min_cell;
}

Cell* LowLevelSolver::findParent(std::vector<Cell> OPEN)
{
	Cell *parent = new Cell;
	parent = NULL;
	int min_value = INT_MAX;

	for (Cell c : OPEN)
	{
		if (c.f < min_value)
		{
			parent = c.parent;
		}
	}
	return parent;
}

inline bool LowLevelSolver::contains(std::vector<Cell> cells, Cell cell) {
	for (std::vector<Cell>::iterator it = cells.begin(); it != cells.end(); ++it) {
		if (*it == cell)
		{
			return true;
		}
	}
	return false;
}

inline int LowLevelSolver::findIndex(std::vector<Cell> cells, Cell cell) {							// BURADA CELLI KARSILASTIRMADA HATA VAR
	int index = 0;
	for (std::vector<Cell>::iterator it = cells.begin(); it != cells.end(); ++it) {
		if (*it == cell)
		{
			break;
		}
		index++;
	}
	return index;
}

std::vector<Cell> LowLevelSolver::updatePath(std::vector<Cell> cells, std::vector<Constraint> constraints)
{
	int index = 0;
	for (Cell cell : cells)
	{
		for (Constraint i : constraints)
		{
			if (cell == i.cell)
			{
				cells.insert(cells.begin() + (index + 1), cell);
			}
		}
		index++;
	}
	return cells;
}

// for each agent find optimal path
std::vector<std::vector<Cell>> LowLevelSolver::findOptimalPaths(std::vector<Constraint> constraints, Map map) {

	for (size_t k = 0; k < map.agents.size(); k++) {
		optimalPaths.emplace_back(solve(constraints, map, k + 1));

		/*
		for (Cell i : optimalPaths[k]) {

			std::cout << i.x << i.y << "\n";
		}
		*/
	}
	return optimalPaths;
}

std::vector<Cell> LowLevelSolver::solve(std::vector<Constraint> constraints, Map map, int agentID) {
	Cell current_cell, temp_cell, child_cell, start, goal, successor;
	optimalPath.clear();
	successorCells.clear();
	std::map<std::pair<int, int>, std::pair<int, int>> path;
	start = map.agents[agentID - 1].start;
	current_cell = start;
	std::cout << "Start Cell is :" << current_cell.x << current_cell.y << " \n";
	goal = map.agents[agentID - 1].end;
	std::cout << "Goal Cell is :" << goal.x << goal.y << " \n";
	child_cell = current_cell;
	time = 1;
	successor.parent = new Cell;
	current_cell.parent = new Cell;
	checkStartGoalCells(current_cell, goal, map);
	OPEN.push_back(current_cell);

	while (!(findMinCostCell(OPEN) == goal)) {
		current_cell = findMinCostCell(OPEN);
		current_cell.parent = findParent(OPEN);
		OPEN.erase(std::min_element(OPEN.begin(), OPEN.end(), compareF));
		CLOSE.push_back(current_cell);

		if (isValid(current_cell.x - 1, current_cell.y, map) && !isConstraint(agentID, current_cell.x - 1, current_cell.y, time, constraints))
		{
			child_cell = current_cell;
			child_cell.x -= 1;

			successorCells.push_back(child_cell);
		}

		if (isValid(current_cell.x + 1, current_cell.y, map) && !isConstraint(agentID, current_cell.x + 1, current_cell.y, time, constraints))
		{
			child_cell = current_cell;
			child_cell.x += 1;

			successorCells.push_back(child_cell);
		}

		if (isValid(current_cell.x, current_cell.y - 1, map) && !isConstraint(agentID, current_cell.x, current_cell.y - 1, time, constraints))
		{
			child_cell = current_cell;
			child_cell.y -= 1;

			successorCells.push_back(child_cell);
		}

		if (isValid(current_cell.x, current_cell.y + 1, map) && !isConstraint(agentID, current_cell.x, current_cell.y + 1, time, constraints))
		{
			child_cell = current_cell;
			child_cell.y += 1;

			successorCells.push_back(child_cell);
		}

		int size = successorCells.size();

		for (int i = 0; i < size; i++)				// for each successor (Left-Right-Up-Down)
		{
			successor = successorCells.back();

			if (successor == goal)
			{
				//	std::cout << "Goal cell is found.\n";
				break;
			}

			updateCostFunction(successor, current_cell, goal, start);

			if (contains(OPEN, successor))
			{
				auto index = findIndex(OPEN, successor);
				if (OPEN[index].f > successor.f)
				{
					OPEN.erase(OPEN.begin() + index);
				}
			}

			if (contains(CLOSE, successor))
			{
				auto index = findIndex(CLOSE, successor);
				if (CLOSE[index].f > successor.f)
				{
					CLOSE.erase(CLOSE.begin() + index);
				}
			}

			if (!(contains(OPEN, successor)) && !(contains(CLOSE, successor)))
			{
				if (&current_cell != NULL)
				{
					successor.f = findHeuristicDistance(successor, goal);
					path.emplace(std::make_pair(successor.x, successor.y), std::make_pair(current_cell.x, current_cell.y));
				}
				OPEN.push_back(successor);
			}
			successorCells.pop_back();
		}
		if (successor == goal) {
			//	std::cout << "Goal cell is found.\n";
			path.emplace(std::make_pair(successor.x, successor.y), std::make_pair(current_cell.x, current_cell.y));
			break;
		}
		time++;
	}

	optimalPath.push_back(goal);
	for (auto iter = path.begin(); iter != path.end(); iter++)
	{
		if (iter->first.first == goal.x && iter->first.second == goal.y)
		{
			goal.x = iter->second.first;
			goal.y = iter->second.second;
			/*
			std::cout << '\t' << iter->first.first
				<< '\t' << iter->first.second << '\n';
			*/
			optimalPath.push_back(goal);
			iter = path.begin();
		}

	}

	path.clear();
	OPEN.clear();
	CLOSE.clear();
	std::reverse(optimalPath.begin(), optimalPath.end());
	return updatePath(optimalPath, constraints);
	//return optimalPath;
}


