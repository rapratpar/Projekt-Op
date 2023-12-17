#pragma once
#include <random>
#include <iostream>
#include <vector>
#include <cstdio>
#include <cmath>
#include <algorithm>
#include <iterator>
#include <utility>

#include <chrono>
#include <thread>
using namespace std;

random_device devg;
default_random_engine rngg(devg());
uniform_real_distribution<float> uniform_dist_Floats(0.0, 1.0);
uniform_int_distribution<int> chose_vertex(0, 1);


void create_member(int vertex_num, vector<vector<int>>& population);
void genetic(int** graph, int vertex_num, vector<int> energy, vector<int>& final_solution, vector<int>& cover_solution);
int fitness(int** graph, vector<int> cover, int vertex_num, vector<int> member);
void member_cover(int** graph, vector<int> member, int vertex_num, vector<int> energy, vector<vector<int>>& cover_population, vector<vector<vector<int>>>& energy_population, int c_index);
void crossover(int** graph, int vertex_num, vector<int> member_1, vector<int> member_2, vector<int> energy, vector<vector<int>>& population, vector<vector<int>>& cover_population, vector<vector<vector<int>>>& energy_population, int c_index);
vector<vector<int>> turniej(int** graph, int population_size, int vertex_num, vector<vector<int>>& cover_population, vector<vector<vector<int>>>& energy_population, vector<vector<int>>& population);
void szufla(vector<vector<int>>& cover_population, vector<vector<vector<int>>>& energy_population, vector<vector<int>>& population, int population_size);
void add_vertex(vector<int>& member, vector<int>& coverd, int vertex_num, int position, int** graph, vector<vector<int>> energy);
void delte_vertex(vector<int>& member, vector<int>& coverd, vector<vector<int>>& energy, int vertex_num, int vertex);
void delete_cover(vector<int>& member, vector<int>& coverd, vector<vector<int>>& energy, int vertex_num, int vertex, int** graph);
void mutation(vector<int>& member, vector<int>& coverd, vector<vector<int>>& energy, float probality, int vertex_num, int** graph);

void genetic(int** graph, int vertex_num, vector<int> energy, vector<int>& final_solution, vector<int>& cover_solution) {

	int populations_size = 50;
	vector<vector<int>> population;
	vector<vector<int>> cover_population;
	vector<vector<vector<int>>> energy_population;
	for (int i = 0;i < populations_size;i++) {
		create_member(vertex_num, population);
		member_cover(graph, population[i], vertex_num, energy, cover_population, energy_population, -1);
	};

	int best_fitness = fitness(graph, cover_population[0], vertex_num, population[0]);
	vector<int> fittest_individual = population[0];
	vector<int> fittest_cover = cover_population[0];


	int gen = 0;
	while (gen != 1000) {

	best_fitness = fitness(graph, cover_population[0], vertex_num, population[0]);
	fittest_individual = population[0];

	for (int i = 0;i < populations_size;i++) {
		int f = fitness(graph, cover_population[i], vertex_num, population[i]);
		if (f < best_fitness)
		{
			best_fitness = f;
			fittest_individual = population[i];
			fittest_cover = cover_population[i];
		}
	}
	/*
	if (gen % 100 == 0)
	{
		cout << "Generation: " << gen << " Best Fitness: " << best_fitness << " Individual: " << endl;
		for (int x = 0; x < vertex_num; x++) {
			cout << fittest_individual[x] << " ";
		};
		cout << endl;

	}*/
	gen++;
	population = turniej(graph, populations_size, vertex_num, cover_population, energy_population, population);
	/*for (int i = 0;i < populations_size;i++) {

		cout << "Member " << i << ": " << endl;
		for (int j = 0;j < vertex_num;j++) {
			cout << population[i][j] << " ";
		}
		cout << endl;
	}
	*/
	for (int i = 0; i < populations_size - 1; i += 2)
	{
		crossover(graph, vertex_num, population[i], population[i + 1], energy, population, cover_population, energy_population, i);
	}
	//cout << "Po turnieju" << endl;

	for (int i = 0;i < populations_size;i++) {
		//cout << "XD" << endl;

		if (gen < 400)
		{
			mutation(population[i], cover_population[i], energy_population[i], 0.4, vertex_num, graph);
		}
		else
		{
			mutation(population[i], cover_population[i], energy_population[i], 0.2, vertex_num, graph);
		}
	}
	}	;
	/*auto stopG = high_resolution_clock::now();
	auto durationG = duration_cast<microseconds>(stopG - startG);
	std::cout << "time: " << durationG.count() << std::endl
	*/
	//for (int i = 0; i < vertex_num; i++)
	//{
		//	cout << fittest_individual[i] << " ";
	//}
	//std::cout << std::endl;
	//cout << fitness(graph, fittest_individual, vertex_num, fittest_individual) << endl;

	final_solution = fittest_individual;
	cover_solution = fittest_cover;
};


