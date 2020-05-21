#include <algorithm>
#include "LowLevelSolver.h"
#include <iostream>

<<<<<<< HEAD
#include <climits>
=======
#include <limits.h>

>>>>>>> 3f52c6e3ab2df89014ab7331fadbee9d8aded953


LowLevelSolver::LowLevelSolver() = default;
LowLevelSolver::~LowLevelSolver() = default;

bool LowLevelSolver::checkStartGoalCells(const Cell &start, const Cell &goal, const Map &map)
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

inline bool LowLevelSolver::isObstacle(const Map &map, int x, int y) {
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

inline bool LowLevelSolver::isValid(int x, int y, const Map &map) {
	size_t a = x;
	size_t b = y;
	return (a >= 0) && (a < map.cells.size()) && (b >= 0) && (b < map.cells[0].size()) && !isObstacle(map, a, b);
}

//checks if the visited cell has a constraint
bool LowLevelSolver::isConstraint(int agentID, int x, int y, int time, const std::vector<Constraint> &constraints) {
	for(Constraint c : constraints) {
		if (agentID == c.agentID) {
			if (time == c.time && c.cell.x == x && c.cell.y == y) {
				return true;
			}
		}
	}
	return false;
}

inline bool compareF(const Cell &cell1, const Cell &cell2) {
	return cell1.f < cell2.f;
}

inline int LowLevelSolver::findHeuristicDistance(Cell current_cell, Cell goal)
{
	return abs(current_cell.x - goal.x) + abs(current_cell.y - goal.y);   //Manhattan distance
}

void LowLevelSolver::updateCostFunction(Cell &successor, Cell goal, Cell start) {
	successor.h = 0;
	successor.g = 0;
	successor.f = 0;
	//successor.g = currentNode.g + 1;
	successor.g = findHeuristicDistance(successor, start);
	successor.h = findHeuristicDistance(successor, goal);
	successor.f = successor.g + successor.h;
}

Cell LowLevelSolver::findMinCostCell(const std::vector<Cell> &OPEN) {
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

Cell* LowLevelSolver::findParent(const std::vector<Cell> &OPEN)
{
	Cell *parent = new Cell;
	parent = nullptr;
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
    return std::count(cells.begin(), cells.end(), cell);
}

inline int LowLevelSolver::findIndex(std::vector<Cell> cells, Cell cell) {
    // BURADA CELLI KARSILASTIRMADA HATA VAR
    auto it = std::find(cells.begin(), cells.end(), cell);
    return it != cells.end();
}


// for each agent find optimal path
std::vector<std::vector<Cell>> LowLevelSolver::findOptimalPaths(const std::vector<Constraint> &constraints, const Map &map) {
	
	for (auto k = 0; k < map.agents.size(); k++) {
		optimalPaths.emplace_back(solve(constraints, map, k));

	}
	return optimalPaths;
}

std::vector<Cell> LowLevelSolver::solve(const std::vector<Constraint> &constraints, const Map &map, int agentID) {
	Cell current_cell, child_cell, start, goal, successor;
	optimalPath.clear();
	successorCells.clear();
	std::vector<std::pair<Cell, Cell>> path;
	start = map.agents[agentID].start;
	current_cell = start;
	//std::cout << "Start Cell is : " << current_cell.x << "," << current_cell.y << " \n";
	goal = map.agents[agentID].end;
	//std::cout << "Goal Cell is : " << goal.x << "," << goal.y << " \n";
	child_cell = current_cell;
	time = 1;
	successor.parent = new Cell;
	current_cell.parent = new Cell;
	checkStartGoalCells(current_cell, goal, map);
	OPEN.push_back(current_cell);

	while (!(findMinCostCell(OPEN) == goal)) {
		current_cell = findMinCostCell(OPEN);
		current_cell.parent = findParent(OPEN);
		if (!OPEN.empty()) {
			OPEN.erase(std::min_element(OPEN.begin(), OPEN.end(), compareF));
		}
		
		/*
		if (goal.x == 1 && goal.y == 1 && current_cell.x == 2 && current_cell.y == 1) {
 			std::cout << "";
		}
		*/
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

			updateCostFunction(successor, goal, start);
		
			if (contains(OPEN, successor))
			{
				auto index = findIndex(OPEN, successor);
				if (OPEN[index].g < successor.g)
				{
					continue;
				}
			}

			if (contains(CLOSE, successor))
			{
				auto index = findIndex(CLOSE, successor);
				if (CLOSE[index].g < successor.g)
				{
					continue;
				}
				CLOSE.erase(CLOSE.begin() + index);
				OPEN.push_back(successor);
			}

			if (!(contains(OPEN, successor)) && !(contains(CLOSE, successor)))
			{
				if (&current_cell != nullptr)
				{
					successor.f = findHeuristicDistance(successor, goal);
					path.emplace_back(std::make_pair(successor, current_cell));
					successor.parent = &current_cell;
				}
				OPEN.push_back(successor);
			}

			successorCells.pop_back();
		}

		CLOSE.push_back(current_cell);

		if (successor == goal) {
			//	std::cout << "Goal cell is found.\n";
			path.emplace_back(std::make_pair(successor, current_cell));
			break;
		}
		time++;
	}
	/*
	for (auto elem : path)
	{
		std::cout << elem.first.x << " " << elem.first.y << " " << elem.second.x << " " << elem.second.y << std::endl;
	}
	*/
	optimalPath.push_back(goal);
	for (auto p1 : path) {
		for (auto p : path) {
			if (p.first == goal) {

				for (Constraint c : constraints) {
					if (c.cell == goal && agentID == c.agentID) {
						optimalPath.push_back(p.second);
					}
				}

				goal = p.second;

				optimalPath.push_back(goal);
			}
		}
	}

	for (auto elem : optimalPath)
	{
		std::cout << elem.x << " " << elem.y << "\n";
	}
	std::cout << " \n \n";

	path.clear();
	OPEN.clear();
	CLOSE.clear();
	std::reverse(optimalPath.begin(), optimalPath.end());
	//return updatePath(optimalPath, constraints);
	return optimalPath;
}


