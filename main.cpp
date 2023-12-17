#include <random>
#include <iostream>
#include <vector>
#include <cstdio>
#include <cmath>
#include <algorithm>
#include <iterator>
#include <utility>

#include "brutal.h"
#include"genetyczny.h"
#include"zachlanny.h"
#include "graph_operations.h"

#include "nlohmann/json.hpp"
#include "nlohmann/json_fwd.hpp"

using namespace std;

int main() {

	int i, e, n;
	cout << "Random graph generation: " << endl;

	n = 8;
	cout << "The graph has " << n << " vertices" << endl;

	e = n*(n-1)/2;
	e = e * 5 / 10;
	cout << "and has " << e << " edges." << endl;

	// Function call 
	generate_graph(e, n);


	return 0;



	
};





/*
void brute_force(vector<vector<int>> edges, int vertex_num, int edges_num, int** grpah, vector<int> energy) {

	int best_cover = vertex_num; // Pocz¹tkowo najogrsze
	string final_solution;
	string combination;

	vector<vector<int>> dummy_edges;
	int left_edges;

	dummy_edges = edges;
	
	int temp_left;
	
	int all_pos = pow(2, vertex_num);
	int curr_pos = 0;

	while (curr_pos < all_pos) {

		combination = bin(curr_pos, vertex_num);
		curr_pos += 1;
		dummy_edges = edges;
		vector<int> coverd_vertexes;
		left_edges = edges_num;

		// cout << "Testowanie rozwiazani: " << combination << endl;
		for (int i = 0; i < vertex_num;i++) {
			temp_left = left_edges;
			// cout << "Pozostale krawedzie: " << temp_left << endl;
			if (combination[i] == '1') {
				coverd_vertexes.push_back(i);
				//     cout << "Dodawny wierzcholek: " << i << endl;
				int curr = 0;
				if (energy[i] == 15) {
					for (int j = 0;j < left_edges;j++) {
						if (dummy_edges[j][0] == i || dummy_edges[j][1] == i) {
							// cout << "Usuwam: " << dummy_edges[j][0] << " " << dummy_edges[j][1] << endl;
							temp_left -= 1;
						}
						else {
							dummy_edges[curr][0] = dummy_edges[j][0];
							dummy_edges[curr][1] = dummy_edges[j][1];
							curr += 1;
						};
					};
				};
			};
			left_edges = temp_left;
		};
		if (left_edges == 0) {
			cout << endl;
			cout << "Graf zostal pokryty!" << endl;
			cout << "Kombinacja: " << combination << endl;

			if (coverd_vertexes.size() < best_cover) {
				best_cover = coverd_vertexes.size();
				final_solution = combination;
			};
		};
	};

	cout << "Minimalna ilosc wierzcholkow: " << best_cover << endl;
	cout << "Kombinacja: " << final_solution << endl;
};
*/





/*
void genetic(int** graph, int vertex_num, int edges_num) {
};
*/

/*
void new_brute(int** graph, int vertex_num) {


	string combination;
	int all_pos = pow(2, vertex_num);
	int curr_pos = 0;

	while (curr_pos < all_pos) {

		combination = bin(curr_pos, vertex_num);
		curr_pos += 1;

		for (int i = 0; i < vertex_num;i++) {
			if (combination[i] == 1) {
				for (int j = 0; j < vertex_num;j++) {
					if (graph[i][j] == 1) {

					}
				};
			};
		};

};
	*/