void create_member(int vertex_num, vector<vector<int>>& population) {
	vector<int> member;
	for (int i = 0;i < vertex_num;i++) {
		member.push_back(chose_vertex(rngg));
	};

	/*cout << "Member: ";
	for (int i = 0;i < vertex_num;i++) {
		cout << member[i] << " ";
	};
	cout << endl;
	*/
	population.push_back(member);
};

int fitness(int** graph, vector<int> cover, int vertex_num, vector<int> member) {
	int fitnessScore = 0;
	for (int i = 0; i < vertex_num; i++) {
		if (member[i] == 1)
			fitnessScore++;
		for (int j = 0; j < i; j++) {
			if (graph[i][j] == 1)
				if (cover[i] == 0 && cover[j] == 0)
					fitnessScore += 10;
		}
	}
	return fitnessScore;
};

void member_cover(int** graph, vector<int> member, int vertex_num, vector<int> energy, vector<vector<int>>& cover_population, vector<vector<vector<int>>>& energy_population, int c_index) {
	vector<vector<int>> member_energy;
	vector<int> coverd;
	vector<int> temp_energy;
	vector <int> dummy(1, 0);
	//cout << "Pokrywam!" << endl;
	temp_energy = energy;


	//cout << "Wejsciowy member : " << endl;
	for (int i = 0; i < vertex_num; i++) {
		//cout << member[i] << " ";
		coverd.push_back(0);
	};

	//cout << member[0] << endl;

	for (int i = 0; i < vertex_num;i++) {
		member_energy.push_back(dummy);
		member_energy[i][0] = energy[i];
		if (member[i] == 1) {
			member_energy[i][0] = 0;
			member_energy[i].push_back(i);
			member_energy[i].push_back(energy[i]);
			coverd[i] = 1;
		};
	};
	//cout << "He" << endl;
	for (int i = 0;i < vertex_num;i++) {
		//cout << "Vertex: " << i << endl;
		if (member[i] == 1) {
			//cout << "czy wszelo?" << endl;
			vector<int> nastepnicy;
			coverd[i] = 1;
			int left_energy = 15 - temp_energy[i];


			//cout << "dodano pokrycie sprawdzanego wierzcholka" << endl;
			for (int j = 0; j < vertex_num;j++) {
				if (graph[i][j] == 1 && member[j] == 0 && coverd[j] == 0) {
					nastepnicy.push_back(j);
				};
			};
			//cout << "czy wszelo2?" << endl;
			if (nastepnicy.size() == 0) continue;
			//cout << "size: " << nastepnicy.size() << endl;

			if (left_energy == 0) {
				continue;
			};
			uniform_int_distribution<int> chose_next(0, nastepnicy.size() - 1);
			//cout << "sprawdzam" << endl;
			int next_vertex;


			next_vertex = nastepnicy[chose_next(rngg)];
			if (member_energy[next_vertex][0] == left_energy) {
				coverd[next_vertex] = 1;
				//cout << "Przekazano i wyzereowano wierzcholek: " << next_vertex << endl;
				member_energy[next_vertex][0] = 0;
				//Dodajemy informqcje ktroy wierzcholek pokrywa, i ile energi przekazal
				member_energy[next_vertex].push_back(i);
				member_energy[next_vertex].push_back(left_energy);
				continue;
			};
			if (member_energy[next_vertex][0] > left_energy && member_energy[next_vertex][0] != 0) {
				member_energy[next_vertex][0] = member_energy[next_vertex][0] - left_energy;


				//Dodajemy informqcje ile energi przekazal wierzcholek
				member_energy[next_vertex].push_back(i);
				member_energy[next_vertex].push_back(left_energy);
				continue;
			};
			if (member_energy[next_vertex][0] < left_energy && member_energy[next_vertex][0] != 0) {

				coverd[next_vertex] = 1;


				//Dodajemy informqcje ktroy wierzcholek pokrywa
				member_energy[next_vertex].push_back(i);
				member_energy[next_vertex].push_back(left_energy - member_energy[next_vertex][0]);
				left_energy = left_energy - member_energy[next_vertex][0];
				member_energy[next_vertex][0] = 0;


				if (nastepnicy.size() == 1) continue;

				int next_vertex2 = next_vertex;
				while (next_vertex2 == next_vertex) {
					next_vertex2 = nastepnicy[chose_next(rngg)];
				};
				if (member_energy[next_vertex2][0] == left_energy) {
					coverd[next_vertex2] = 1;

					//Dodajemy informqcje ktroy wierzcholek pokrywa
					member_energy[next_vertex2].push_back(i);
					member_energy[next_vertex2].push_back(left_energy);
					member_energy[next_vertex2][0] = 0;

					continue;
				};
				if (member_energy[next_vertex2][0] > left_energy) {

					member_energy[next_vertex2][0] = member_energy[next_vertex2][0] - left_energy;
					member_energy[next_vertex2].push_back(i);
					member_energy[next_vertex2].push_back(left_energy);


					continue;
				};

			};
		}
	};
	/*cout << "Pokrycie: ";
	for (int i = 0;i < vertex_num;i++) {
		cout << coverd[i] << " ";
	};
	cout << endl;
	cout << "Energia: ";
	for (int i = 0;i < vertex_num;i++) {
		cout << member_energy[i][0] << " ";
	};
	cout << endl;
	*/
	if (c_index == -1) {
		cover_population.push_back(coverd);
		energy_population.push_back(member_energy);
	};
	if (c_index >= 0) {
		cover_population[c_index] = coverd;
		energy_population[c_index] = member_energy;
	};
	//cout << endl;
}

