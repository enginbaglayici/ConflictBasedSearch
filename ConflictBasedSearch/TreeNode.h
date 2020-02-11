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
	void setSolution(const std::vector<std::vector<Cell>> &&solution);
	void addConstraint(const Constraint &constaint);
	void setConstraint(const Constraint &constaint);
	void updateSolution(Map map);
	void updateCost();

	std::vector<std::vector<Cell>> getSolution();
	int getCost() const;

	std::vector<Constraint> getConstraints();

	TreeNode();

	TreeNode(const std::vector<Constraint> &constraints);
	~TreeNode();
};

