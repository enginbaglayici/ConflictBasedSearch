#pragma once
#include "TreeNode.h"
#include "util.h"

class HighLevelSolver
{
private:
	static inline bool isEmpty(const std::vector<TreeNode> &tree);
	static bool hasConflict(const std::vector<Cell> &route1, const std::vector<Cell> &route2);
	static bool hasConflict(const TreeNode &P);
	static bool hasEdgeConflict(const std::vector<Cell> &route1, const std::vector<Cell> &route2);
	static bool hasEdgeConflict(const TreeNode &P);
	static int getMinCost(const std::vector<TreeNode> &tree);
	static Conflict getFirstConflict(const TreeNode &P);
	static TreeNode findBestNode(const std::vector<TreeNode> &tree);

public:
	HighLevelSolver();
	~HighLevelSolver();
	static std::vector<std::vector<Cell>> solve(const Map &map);
};
