#pragma once
#include <random>
#include <iostream>
#include <vector>
#include <cstdio>
#include <cmath>
#include <algorithm>
#include <iterator>
#include <utility>


void greddy_cover(vector<vector<int>> edges, int vertex_num, int edges_num, vector<int> energy, int** grpah, int losowy, vector<int>& final_solutio, vector<int>& cover_solution);
void greddy_helper(vector<int>& coverd, vector<int>& energy, int** graph, int vertex_num, int vertex, vector<int>& to_delete, vector<int>& plants);

using namespace std;

random_device dev;
mt19937 rng(dev());



// Algorytm wyibera wierzcho³ki na podstawie krawêdzi, u suwa wszystkie werzcho³ki któe znajduj¹ siê w rozwij¹zaniu


void greddy_cover(vector<vector<int>> edges, int vertex_num, int edges_num, vector<int> energy, int** graph, int losowy, vector<int>& final_solution, vector<int>& cover_solution) {
	vector<int> coverd_vertexes;
	vector<int> plants;
	int vertex_1 = -1;
	int vertex_2 = -1;
	int left_edges = edges_num;
	//cout << "Losowy: " << losowy << endl;
	for (int i = 0;i < vertex_num;i++) {
		coverd_vertexes.push_back(0);
		plants.push_back(0);
	};

	if (losowy == 0) {
		vertex_1 = edges[0][0];
		vertex_2 = edges[0][1];
		plants[vertex_1] = 1;
		plants[vertex_2] = 1;

	};

	if (losowy == 1) {
		int chosen_edge;
		uniform_int_distribution<mt19937::result_type> edge_choser(0, edges.size() - 1);
		chosen_edge = edge_choser(rng);
		vertex_1 = edges[chosen_edge][0];
		vertex_2 = edges[chosen_edge][1];
		plants[vertex_1] = 1;
		plants[vertex_2] = 1;
	};

	//cout << "TUTAJ" << endl;
	//cout << "T  " << vertex_1 << endl;
	//cout << "T  " << vertex_2 << endl;

	coverd_vertexes[vertex_1] = 1;
	coverd_vertexes[vertex_2] = 1;
	//cout << "TUTAJ" << endl;

	vector<int> temp_energy;
	vector<int> to_delete;

	temp_energy = energy;
	//cout << "TUTAJ --ddd" << endl;


	greddy_helper(coverd_vertexes, temp_energy, graph, vertex_num, vertex_1, to_delete, plants);
	greddy_helper(coverd_vertexes, temp_energy, graph, vertex_num, vertex_2, to_delete, plants);


	//cout << "TUTAJ 55555--" << endl;

	vector<vector<int>> dummy_edges;
	dummy_edges = edges;

	/*for (int i = 0;i < edges_num;i++) {
		cout << "Krawedzie w dumy: " << dummy_edges[i][0] << " " << dummy_edges[i][1] << ". Index: " << i << endl;

	}; */
	while (left_edges) {
		int x = 0;
		int temp_left = left_edges;
		//cout << endl;
		//cout << "POZOSTALE KRAWEDZIE: " << left_edges << endl;

		for (int a = 0;a < to_delete.size();a++) {
			temp_left = left_edges;
			for (int i = 0; i < left_edges; i++) {
				if (dummy_edges[i][0] == to_delete[a] || dummy_edges[i][1] == to_delete[a]) {
					temp_left -= 1;
				}
				else {

					dummy_edges[x][0] = dummy_edges[i][0];
					dummy_edges[x][1] = dummy_edges[i][1];
					x += 1;
				};
			};
			x = 0;
			left_edges = temp_left;
		};


		if (left_edges == 0)  break;
		/*
		for (int i = 0; i < left_edges; i++) {
			if (dummy_edges[i][0] == vertex_1 || dummy_edges[i][1] == vertex_1 || dummy_edges[i][0] == vertex_2 || dummy_edges[i][1] == vertex_2) {
				temp_left -= 1;
			}
			else {

				dummy_edges[x][0] = dummy_edges[i][0];
				dummy_edges[x][1] = dummy_edges[i][1];
				x += 1;
			};
		}; */
		for (int i = 0; i < temp_left;i++) {
			//cout << "Krawedzie w dumy: " << dummy_edges[i][0] << " " << dummy_edges[i][1] << ". Index: " << i << endl;
		};
		//vertex_1 = dummy_edges[0][0];
		//vertex_2 = dummy_edges[0][1];

		if (losowy == 0) {
			vertex_1 = dummy_edges[0][0];
			vertex_2 = dummy_edges[0][1];
		};
		if (losowy == 1) {
			int chosen_edge;
			uniform_int_distribution<mt19937::result_type> edge_choser(0, left_edges - 1);
			chosen_edge = edge_choser(rng);
			int vertex_1 = dummy_edges[chosen_edge][0];
			int vertex_2 = dummy_edges[chosen_edge][1];
		};
		to_delete.clear();

		greddy_helper(coverd_vertexes, energy, graph, vertex_num, vertex_1, to_delete, plants);
		greddy_helper(coverd_vertexes, energy, graph, vertex_num, vertex_2, to_delete, plants);
	};


	final_solution = plants;

	cout << "Wieszcholki w rozwiazaniu: ";
for (int i = 0; i < final_solution.size(); i++) {
	cout << final_solution[i] << " ";
};
	cover_solution = coverd_vertexes;
	//cout << "Wieszcholki w rozwiazaniu: ";
	//for (int i = 0; i < plants.size(); i++) {
		//cout << plants[i] << " ";
	//};

};


