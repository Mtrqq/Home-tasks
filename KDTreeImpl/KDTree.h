#pragma once

#include "KDPoint.h"

#include <algorithm>
#include <vector>
#include <utility>

namespace nostd
{
	template <size_t DimensionsCount>
	class KDTree
	{
		struct Node
		{
			KDPoint<DimensionsCount> data;
			Node* left = nullptr, *right = nullptr;
		};
	public:
		KDTree(nullptr_t ptr = nullptr)
			:root{ ptr } {}

		KDTree(std::vector < KDPoint<DimensionsCount>> points)
		{
			root = createBalancedTree(std::move(points), 0, points.size() - 1);
		}

		class Iterator
		{
		public:
			Iterator(Node* node = nullptr);
			Iterator& operator++();
			KDPoint<DimensionsCount>& operator*();
			bool operator!=(const Iterator &another)
			{
				return currentNode != another.currentNode;
			}
		private:
			Node* currentNode;
		};

		Iterator begin() const
		{
			return Iterator{ root };
		}

		Iterator end() const
		{
			return Iterator{};
		}
	private:
		Node* root;

		Node* createBalancedTree(std::vector<KDPoint<DimensionsCount>> points, long left, long right);
	};

	template<size_t DimensionsCount>
	typename KDTree<DimensionsCount>::Node* KDTree<DimensionsCount>::createBalancedTree
	(std::vector<KDPoint<DimensionsCount>> points, long left, long right)
	{
		if (left <= right)
		{
			auto middle = left + (right - left) / 2;
			auto begin = points.begin() + left;
			auto end = points.begin() + right;
			std::nth_element(begin, points.begin() + middle, end, PointsComparator<DimensionsCount>{});
			Node* created_node = new Node{ points[middle],nullptr,nullptr };
			created_node->left = createBalancedTree(points, left, middle - 1);
			created_node->right = createBalancedTree(points, middle + 1, right);
			return created_node;
		}
		return nullptr;
	}

	template<size_t DimensionsCount>
	KDTree<DimensionsCount>::Iterator::Iterator(Node * node)
	{
		currentNode = node;
	}

	template<size_t DimensionsCount>
	typename KDTree<DimensionsCount>::Iterator& KDTree<DimensionsCount>::Iterator::operator++()
	{
		if (currentNode->left) currentNode = currentNode->left;
		else currentNode = currentNode->right;
		return *this;
	}

	template<size_t DimensionsCount>
	KDPoint<DimensionsCount>& KDTree<DimensionsCount>::Iterator::operator*()
	{
		return currentNode->data;
	}

}