void crossover(int** graph, int vertex_num, vector<int> member_1, vector<int> member_2, vector<int> energy, vector<vector<int>>& population, vector<vector<int>>& cover_population, vector<vector<vector<int>>>& energy_population, int c_index) {
	vector<int> children_1(vertex_num, 0 );
	vector<int> children_2(vertex_num, 0 );


	//for (int i = 0; i < vertex_num;i++) {
		//cout << children_1[i] << " ";
	//};
	//cout << endl;
	//cout << "vertex num: " << vertex_num << endl;
	uniform_int_distribution<int> uniform_dist_1_n_3(1, vertex_num - 3);
	int position = uniform_dist_1_n_3(rngg);
	//cout << "Przed dizeciach" << endl;
	//cout << "Position: " << position << endl;

  for (int i = 0; i < position; i++) {

		//cout << i << " ";
		children_1[i] = member_1[i];
		children_2[i] = member_2[i];
	}
	//cout << "ddd" << endl;
	for (int i = position + 1; i < vertex_num; i++) {
		//cout << i << " ";

		children_1[i] = member_1[i];
		children_2[i] = member_2[i];
	};
	/*cout << "Dziecko 1: " << endl;
	for (int i = 0; i < vertex_num; i++) {
		cout << children_1[i] << " ";
	};
	cout << endl;
	cout << "Dziecko 2: " << endl;
	for (int i = 0; i < vertex_num; i++) {
		cout << children_2[i] << " ";
	};
	cout << endl;*/
	//cout << "Po dizeciach" << endl;
	population[c_index] = children_1;
	population[c_index+1] = children_2;

	//cout << "Pokrycie dziecka 1: " << endl;
	member_cover(graph, population[c_index], vertex_num, energy, cover_population, energy_population, c_index);

	//cout << "Pokrycie dziecka 2: " << endl;

	member_cover(graph, population[c_index+1], vertex_num, energy, cover_population, energy_population, c_index + 1);

}

vector<vector<int>> turniej(int** graph, int population_size, int vertex_num, vector<vector<int>>& cover_population, vector<vector<vector<int>>>& energy_population, vector<vector<int>>& population) {
	vector<vector<int>> new_population;
	int szufel_range;
	szufel_range = population_size;
	for (int j = 0; j < 2; j++) {
		szufla(cover_population, energy_population, population, szufel_range);
		for (int i = 0; i < szufel_range - 1; i += 2) {
			if (fitness(graph, cover_population[i], vertex_num, population[i]) < fitness(graph, cover_population[i + 1], vertex_num, population[i + 1])) {
				new_population.push_back(population[i]);
			}
			else {
				new_population.push_back(population[i + 1]);
			};
		};
		if (population_size % 2 == 1) new_population.push_back(population[population_size - 1]);

	};


	szufla(cover_population, energy_population, new_population, szufel_range);
	return new_population;
};


