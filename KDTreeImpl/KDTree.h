#pragma once

#include "KDPoint.h"
#include "KDRectangle.h"

#include <algorithm>
#include <vector>
#include <utility>
#include <stack>

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
			root = CreateBalancedTree(points, 0, static_cast<long>(points.size()));
		}

		~KDTree()
		{
			DestroyTree(root);
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

		bool HasPoint(const KDPoint<DimensionsCount>& point) const
		{
			return FindNode(point, root, PointsComparator<DimensionsCount>::GetComparator()) != nullptr;
		}

		void Insert(const KDPoint<DimensionsCount>& point)
		{
			if (!root) root = new Node{ point,nullptr,nullptr };
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

		KDPoint<DimensionsCount> GetClosestTo(const KDPoint<DimensionsCount>& position) const;

		Node* NearestNeighbor(const KDPoint<DimensionsCount>& destination,
			Node* currentBranch,
			Node* best,
			double best_distance,
			unsigned dimensionIndex) const;

		std::vector<KDPoint<DimensionsCount>> GetPointsInSection(const KDRectangle<DimensionsCount>& selection) const;
	private:
		Node* root;

		Node* CreateBalancedTree(std::vector<KDPoint<DimensionsCount>>& points, long left, long right, unsigned scoreIndex = 0);

		Node*& FindNode(const KDPoint<DimensionsCount>& point, Node*& currentNode, PointsComparator<DimensionsCount>& pathChooser);

		void TryInsert(const KDPoint<DimensionsCount>& point, Node* currentNode, PointsComparator<DimensionsCount>& pathChooser);

		Node* TryDelete(const KDPoint<DimensionsCount>& point, Node* currentNode, PointsComparator<DimensionsCount>& pathChooser);

		void SelectedPointsGatherer(const KDRectangle<DimensionsCount>& selection_area, const KDRectangle<DimensionsCount> current_selection,
			std::vector<KDPoint<DimensionsCount>>& output, Node* current_node, unsigned) const;

		template <typename Proccessor>
		void ForeachHelper(Proccessor function, Node* currentNode) const;

		Node* GetLargestValue(Node* currentNode) const;

		void DestroyTree(Node*);

	};

	template<size_t DimensionsCount>
	KDPoint<DimensionsCount> KDTree<DimensionsCount>::GetClosestTo(const KDPoint<DimensionsCount>& position) const
	{
		if (!root) throw std::runtime_error{ "Tree is empty, unable to start searching" };
		return NearestNeighbor(position, root, root, position.SquareDistance(root->data), 0)->data;
	}

	template<size_t DimensionsCount>
	typename KDTree<DimensionsCount>::Node*
		KDTree<DimensionsCount>::NearestNeighbor(const KDPoint<DimensionsCount>& destination,
			Node* currentBranch,
			Node* best,
			double best_distance,
			unsigned dimensionIndex) const
	{
		if (!currentBranch) return nullptr;

		KDPoint<DimensionsCount> current_point = currentBranch->data;

		auto distance = current_point.SquareDistance(destination);
		auto offset = current_point.At(dimensionIndex) - destination.At(dimensionIndex);

		Node* local_best = best;
		double local_best_distance = best_distance;

		if (distance < best_distance)
		{
			local_best = currentBranch;
			local_best_distance = distance;
		}

		Node* best_possible, * other;

		if (offset > 0)
		{
			best_possible = currentBranch->left;
			other = currentBranch->right;
		}
		else
		{
			best_possible = currentBranch->right;
			other = currentBranch->left;
		}

		dimensionIndex = (dimensionIndex + 1) % DimensionsCount;

		auto next_best = NearestNeighbor(destination, best_possible, local_best, local_best_distance, dimensionIndex);

		if (next_best != nullptr)
		{
			double distance = destination.SquareDistance(next_best->data);

			if (distance < local_best_distance)
			{
				local_best_distance = distance;
				local_best = next_best;
			}
		}

		if (offset * offset < local_best_distance)
		{
			next_best = NearestNeighbor(destination, other, local_best, local_best_distance, dimensionIndex);

			if (next_best != nullptr)
			{
				double distance = destination.SquareDistance(next_best->data);

				if (distance < local_best_distance)
				{
					local_best_distance = distance;
					local_best = next_best;
				}
			}
		}

		return local_best;
	}

	template<size_t DimensionsCount>
	std::vector<KDPoint<DimensionsCount>> KDTree<DimensionsCount>::GetPointsInSection(const KDRectangle<DimensionsCount>& selection) const
	{
		KDRectangle<DimensionsCount> initial_rectangle(KDVector<DimensionsCount>{std::numeric_limits<double>::min()},
			KDVector<DimensionsCount>{std::numeric_limits<double>::max()});
		std::vector<KDPoint<DimensionsCount>> result;
		SelectedPointsGatherer(selection, initial_rectangle, result, root, 0);
		return result;
	}



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
			currentNode->left = TryDelete(point, currentNode->left, ++pathChooser);
		}
		else
		{
			currentNode->right = TryDelete(point, currentNode->right, ++pathChooser);
		}

		return currentNode;
	}

	template<size_t DimensionsCount>
	void KDTree<DimensionsCount>::SelectedPointsGatherer(const KDRectangle<DimensionsCount>& selection_area, const KDRectangle<DimensionsCount> current_selection,
		std::vector<KDPoint<DimensionsCount>>& output, Node* current_node, unsigned axis) const
	{
		KDPoint<DimensionsCount> point = current_node->data;
		if (selection_area.Contains(point)) output.push_back(point);
		auto [left, right] = current_selection.SplitByAxis(axis, point.At(axis));
		if (current_node->left && selection_area.Overlap(left))
			SelectedPointsGatherer(selection_area, left, output, current_node->left, (axis + 1) % DimensionsCount);
		if (current_node->right && selection_area.Overlap(right))
			SelectedPointsGatherer(selection_area, right, output, current_node->right, (axis + 1) % DimensionsCount);
	}

	template<size_t DimensionsCount>
	typename  KDTree<DimensionsCount>::Node* KDTree<DimensionsCount>::GetLargestValue(Node* root) const
	{
		if (root->right) return GetLargestValue(root->right);
		else return root;
	}

	template<size_t DimensionsCount>
	void KDTree<DimensionsCount>::DestroyTree(Node* node)
	{
		if (node->left) DestroyTree(node->left);
		if (node->right) DestroyTree(node->right);
		delete node;
	}

	template<size_t DimensionsCount>
	template<typename Proccessor>
	void KDTree<DimensionsCount>::ForeachHelper(Proccessor function, Node* currentNode) const
	{
		if (currentNode->left) ForeachHelper(function, currentNode->left);
		function(currentNode->data);
		if (currentNode->right) ForeachHelper(function, currentNode->right);
	}

}