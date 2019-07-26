#pragma once

#include "KDPoint.h"

#include <algorithm>
#include <vector>
#include <utility>
#include <queue>

namespace nostd
{
	template <size_t DimensionsCount>
	class KDTree
	{
		struct Node
		{
			KDPoint<DimensionsCount> data;
			Node* left = nullptr, * right = nullptr;
		};
	public:
		KDTree(nullptr_t ptr = nullptr)
			:root{ ptr } {}

		KDTree(std::vector < KDPoint<DimensionsCount>> points)
		{
			root = CreateBalancedTree(points, 0, points.size());
		}

		std::vector<KDPoint<DimensionsCount>> ToVector() const
		{
			std::vector<KDPoint<DimensionsCount>> vector;
			Foreach([&vector](const KDPoint<DimensionsCount>& point)
				{
					vector.push_back(point);
				});
			return vector;
		}

		bool HasPoint(const KDPoint<DimensionsCount>& point)
		{
			return FindNode(point, root, PointsComparator<DimensionsCount>::GetComparator()) != nullptr;
		}

		void Insert(const KDPoint<DimensionsCount>& point)
		{
			TryInsert(point, root, PointsComparator<DimensionsCount>::GetComparator());
		}

		void Delete(const KDPoint<DimensionsCount>& point)
		{
			TryDelete(point, root, PointsComparator<DimensionsCount>::GetComparator());
		}

		template <typename Proccessor>
		void Foreach(Proccessor function) const
		{
			ForeachHelper(function, root);
		}

		KDPoint<DimensionsCount> GetClosestTo(const KDPoint<DimensionsCount>& position);
	private:
		Node* root;

		Node* CreateBalancedTree(std::vector<KDPoint<DimensionsCount>>& points, long left, long right, unsigned scoreIndex = 0);

		Node*& FindNode(const KDPoint<DimensionsCount>& point, Node*& currentNode, PointsComparator<DimensionsCount>& pathChooser);

		void TryInsert(const KDPoint<DimensionsCount>& point, Node* currentNode, PointsComparator<DimensionsCount>& pathChooser);

		Node* TryDelete(const KDPoint<DimensionsCount>& point, Node* currentNode, PointsComparator<DimensionsCount>& pathChooser);

		template <typename Proccessor>
		void ForeachHelper(Proccessor function, Node* currentNode) const;

		Node* GetLargestValue(Node* currentNode);

	};



	// TODO : remove "if" crutch
	template<size_t DimensionsCount>
	typename KDTree<DimensionsCount>::Node* KDTree<DimensionsCount>::CreateBalancedTree
	(std::vector<KDPoint<DimensionsCount>>& points, long left, long right, unsigned scoreIndex)
	{
		if (left <= right)
		{
			auto begin = points.begin() + left;
			if (begin != points.end())
			{
				auto middle = left + (right - left) / 2;
				auto end = points.begin() + right;
				std::nth_element(begin, points.begin() + middle, end, PointsComparator<DimensionsCount>{scoreIndex});
				Node* created_node = new Node{ points[middle],nullptr,nullptr };
				created_node->left = CreateBalancedTree(points, left, middle - 1, (scoreIndex + 1) % DimensionsCount);
				created_node->right = CreateBalancedTree(points, middle + 1, right, (scoreIndex + 1) % DimensionsCount);
				return created_node;
			}
		}
		return nullptr;
	}
	template<size_t DimensionsCount>
	typename KDTree<DimensionsCount>::Node*& KDTree<DimensionsCount>::FindNode(const KDPoint<DimensionsCount>& point, Node*& currentNode,
		PointsComparator<DimensionsCount>& pathChooser)
	{
		if (!currentNode || currentNode->data == point)
			return currentNode;
		Node* next_checked_node = pathChooser(point, currentNode->data) ? currentNode->left : currentNode->right;
		return FindNode(point, next_checked_node, ++pathChooser);
	}


	template<size_t DimensionsCount>
	void KDTree<DimensionsCount>::TryInsert(const KDPoint<DimensionsCount>& point, Node* currentNode, PointsComparator<DimensionsCount>& pathChooser)
	{
		if (currentNode->data != point)
		{
			auto isLeft = pathChooser(point, currentNode->data);
			if (isLeft)
			{
				if (currentNode->left) TryInsert(point, currentNode->left, ++pathChooser);
				else currentNode->left = new Node{ point,nullptr,nullptr };
			}
			else
			{
				if (currentNode->right) TryInsert(point, currentNode->right, ++pathChooser);
				else currentNode->right = new Node{ point,nullptr,nullptr };
			}
		}
	}

	template<size_t DimensionsCount>
	typename KDTree<DimensionsCount>::Node* KDTree<DimensionsCount>::TryDelete(const KDPoint<DimensionsCount>& point, Node* currentNode, PointsComparator<DimensionsCount>& pathChooser)
	{
		if (currentNode == nullptr) return nullptr;

		if (currentNode->data == point)
		{
			if (!currentNode->left && !currentNode->right)
			{
				delete currentNode;
				return nullptr;
			}
			Node* replacer = nullptr;
			if (currentNode->left == nullptr)
			{
				replacer = currentNode->right;
			}
			else if (currentNode->right == nullptr)
			{
				replacer = currentNode->left;
			}
			if (replacer) {
				delete currentNode;
				return replacer;
			}
			replacer = GetLargestValue(currentNode->left);
			currentNode->data = replacer->data;
			currentNode->left = TryDelete(replacer->data, currentNode->left, pathChooser);
		}

		if (pathChooser(point, currentNode->data))
		{
			currentNode->left = TryDelete(point,currentNode->left, ++pathChooser);
		}
		else
		{
			currentNode->right = TryDelete( point, currentNode->right, ++pathChooser);
		}

		return currentNode;
	}

	template<size_t DimensionsCount>
	typename  KDTree<DimensionsCount>::Node* KDTree<DimensionsCount>::GetLargestValue(Node* root)
	{
		if (root->right) return GetLargestValue(root->right);
		else return root;
	}

	template<size_t DimensionsCount>
	template<typename Proccessor>
	void KDTree<DimensionsCount>::ForeachHelper(Proccessor function, Node* currentNode) const
	{
		if (currentNode->left) ForeachHelper(function, currentNode->left);
		function(currentNode->data);
		if (currentNode->right) ForeachHelper(function, currentNode->right);
	}

	template<size_t DimensionsCount>
	KDPoint<DimensionsCount> KDTree<DimensionsCount>::GetClosestTo(const KDPoint<DimensionsCount>& position)
	{
		/*std::queue < std::pair<Node*, KDPoint<DimensionsCount>> to_visit{ root,  {} };
		unsigned long long min_distance = std::numeric_limits<unsigned long long>::max();
		auto path_finder = PointsComparator<DimensionsCount>::GetComparator();
		while (!to_visit.empty())
		{
			auto current_node = to_visit.front();
			to_visit.pop();
			if (position.DistanceTo)
			auto isLeft = path_finder(position,
		}
		return {};*/
	}

}