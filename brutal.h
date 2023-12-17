#pragma once
#include <random>
#include <iostream>
#include <vector>
#include <cstdio>
#include <cmath>
#include <algorithm>
#include <iterator>
#include <utility>

#include "nlohmann/json.hpp"
#include "nlohmann/json_fwd.hpp"

using namespace std;

string bin(int n, int vertex_num);
vector<vector<int>> delte_edges(vector<vector<int>> edges, int edges_num, int edge_to_delete);
void brute_helper(int** graph, vector<vector<int>> edges, int curr_index, int vertex_num, vector<int> energy, string plants, int& best_cover, string& final_solution, vector<int> coverd_vertexes);
void brute_force(int vertex_num, int** graph, vector<int> vertexes_weight, vector<vector<int>> edges_2, string& final_solution, vector<int>& coverd_vertexes);



void brute_force(int vertex_num, int**graph, vector<int> vertexes_weight, vector<vector<int>> edges_2, string& final_solution, vector<int>& coverd_solution) {
	int best_cover = vertex_num + 1;
	vector<int> to_delete;
	int num_of_combo = pow(2, vertex_num);


	for (int i = 0; i < num_of_combo;i++) {
		string z = bin(i, vertex_num);
		for (int j = 0; j < vertex_num; j++) {
			if (z[j] == '1') {
				brute_helper(graph, edges_2, j, vertex_num, vertexes_weight, z, best_cover, final_solution, coverd_solution);
				break;
			};
		};
	};

};

string bin(int n, int vertex_num) {
	string b(vertex_num, '0');
	int i = n;
	int pos = 0;

	while (i > 0) {
		if (i % 2 == 1) {
			b[pos] = '1';
		};
		if (i % 2 == 0) {
			b[pos] = '0';
		};
		i = i >> 1;
		pos += 1;

	};
	//cout << "Liczba: " << b << endl;
	return b;
};



