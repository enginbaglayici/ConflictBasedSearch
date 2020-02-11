#pragma once
#include "TreeNode.h"
#include "util.h"

class HighLevelSolver
{
private:
	inline bool isEmpty(const std::vector<TreeNode> &tree);
	bool hasConflict(std::vector<Cell> route1, std::vector<Cell> route2);
	bool hasConflict(TreeNode P);
	bool hasEdgeConflict(std::vector<Cell> route1, std::vector<Cell> route2);
	bool hasEdgeConflict(TreeNode P);
	int getMinCost(const std::vector<TreeNode> &tree);
	Conflict getFirstConflict(TreeNode P);
	TreeNode findBestNode(std::vector<TreeNode> tree);

public:
	HighLevelSolver();
	~HighLevelSolver();
	std::vector<std::vector<Cell>> solve(Map map);
};