/*
vector<int> create_member(int vertex_num) {
	vector<int> member;
	for (int i = 0;i < vertex_num;i++) {
		member.push_back(chose_vertex(e1));
	};

	cout << "Member: ";
	for (int i = 0;i < vertex_num;i++) {
		cout << member[i] << " ";
	};
	cout << endl;
	return member;
};


int fitness(int** graph, vector<vector<int>> member, int vertex_num) { 
	int fitnessScore = 0;
	for (int i = 0; i < vertex_num; i++) {
		if (member[i][0] == 1)
			fitnessScore++;
		for (int j = 0; j < i; j++) {
			if (graph[i][j] == 1)
				if (member[i][0] == 0 && member[j][0] == 0)
					fitnessScore += 10;
		}
	}
	return fitnessScore;
};

vector <vector<int>> member_cover(int** graph, vector<int> member, int vertex_num, vector<int> energy) {
	vector<vector<int>> member_energy;
	vector<int> coverd;
	vector<int> temp_energy;
	vector <int> dummy(1, 0);
	//cout << "Pokrywam!" << endl;
	temp_energy = energy;


	cout << "Wejsciowy member : " << endl;
	for (int i = 0; i < vertex_num; i++) {
		cout << member[i] << " ";
	};
	cout << endl;
	//cout << member[0] << endl;

	for (int i = 0; i < vertex_num;i++) {
		member_energy.push_back(dummy);
		member_energy[i][0] = energy[i];
	};

	for (int i = 0; i < vertex_num;i++) {
		member_energy.push_back(dummy);
		member_energy[i][0] = energy[i];
		if (member[i] == 1) {
			member_energy[i][0] = 0;
			member_energy[i].push_back(i);
			member_energy[i].push_back(energy[i]);
		};
	};

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


			next_vertex = nastepnicy[chose_next(e1)];
			if (member_energy[next_vertex][0] == left_energy) {
				cout << "Przekazano i wyzereowano wierzcholek: " << next_vertex << endl;
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
					next_vertex2 = nastepnicy[chose_next(e1)];
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

}



void mutation(vector<int> member, vector<vector<int>> coverd,  float probality, int vertex_num, int** graph, vector<int> energy) {
	std::uniform_int_distribution<int> uniform_dist_0_n_1(1, vertex_num - 3);
	float x = uniform_dist_Floats(e1);
	if (x <= probality) {
		int position = uniform_dist_0_n_1(e1);
		if (coverd[position][0] == 1 && coverd[position].size() != 1)
		{
			delte_vertex(member, coverd, vertex_num, coverd[position][1]);
		};
		if (coverd[position][0] == 1 && coverd[position].size() == 1)
		{
			delte_vertex(member, coverd, vertex_num, position);
		};
		if (coverd[position][0] == 1) {
			add_vertex(member, coverd, vertex_num, position, graph, energy);
		};
	}
}

void delte_vertex( vector<int> member, vector<vector<int>> coverd, int vertex_num, int position) {
	for (int i = 0; i < vertex_num; i++) {
		if (coverd[position][0] == 1 && coverd[position].size() != 1 && coverd[position][1] == position) {
			coverd[i].clear();
			coverd[i].push_back(0);
		};
	};
	coverd[position].clear();
	coverd[position].push_back(0);
	member[position] = 0;
};

void add_vertex(vector<int>& member, vector<int>& coverd, int vertex_num, int position, int** graph, vector<vector<int>> energy) {
	member[position] = 1;
	vector<int> nastepnicy;
	int left_energy;
	left_energy = 15 - energy[position][0];
	coverd[position] = 1;
	for (int i = 0; i < vertex_num; i++) {
		if (graph[position][i] == 1) {
			nastepnicy.push_back(i);
		}
	}
	if (nastepnicy.size() != 0 && left_energy != 0) {
		uniform_int_distribution<int> chose_next(0, nastepnicy.size() - 1);
		int next_vertex = nastepnicy[chose_next(e1)];
		if (energy[next_vertex] == left_energy) {
			coverd[next_vertex][0] = 1;
			coverd[next_vertex].push_back(position);
		}
		else if (energy[next_vertex] > left_energy) {
			energy[next_vertex] = energy[next_vertex] - left_energy;
		}
		else if (left_energy > energy[next_vertex] && energy[next_vertex] !=0) {
			left_energy = left_energy - energy[next_vertex];
			energy[next_vertex] = 0;
			if (nastepnicy.size() != 1) {
				int next_vertex2 = next_vertex;
				while (next_vertex == next_vertex2) {
					next_vertex2 = nastepnicy[chose_next(e1)];
				};
				if (left_energy == energy[next_vertex2]) {
					coverd[next_vertex2][0] = 1;
					coverd[next_vertex2].push_back(position);

				}else if(left_energy < energy[next_vertex2]) {
					energy[next_vertex2] = energy[next_vertex2] - left_energy;
				};
			};
		};
	};
};


pair<vector<int>, vector<int>> crossover(int vertex_num, vector<int> member_1, vector<int> member_2, vector<vector<int>> coverd_1, vector<vector<int>> cover_2, vector<int> energy) {
	vector<int> childre_1{ vertex_num, 0 };
	vector<int> children_1_energy;
	vector<vector<int>> children_1_cover;
	children_1_energy = energy;




	vector<int> childre_2{ vertex_num, 0 };
	vector<int> children_2_energy;
	children_2_energy = energy;

	uniform_int_distribution<int> uniform_dist_1_n_3(1, vertex_num - 3);
	int position = uniform_dist_1_n_3(e1);


	for (int i = 0; i < position + 1; i++) {

		/*
		if (member_1[i] == 1) {
			int left_energy = 15 - energy[i];
			children_1_energy[i] = 0;
			children_1_cover[i][0] = 1;
			if (left_energy == 0) continue;
			for (int j = 0; j < vertex_num;j++) {
				for (int k = 0;k < coverd_1[i].size();k++) {
					if (k != j && k % 2 == 1 && coverd_1[i][k] == i) {
						int left_energy2 = energy[j] - left_energy;
						// W tym przypadku ca³a pozosta³a energia zosta³a przekazana temu wierzcho³kowi
						if (coverd_1[i][k + 1] == left_energy) {
							if (left_energy2 == 0) {
								children_1_cover[j][0] = 1;
								children_1_cover[j].push_back(i);
								children_1_cover[j].push_back(left_energy);
								children_1_energy[j] = 0;
							};
							if (left_energy2 != 0 && children_1_energy[j] == left_energy2) {
								children_1_cover[j][0]
							}

							// W tym przypadku przekazujemy ca³¹ energie ale nie zerujemy wierzcho³ka
							if (coverd_1[i][k + 1] > left_energy) {
						}
					};
				};
			};
		};




		children.first.push_back(parent1[i]);
		children.second.push_back(parent2[i]);
	}
	for (int i = position + 1; i < n; i++) {
		children.first.push_back(parent2[i]);
		children.second.push_back(parent1[i]);
	};

	};
}



void child_cover(vector<int> child, vector<int> energy, vector<vector<int>> energy_1, vector<vector<int>> energy_2, int switch_point, int vertex_num, int** graph) { 
	vector<vector<int>> child_energy;
	vector<int> coverd;
	vector<int> dummy{ 1, 0 };

	coverd = child;

	for (int i = 0; i < switch_point;i++) {
		child_energy.push_back(dummy);
		child_energy[i][0] = energy[i];
		if(child[i] == 1) {
			child_energy[i][0] = 0;
			child_energy[i].push_back(i);
			child_energy[i].push_back(energy[i]);
		};
	};

	for(int i = 0; i<switch_point;i++) {
		if (energy[i] == 15) continue; 


		for (int j = 0;j < vertex_num;j++) {

			if (j == i) continue;

			for (int k = 1;k < energy_1[j].size(); k+=2) {


				if (energy_1[j][k] == i && child[j] == 0 && coverd[j] == 0) {
					child_energy[j][0] = child_energy[j][0] - energy_1[j][k + 1];
					if (child_energy[j][0] <= 0) {
						coverd[j] = 1;
					};
					child_energy[j].push_back(i);
					child_energy[j].push_back(energy_1[j][k + 1]);
				}
				else if (energy_1[j][k] == i && (child[j] == 1 || coverd[j] == 1)) {

					vector<int> nastepnicy;
					for (int z = 0;z < vertex_num;z++) {
						if (graph[i][z] == 1 && coverd[z] == 0 && child[z] == 0) {
							nastepnicy.push_back(z);
						};
					};

					if (nastepnicy.size() == 0) continue;


					uniform_int_distribution<int> chose_next(0, nastepnicy.size() - 1);
					int next_vertex = nastepnicy[chose_next(e1)];

					if (child_energy[j][0] == energy_1[j][k + 1]) {
						child_energy[j][0] = 0;
						child_energy[j].push_back(i);
						child_energy[j].push_back(energy_1[j][k + 1]);
					}
					else if (child_energy[j][0] > energy_1[j][k + 1]) {
						child_energy[j][0] = child_energy[j][0] - energy_1[j][k + 1];

						child_energy[j].push_back(i);
						child_energy[j].push_back(energy_1[j][k + 1]);

					} else if(child_energy[j][0] < energy_1[j][k + 1]);

				};


			}
		};
	}
};






void genetic(int** graph, int vertex_num, vector<int> energy) {

	int populations_size = 50;
	vector<vector<int>> population;
	for (int i = 0;i < populations_size;i++) {
		population.push_back(create_member(vertex_num));
	};

	cout << "Stworzono populacje" << endl;
	for (int i = 0; i < vertex_num; i++) {
		cout << population[0][i];
	};
	cout << endl;
	vector< vector<vector<int>>> coverd;
	for (int i = 0;i < populations_size;i++) {
		coverd.push_back(member_cover(graph, population[i], vertex_num, energy));
	};
	cout << "Stworzono przykladowe pokrycie" << endl;
	int best_fitness = fitness(graph, coverd[0], vertex_num);
	vector<vector<int>> fittest_individual = coverd[0];
	int gen = 0;

		best_fitness = fitness(graph, coverd[0], vertex_num);
		fittest_individual = coverd[0];
		for (vector<vector<int>> individual : coverd)
		{
			int f = fitness(graph, individual, vertex_num);
			if (f < best_fitness)
			{
				best_fitness = f;
				fittest_individual = individual;
			}
		}
		if (gen % 100 == 0)
		{
			std::cout << "Generation: " << gen << " Best Fitness: " << best_fitness << " Individual: " << std::endl;
			for (vector<int> x : fittest_individual)
				std::cout << x[0] << " ";
			std::cout << std::endl;
		}
}
*/