void greddy_helper(vector<int>& coverd, vector<int>& energy, int**graph, int vertex_num, int vertex, vector<int>& to_delete, vector<int>& plants) {
	int left_energy;
	vector<int> nastepnicy;
	int nas_size;
	//cout << "energy: " << energy.size() << endl;
	to_delete.push_back(vertex);
	plants[vertex] = 1;


	left_energy = 15 - energy[vertex];
	energy[vertex] = 0;
	coverd[vertex] = 1;
	//cout << "XDDDDD";
	if (left_energy == 0) return;

	for (int i = 0;i < vertex_num;i++) {
		if (graph[vertex][i] == 1 && coverd[i] == 0) {
			nastepnicy.push_back(i);
		};
	};
	nas_size = nastepnicy.size();

	//cout << "nas_size: " << nas_size << endl;


	if (nas_size != 0) {
		//cout << "Nassfdfdfs: " << endl;

		uniform_int_distribution<mt19937::result_type> nas1(0, nas_size - 1);
		int ind = nas1(rng);
		//cout << "Nass: " << ind << endl;

		int next_vertex = nastepnicy[ind];
		//cout << "Next_vertes: " << next_vertex << endl;

		if (left_energy == energy[next_vertex] || left_energy < energy[next_vertex]) {
			//cout << "TToosdsdsooooo" << endl;

			energy[next_vertex] = energy[next_vertex] - left_energy;
			if (energy[next_vertex] == 0) {
				coverd[next_vertex] = 1;
				to_delete.push_back(next_vertex);
			};
		}
		else if (left_energy > energy[next_vertex] && energy[next_vertex] != 0) {
			//cout << "TToosdsdsdsfdsfgrgresdfgfddsooooo" << endl;

			coverd[next_vertex] = 1;
			energy[vertex] = 0;

			to_delete.push_back(next_vertex);

			nastepnicy.erase(nastepnicy.begin() + ind);
			nas_size--;
			left_energy = left_energy - energy[next_vertex];
			//cout << "Tutaj jeszcze ok" << endl;
			if (nas_size != 0) {
				uniform_int_distribution<mt19937::result_type> nas1(0, nas_size - 1);

				ind = nas1(rng);
				next_vertex = nastepnicy[ind];
				if (left_energy == energy[next_vertex] || left_energy < energy[next_vertex]) {
					energy[next_vertex] = energy[next_vertex] - left_energy;
					if (energy[next_vertex] == 0) {
						energy[next_vertex] = 0;
						coverd[next_vertex] = 1;
						to_delete.push_back(next_vertex);
					};
				}
			};
		};
	}
};