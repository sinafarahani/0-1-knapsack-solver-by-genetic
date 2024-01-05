#include "01knapsack.h"

knapsack::knapsack(int n, int s)
	:
	numberOfItems(n),
	size(s)
{
	for (int i = 0; i < n; i++) {
		int v, b;
		std::cin >> v >> b;
		items.push_back(std::make_pair(v, b));
	}
}

std::vector<std::pair<int, int>> knapsack::getItems() const
{
	return items;
}

int knapsack::getSize() const
{
	return size;
}

int knapsack::getN() const
{
	return numberOfItems;
}
