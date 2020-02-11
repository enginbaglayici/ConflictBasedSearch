#include "TreeNode.h"


std::vector<std::vector<Cell>> TreeNode::getSolution()
{
	return solution;
}

int TreeNode::getCost() const {
	return cost;
}


std::vector<Constraint> TreeNode::getConstraints() {
	return std::vector<Constraint>();
}

TreeNode::TreeNode() {
}

TreeNode::TreeNode(const std::vector<Constraint> &constraints) {
	this->constraints = constraints;
}


TreeNode::~TreeNode()
{
}

void TreeNode::setSolution(const std::vector<std::vector<Cell>>&& solution) {

}

void TreeNode::addConstraint(const Constraint & constaint) {
	this->constraints.emplace_back(constaint);
}

void TreeNode::setConstraint(const Constraint & solution) {

}

void TreeNode::updateSolution(Map map) {
	LowLevelSolver solver;
	solution = solver.findOptimalPaths(constraints, map);
}



void TreeNode::updateCost() {
	auto cost = 0;
	for (auto route : solution) {
		cost += route.size();
	}
	this->cost = cost;
}
