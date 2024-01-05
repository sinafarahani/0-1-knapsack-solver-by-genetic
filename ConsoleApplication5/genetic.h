#pragma once

#include <iostream>
#include <unordered_map>
#include <vector>
#include <random>
#include <mutex>

#include "XoshiroCpp.hpp"
#include "01knapsack.h"

class genetic {
public:
	genetic(knapsack k);
	void start(int i, std::mutex& mtx);
private:
	class ItemSelection
	{
	public:
		bool operator<(ItemSelection item) {
			return this->fitness < item.fitness;
		}
		bool operator==(ItemSelection item) {
			return this->chromosome == item.chromosome;
		}
		std::vector<bool> chromosome;
		int fitness = 0;
	};
	int getRandomInt(int r);
	int calc_fitness(ItemSelection item);
	ItemSelection get_fittest();
	ItemSelection get_second_fittest(ItemSelection fittest);
	void elitism();
	ItemSelection roulette_wheel_selection();
	void mutation(std::vector<bool>& chromosome);
	void crossover(std::vector<bool>& chromosome1, std::vector<bool>& chromosome2);
	void reproduce();
	bool stop(int generationsCount);

	int initiPopulation;
	const int muRate = 50;
	int steps = 0;
	knapsack k;
	std::vector<ItemSelection> generation;
	std::vector<ItemSelection> nextGeneration;
	std::vector<int> maxFitnessesOverGenerations;
};