void brute_helper(int** graph, vector<vector<int>> edges, int curr_index, int vertex_num, vector<int> energy, string plants, int& best_cover, string& final_solution, vector<int> coverd_vertexes) {
	vector<int> new_delete;
	vector<vector<int>> temp_edges;
	coverd_vertexes[curr_index] = 1;

	//cout << "Usuwam wierzcholek: " << curr_index << endl;


	temp_edges = delte_edges(edges, edges.size(), curr_index);

	//cout << "Pozostale krawedzie: " << endl;
	//for (int i = 0;i < temp_edges.size();i++) {
		//cout << i << ": " << temp_edges[i][0] << " " << temp_edges[i][1] << endl;
	//};
	coverd_vertexes[curr_index] = 1;
	//cout << "Zostalo: " << temp_edges.size() << endl;

	if (temp_edges.size() == 0) {
		//cout << "Pokryto wszystko przy sprawdzeniu nowego wierzcholka||||||||||||||||" << endl;
		int num_of_plants = 0;
		for (int i = 0; i < vertex_num; i++) {
			if (plants[i] == '1') {
				num_of_plants++;
			};
		};
		if (num_of_plants < best_cover) {
			best_cover = num_of_plants;
			final_solution = plants;
		};
		return;
	};



	// Twoerzenie kolejen opcij

	vector<int> temp_energy;
	// curr_index - wierzcho³ek z fabryk¹, cover_vertexes - wierzcho³ki pokryte przez elketrownie

	temp_energy = energy;

	// Jeœli wierzcho³ek ma zapotrzebowanie 15, to nie mo¿na przekazaæ energi
	if (energy[curr_index] == 15) {
		// Zereujemy energie
		temp_energy[curr_index] = 0;

		//cout << "Energia wierzcholka rowna sie 15, szukamy kolejnej fabryki" << endl;

		// Idziemy do kolejnego wierzcho³ka z farbeyk¹ energetyczn¹ 
		for (int i = curr_index + 1;i < vertex_num;i++) {
			if (plants[i] == '1') {
				brute_helper(graph, temp_edges, i, vertex_num, temp_energy, plants, best_cover, final_solution, coverd_vertexes);
				return;
			};

		};
		//cout << "Nie znaleziono nowego nowej fabryki, cofam sie do poprzedniej opcji" << endl;
		return;
	};



	// ENrgia pozosta³a do przekaznaia
	int left_energy = 15 - temp_energy[curr_index];

	//cout << "POZOSTALA ENERGIA: " << left_energy << endl;
	// Sprawdzamy wyszstkie wierzcho³ki ktorym mo¿na przekazaæ energie
	for (int i = 0; i < vertex_num; i++) {

		vector<int> new_delete;
		temp_energy = energy;
		temp_energy[curr_index] = 0;
		vector<vector<int>> temp_edges2;



		// Najpierw sprawdzamy czy wierzhoc³ek jest bez elektrowni, i czy nie jest pokryty
		if (graph[curr_index][i] == 1 && coverd_vertexes[i] == 0 && i != curr_index && plants[i] != '1') {



			// Perzypadke gdzie przekazujemy ca³¹ energie
			if (energy[i] == left_energy) {
			//	cout << "Energia jest sobie rowna: " << left_energy << ". Przekazuje wierzcholkowi: " << i << " z " << curr_index << endl;
				coverd_vertexes[i] = 1;
				temp_edges2 = delte_edges(temp_edges, temp_edges.size(), i);
				temp_energy[i] = 0;
				if (temp_edges2.size() == 0) {
					//cout << "POKRYTO WSZYSTKO" << endl;
					int num_of_plants = 0;
					for (int z = 0; z < sizeof(plants); z++) {
						if (plants[i] == '1') {
							num_of_plants++;
						};
					};
					//cout << "XDD" << endl;

					if (num_of_plants < best_cover) {
						best_cover = num_of_plants;
						final_solution = plants;
					};
					return;
				};
				// Nie pokryto grafu, szukamy nastêpnej elektrowni
				for (int j = curr_index + 1; j < vertex_num;j++) {
					if (plants[j] == '1') {
						brute_helper(graph, temp_edges2, j, vertex_num, temp_energy, plants, best_cover, final_solution, coverd_vertexes);
						break;
					}
				}

			}

			// Przekazanie nie zeruje wierzcho³ka
			else if (energy[i] > left_energy) {
				//cout << "Energia przekazana wierzcholkowi, nie zeruje: " << i << endl;

				temp_energy[i] = temp_energy[i] - left_energy;


				// szukamy nastêpnej elektrowni
				for (int j = curr_index + 1; j < vertex_num;j++) {
					if (plants[i] == '1') {
						brute_helper(graph, temp_edges, j, vertex_num, energy, plants, best_cover, final_solution, coverd_vertexes);
						break;
					}
				}
				continue;
			}



			//Przypadek gdzie przekazujemy ernegire wiêcej ni¿ jednemu wierzcho³kowi
			else if (energy[i] < left_energy) {
				//cout << "Energia przekazana wierzcholkowi, zeruje: " << i << ". Zostalo wiecej energi, szukamy kolejnego wierzcholka." << endl;

				//Pozosta³a energia któr¹ mo¿emy przekazaæ do innego wierzcho³ka
				int left_energy2 = left_energy - temp_energy[i];

				temp_energy[i] = 0;
				coverd_vertexes[i] = 1;
				temp_edges2 = delte_edges(temp_edges, temp_edges.size(), i);
				if (temp_edges2.size() == 0) {
					//cout << "POKRYTO WSZYSTKO" << endl;
					int num_of_plants = 0;
					for (int z = 0; z < sizeof(plants); z++) {
						if (plants[i] == '1') {
							num_of_plants++;
						};
					};
					//cout << "XDD" << endl;

					if (num_of_plants < best_cover) {
						best_cover = num_of_plants;
						final_solution = plants;
					};
					return;
				};

				//szukamy kolejnego wierzcho³ka

				for (int k = 0; k < vertex_num; k++) {
					vector<vector<int>> temp_edges3;
					vector<int> temp_energy2;
					temp_energy2 = temp_energy;
					vector<int> new_delete;
					vector<int> temp_coverd;
					temp_coverd = coverd_vertexes;
					// Zosta³o maksymalnie 5 energi, wiêc sprawdzamy tylko czy jest wiezrzcho³ek z równym lub wiêkszym zapotrzebowaniem energii
					if (k != curr_index && k != i && graph[curr_index][k] == 1 && temp_coverd[k] == 0 && plants[k] != '1') {

						if (temp_energy2[k] == left_energy2) {
							//cout << "Energia przekazana wierzcholkowi, zeruje (2): " << k << endl;
							temp_energy2[k] = 0;
							temp_edges3 = delte_edges(temp_edges2, temp_edges2.size(), k);

							if (temp_edges3.size() == 0) {
								//cout << "POKRYTO WSZYSTKO" << endl;
								int num_of_plants = 0;
								for (int z = 0; z < sizeof(plants); z++) {
									if (plants[i] == '1') {
										num_of_plants++;
									};
								};
								//cout << "XDD" << endl;

								if (num_of_plants < best_cover) {
									best_cover = num_of_plants;
									final_solution = plants;
								};
								return;
							};

							temp_coverd[k] = 1;
							for (int j = curr_index + 1; j < vertex_num;j++) {
								if (plants[j] == '1') {
									brute_helper(graph, temp_edges3, j, vertex_num, temp_energy2, plants, best_cover, final_solution, temp_coverd);
									break;
								}
							};
							/*if (temp_edges3.size() == 0) {
								cout << "POKRYTO WSZYSTKO" << endl;
								int num_of_plants = 0;
								for (int z = 0; z < sizeof(plants); z++) {
									if (plants[i] == '1') {
										num_of_plants++;
									};
								};
								cout << "XDD" << endl;

								if (num_of_plants < best_cover) {
									best_cover = num_of_plants;
									final_solution = plants;
								};
							};*/
						}

						// Przekazujemy energie ale nie pokrywamy wierzcho³ka

						else if (left_energy < temp_energy[k]) {
							//cout << "Energia przekazana wierzcholkowi, zeruje (2): " << k << endl;

							temp_energy2[k] = temp_energy2[k] - left_energy2;
							for (int j = curr_index + 1; j < vertex_num;j++) {
								if (plants[j] == '1') {
									brute_helper(graph, temp_edges2, j, vertex_num, temp_energy2, plants, best_cover, final_solution, coverd_vertexes);
									break;
								}
							}
						};
					};
				};
			};
		};
	};
	for (int i = curr_index + 1;i < vertex_num;i++) {
		if (plants[i] == '1') {
			brute_helper(graph, temp_edges, i, vertex_num, temp_energy, plants, best_cover, final_solution, coverd_vertexes);
			return;
		};
	};
};





vector<vector<int>> delte_edges(vector<vector<int>> edges, int edges_num, int edge_to_delete) {
	vector<vector<int>> dummy_edges;
	dummy_edges = edges;
	int left_edges = edges_num;
	int i = 0;
	while (i < dummy_edges.size()) {
		if (dummy_edges[i][0] == edge_to_delete || dummy_edges[i][1] == edge_to_delete) {
			dummy_edges.erase(dummy_edges.begin() + i);

		}
		else {
			i++;
		};
	}
	return dummy_edges;
};