void szufla(vector<vector<int>>& cover_population, vector<vector<vector<int>>>& energy_population, vector<vector<int>>& population, int population_size) {
	uniform_int_distribution<int> szufler(0, population_size-1);
	int swap_1,swap_2;
	vector<int> temp_memebr;
	vector<int> temp_cover;
	vector<vector<int>> temp_energy;


	for (int i = 0; i < population_size;i++) {
		swap_1 = szufler(rngg);
		swap_2 = szufler(rngg);
		temp_memebr = population[swap_1];
		temp_cover = cover_population[swap_1];
		temp_energy = energy_population[swap_1];

		population[swap_1] = population[swap_2];
		cover_population[swap_1] = cover_population[swap_2];
		energy_population[swap_1] = energy_population[swap_2];
		population[swap_2]= temp_memebr;
		cover_population[swap_2]= temp_cover;
		energy_population[swap_2]= temp_energy;
	};
}

void add_vertex(vector<int>& member, vector<int>& coverd, int vertex_num, int position, int** graph, vector<vector<int>> energy) {
	member[position] = 1;
	vector<int> nastepnicy;
	int left_energy;
	left_energy = 15 - energy[position][0];
	coverd[position] = 1;

	//cout << "add vertex 1" << endl;
	for (int i = 0; i < vertex_num; i++) {
		if (graph[position][i] == 1 && coverd[i] == 0 && member[i] == 0) {
			nastepnicy.push_back(i);
		}
	}
	//cout << "add vertex 2" << endl;

	int nas_size = nastepnicy.size();
	if (nas_size != 0 && left_energy != 0) {
		uniform_int_distribution<int> chose_next(0, nastepnicy.size() - 1);
		int ind = chose_next(rngg);
		int next_vertex = nastepnicy[ind];
		//cout << "add vertex 3" << endl;

		if (energy[next_vertex][0] == left_energy) {
			//cout << "Ta sama energia" << endl;

			coverd[next_vertex] = 1;
			energy[next_vertex][0] = 0;
			energy[next_vertex].push_back(position);
			energy[next_vertex].push_back(left_energy);

			//cout << "Po Ta sama energia" << endl;
		}
		else if (energy[next_vertex][0] > left_energy) {
			//cout << "Wieskza energia" << endl;

			energy[next_vertex][0] = energy[next_vertex][0] - left_energy;
			energy[next_vertex].push_back(position);
			energy[next_vertex].push_back(left_energy);
			//cout << "Po Wieskza energia" << endl;

		}
		else if (left_energy > energy[next_vertex][0] && energy[next_vertex][0] != 0) {
			//cout << "Wieskza left energia" << endl;

			energy[next_vertex].push_back(position);
			energy[next_vertex].push_back(energy[next_vertex][0]);
			coverd[next_vertex] = 1;
			left_energy = left_energy - energy[next_vertex][0];
			energy[next_vertex][0] = 0;
			//cout << "maybe tutaj" << endl;
			if (nas_size == 1) return;

			nastepnicy.erase(nastepnicy.begin() + ind);
			nas_size--;
			//cout << "maybe tutaj 2" << endl;
			uniform_int_distribution<int> chose_next(0, nas_size - 1);
			ind = chose_next(rngg);
			next_vertex = nastepnicy[ind];
			//cout << "przed kolejnym wierzcholkiem" << endl;
				if (left_energy == energy[next_vertex][0]) {
					coverd[next_vertex] = 1;
					energy[next_vertex][0] = 0;
					energy[next_vertex].push_back(position);
					energy[next_vertex].push_back(left_energy);


				}
				else if (left_energy < energy[next_vertex][0]) {
					energy[next_vertex].push_back(position);
					energy[next_vertex].push_back(left_energy);
					energy[next_vertex][0] = energy[next_vertex][0] - left_energy;
			};
		};
	};
};

void delte_vertex(vector<int>& member, vector<int>& coverd, vector<vector<int>>& energy, int vertex_num, int vertex) {
	member[vertex] = 0;
	coverd[vertex] = 0;
	for (int i = 0; i < vertex_num; i++) {
		for (int j = 1;j < energy[i].size() - 1; j += 2) {
			if (energy[i][j] == vertex) {
				if (energy[i][0] == 0 && coverd[energy[i][j]] == 1) {
					coverd[energy[i][j]] = 0;
				};
				energy[i][0] = energy[i][j + 1];
				energy[i].erase(energy[i].begin() + j);
				energy[i].erase(energy[i].begin() + j);
				break;
			};
		};
	}
};

