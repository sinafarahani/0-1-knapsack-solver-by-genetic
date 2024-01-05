#include "genetic.h"

genetic::genetic(knapsack k)
	:
	k(k)
{
	if(k.getN() < 9)
		initiPopulation = std::pow(2, k.getN() - 1);
	else {
		initiPopulation = 500;
	}
	generation.reserve(initiPopulation);
	for (int i = 0; i < initiPopulation; ++i) {
		ItemSelection item;
		item.chromosome.reserve(k.getN());
		for (int j = 0; j < k.getN(); ++j) {
			int r = getRandomInt(1);
			item.chromosome.push_back(r == 1);
		}
		item.fitness = calc_fitness(item);
		if (std::find(generation.begin(), generation.end(), item) == generation.end()) {
			generation.push_back(item);
		}
	}
}

void genetic::start(int i, std::mutex &mtx)
{
	int Count = 0;
	while (!stop(Count))
	{
		reproduce();
		generation.clear();
		generation = nextGeneration;
		nextGeneration.clear();
		Count++;
		for (auto& item : generation)
		{
			item.fitness = calc_fitness(item);
		}
	}
	mtx.lock();
	std::cout << "Test #" << i << ": " << get_fittest().fitness << " and took: " << steps << " steps." << std::endl;
	mtx.unlock();
}

int genetic::getRandomInt(int r)
{
	std::random_device rr;
	XoshiroCpp::Xoshiro256Plus rnd(rr());
	std::uniform_int_distribution<int> dist(0, r);
	return dist(rnd);
}

int genetic::calc_fitness(ItemSelection item)
{
	int total_benefit = 0, total_volume = 0;
	for (int i = 0; i < k.getN(); ++i) {
		if (item.chromosome[i]) {
			total_volume += k.getItems()[i].first;
			total_benefit += k.getItems()[i].second;
		}
	}
	if (total_volume > k.getSize()) {
		return 0;
	}
	else {
		return total_benefit;
	}
}

genetic::ItemSelection genetic::get_fittest()
{
	genetic::ItemSelection fittest;
	for (const auto& item : generation) {
		if (fittest < item) {
			fittest = item;
		}
	}
	maxFitnessesOverGenerations.push_back(fittest.fitness);
	return fittest;
}

genetic::ItemSelection genetic::get_second_fittest(genetic::ItemSelection fittest)
{
	genetic::ItemSelection second_fittest;
	for (const auto& item : generation) {
		if (item != fittest && second_fittest < item) {
			second_fittest = item;
		}
	}
	return second_fittest;
}

void genetic::elitism()
{
	auto fittest = get_fittest();
	nextGeneration.push_back(fittest);
	auto second_fittest = get_second_fittest(fittest);
	nextGeneration.push_back(second_fittest);
}

genetic::ItemSelection genetic::roulette_wheel_selection()
{
	int totalfitness = 0;
	for (const auto& item : generation) {
		totalfitness += item.fitness;
	}
	int r = getRandomInt(totalfitness - 1), sum = 0;
	for (auto item : generation) {
		sum += item.fitness;
		if (sum > r) {
			//if(generation.size() > 10)
				//generation.erase(std::find(generation.begin(), generation.end(), item));
			return item;
		}
	}
}

void genetic::mutation(std::vector<bool>& chromosome)
{
	int i = getRandomInt(chromosome.size() - 1);
	chromosome[i] = !chromosome[i];
}

void genetic::crossover(std::vector<bool>& chromosome1, std::vector<bool>& chromosome2)
{
	int r = getRandomInt(chromosome1.size() - 1);
	for (int i = 0; i < r; ++i)
		swap(chromosome1[i], chromosome2[i]);
}

void genetic::reproduce()
{
	// Elitism - two of the fittest chromosomes are copied without changes to a new population
	elitism();
	while (nextGeneration.size() < initiPopulation) {
		auto item1 = roulette_wheel_selection();
		auto item2 = roulette_wheel_selection();
		crossover(item1.chromosome, item2.chromosome);
		nextGeneration.push_back(item1);
		nextGeneration.push_back(item2);
		int r = getRandomInt(100);
		if (r < muRate) {
			mutation(item1.chromosome);
			nextGeneration.push_back(item1);
		}
		r = getRandomInt(100);
		if (r < muRate) {
			mutation(item2.chromosome);
			nextGeneration.push_back(item2);
		}
	}
	steps++;
}

bool genetic::stop(int generationsCount)
{
	if (k.getN() < 9) {
		if (generationsCount > 15)
			return maxFitnessesOverGenerations[generationsCount - 11] == maxFitnessesOverGenerations[generationsCount - 2];
		else return false;
	}
	else if (k.getN() < 20) {
		if (generationsCount > 105)
			return maxFitnessesOverGenerations[generationsCount - 101] == maxFitnessesOverGenerations[generationsCount - 2];
		else return false;
	}
	else {
		if (generationsCount > std::pow(k.getN(), 2) / 4 + 5)
			return maxFitnessesOverGenerations[generationsCount - (std::pow(k.getN(), 2) / 4 + 1)] == maxFitnessesOverGenerations[generationsCount - 2];
		else return false;
	}
}
