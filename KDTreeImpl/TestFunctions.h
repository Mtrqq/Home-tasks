#pragma once

#include "KDTree.h"
#include "KDPoint.h"
#include "KDRectangle.h"

#include <utility>
#include <vector>

constexpr unsigned tested_dimmension = 2;

std::pair<double, nostd::KDPoint<tested_dimmension>> FindNearestTree(const nostd::KDTree<tested_dimmension> & tree, const nostd::KDPoint<tested_dimmension> & to_find);

std::pair<double, nostd::KDPoint<tested_dimmension>> FindNearestBruteforce(const std::vector<nostd::KDPoint<tested_dimmension>> & pts, const nostd::KDPoint<tested_dimmension> & to_find);

std::pair<double, std::vector<nostd::KDPoint<tested_dimmension>>> PointsInSectorTree(const nostd::KDTree<tested_dimmension> & tree, const nostd::KDRectangle<tested_dimmension> & selection);

std::pair<double, std::vector<nostd::KDPoint<tested_dimmension>>> BruteForcePtsInSector(const std::vector<nostd::KDPoint<tested_dimmension>> & pts, const nostd::KDRectangle<tested_dimmension> & selection);

void TestNPSearch(const std::vector<nostd::KDPoint<tested_dimmension>> & points, unsigned tests_count);

void TestRangeSearch(const std::vector<nostd::KDPoint<tested_dimmension>> & points, unsigned tests_count);