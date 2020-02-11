#include "HighLevelSolver.h"
#include <vector>
#include <algorithm>


HighLevelSolver::HighLevelSolver() = default;

HighLevelSolver::~HighLevelSolver() = default;


// Returns true if there is a conflict between two given routes
bool HighLevelSolver::hasConflict(std::vector<Cell> route1, std::vector<Cell> route2) {
	auto min_index = std::min(route1.size(), route2.size());
	for (int i = 0; i < min_index; i++) {
		if (route1[i] == route2[i])
			return true;
	}
	return false;
}

bool HighLevelSolver::hasConflict(TreeNode node) {
	auto solutions = node.getSolution();
	for (int i = 0; i < solutions.size(); i++) {
		for (int j = i + 1; j < solutions.size(); j++) {
			if (hasConflict(solutions[i], solutions[j]))
				return true;
		}
	}
	return false;
}

// Returns true if there is an edge conflict between two routes.
bool HighLevelSolver::hasEdgeConflict(std::vector<Cell> route1, std::vector<Cell> route2) {
	auto min_route_size = std::min(route1.size(), route2.size()) - 1;
	for (int i = 0; i < min_route_size; i++) {
		if (route1[i] == route2[i + 1] && route1[i + 1] == route2[i])
			return true;
	}
	return false;
}

bool HighLevelSolver::hasEdgeConflict(TreeNode node) {
	auto solutions = node.getSolution();

	for (int i = 0; i < solutions.size(); i++) {
		for (int j = i + 1; j < solutions.size(); j++) {
			if (hasEdgeConflict(solutions[i], solutions[j]))
				return true;
		}
	}
	return false;
}

Conflict HighLevelSolver::getFirstConflict(TreeNode P) {
	auto solutions = P.getSolution();

	// Look for normal conflicts first
	for (int i = 0; i < solutions.size(); i++) {
		for (int j = i + 1; j < solutions.size(); j++) {
			auto min_index = std::min(solutions[i].size(), solutions[j].size());
			for (int k = 0; k < min_index; k++) {
				if (solutions[i][k] == solutions[j][k])
					return Conflict(i, j, solutions[i][k], solutions[i][k], k);
			}
		}
	}

	// Look for Edge Conflicts if there is no normal conflicts
	for (int i = 0; i < solutions.size(); i++) {
		for (int j = i + 1; j < solutions.size(); j++) {
			auto min_index = std::min(solutions[i].size(), solutions[j].size()) - 1;
			for (int k = 0; k < min_index; k++) {
				auto route1 = solutions[i];
				auto route2 = solutions[j];
				if (route1[k] == route2[k + 1] && route1[k + 1] == route2[k])
					return Conflict(i, j, route1[k + 1], route2[k + 1], k);
			}
		}
	}
	// Trivial. It is assured that this method is called only when there is conflict
	return Conflict(0, 0, Cell(0, 0), Cell(0, 0), 0);
}


// Returns min cost on tree
int HighLevelSolver::getMinCost(const std::vector<TreeNode> &tree) {
	int min = INT_MAX;

	for (const auto &node : tree) {
		if (node.getCost() < min)
			min = node.getCost();
	}
	return min;
}

// Returns first node with minCost
TreeNode HighLevelSolver::findBestNode(std::vector<TreeNode> tree) {
	auto minCost = getMinCost(tree);
	for (const auto &node : tree) {
		if (node.getCost() == minCost)
			return node;
	}
	return TreeNode();
}

inline bool HighLevelSolver::isEmpty(const std::vector<TreeNode> &tree) {
	return tree.empty();
}

std::vector<std::vector<Cell>> HighLevelSolver::solve(Map map) {
	std::vector<TreeNode> tree;

	auto root = TreeNode();
	root.updateSolution(map);
	root.updateCost();

	tree.emplace_back(root); // TODO

	while (!isEmpty(tree)) {
		TreeNode P;
		P = findBestNode(tree);
		if (!hasConflict(P)) {
			return P.getSolution();
		}
		auto conflict = getFirstConflict(P);

		// Remove current node from tree because it has conflicts.
		tree.pop_back();

		for (int i = 0; i < 2; i++) {

			// Initialize new node with current nodes constraints
			TreeNode A(P.getConstraints());

			auto newConstraint = Constraint(conflict.conflictedAgentsID.first, conflict.cell1, conflict.time);

			if (i == 1) {
				newConstraint = Constraint(conflict.conflictedAgentsID.second, conflict.cell2, conflict.time);
			}

			// Add new constraint, solve with low level solver and update cost
			A.addConstraint(newConstraint);
			A.updateSolution(map);
			A.updateCost();

			// If a solution found, push it to the tree
			if (A.getCost() < INT_MAX) {
				tree.emplace_back(A);
			}
		}
	}

	// Return empty vector if there is no solution.
	return std::vector<std::vector<Cell>>();
}

