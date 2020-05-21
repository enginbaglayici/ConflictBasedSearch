#pragma once
#include "LowLevelSolver.h"
#include "util.h"

class TreeNode
{
private:
	int cost = 0;

	std::vector<Constraint> constraints;
	std::vector<std::vector<Cell>> solution;

public:
	void addConstraint(const Constraint &constaint);
	void updateSolution(const Map &map);
	void updateCost();

	std::vector<std::vector<Cell>> getSolution() const;
	int getCost() const;

	std::vector<Constraint> getConstraints();

	TreeNode();

	TreeNode(const std::vector<Constraint> &constraints);
	~TreeNode();
};