void delete_cover(vector<int>& member, vector<int>& coverd, vector<vector<int>>& energy, int vertex_num, int vertex, int**graph) {

	// DODAC LOOPE BO MOZE POKRYWAC WIECEJ WIERZCHOLKOW;
	// ZMIENIC WSZYSTKIE MOZLIWE;
	// JAK NIE MA TO ZOSTAWAIMY
	
	for (int j = 1; j < energy[vertex].size(); j += 2) {
		

		int change_vertex = energy[vertex][j];
		vector<int> nastepnicy;

		//cout << "HEHE" << endl;

		for (int i = 0;i < vertex_num;i++) {
			if (graph[change_vertex][i] == 1 && coverd[change_vertex] == 0 && member[change_vertex] == 0) {
				nastepnicy.push_back(i);
			};
		};

		if (nastepnicy.size() == 0) continue;

		int left_energy = energy[vertex][j + 1];

		energy[vertex][0] = energy[vertex][0] + energy[vertex][j+1];
		energy[vertex].erase(energy[vertex].begin() + j);
		energy[vertex].erase(energy[vertex].begin() + j);

		//cout << "HEHE" << endl;
		int nas_size = nastepnicy.size();
		if (nas_size != 0 && left_energy != 0) {
			uniform_int_distribution<int> chose_next(0, nastepnicy.size() - 1);
			int ind = chose_next(rngg);
			int next_vertex = nastepnicy[ind];
			if (energy[next_vertex][0] == left_energy) {
				coverd[next_vertex] = 1;
				energy[next_vertex][0] = 0;
				energy[next_vertex].push_back(change_vertex);
				energy[next_vertex].push_back(left_energy);

			}
			else if (energy[next_vertex][0] > left_energy) {
				energy[next_vertex][0] = energy[next_vertex][0] - left_energy;
				energy[next_vertex].push_back(change_vertex);
				energy[next_vertex].push_back(left_energy);
			}
			else if (left_energy > energy[next_vertex][0] && energy[next_vertex][0] != 0) {
				energy[next_vertex].push_back(change_vertex);
				energy[next_vertex].push_back(energy[next_vertex][0]);
				coverd[next_vertex] = 1;
				left_energy = left_energy - energy[next_vertex][0];
				energy[next_vertex][0] = 0;

				if (nas_size == 1) return;

				nastepnicy.erase(nastepnicy.begin() + ind);
				nas_size--;

				ind = chose_next(rngg);
				next_vertex = nastepnicy[ind];
				if (left_energy == energy[next_vertex][0]) {
					coverd[next_vertex] = 1;
					energy[next_vertex][0] = 0;
					energy[next_vertex].push_back(change_vertex);
					energy[next_vertex].push_back(left_energy);


				}
				else if (left_energy < energy[next_vertex][0]) {
					energy[next_vertex].push_back(change_vertex);
					energy[next_vertex].push_back(left_energy);
					energy[next_vertex][0] = energy[next_vertex][0] - left_energy;
				};
			};
		};
	};
};

void mutation(vector<int>& member, vector<int>& coverd, vector<vector<int>>& energy, float probality, int vertex_num, int** graph) {
	
	uniform_int_distribution<int> uniform_dist_0_n_1(1, vertex_num - 3);
	float x = uniform_dist_Floats(rngg);
	//cout << "W mutacji" << endl;

	if (x <= probality) {
		int position = uniform_dist_0_n_1(rngg);
		if (coverd[position] == 1 && member[position] == 0)
		{
			//cout << "Przed delete cover" << endl;

			delete_cover(member, coverd, energy, vertex_num, position, graph);
			//cout << "Po delete cover" << endl;

		}
		else if (coverd[position] == 1 && member[position] == 1)
		{
			//cout << "Przed delete vertex" << endl;

			delte_vertex(member, coverd, energy, vertex_num, position);
			//cout << "Po delete vertex" << endl;

		}
		else if (coverd[position] == 0 && member[position] == 0) {
			//cout << "Przed add cover" << endl;

			add_vertex(member, coverd, vertex_num, position, graph, energy);
			//cout << "Po add cover" << endl;

		};
	}
};