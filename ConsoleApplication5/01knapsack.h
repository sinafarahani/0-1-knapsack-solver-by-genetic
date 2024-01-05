#pragma once

#include <vector>
#include <iostream>

class knapsack {
public:
	knapsack(int n, int s);
	std::vector<std::pair<int, int>> getItems() const;
	int getSize() const;
	int getN() const;
private:
	int size;
	int numberOfItems;
	std::vector<std::pair<int, int>> items;
};