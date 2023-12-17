#pragma once
#include <random>
#include <iostream>
#include <vector>
#include <cstdio>
#include <cmath>
#include <algorithm>
#include <iterator>
#include <utility>

#include "zachlanny.h"
#include"genetyczny.h"
#include"brutal.h"

#include <chrono>
/* Only needed for the sake of this example. */
#include <iostream>
#include <thread>

#include "nlohmann/json.hpp"
#include "nlohmann/json_fwd.hpp"

using namespace std;
using json = nlohmann::json;

void generate_graph(int edges_num, int vertex_num);
void make_json(int vertex_num, int** graph, vector<int> energy);
void print_graph(int vertex_num, int** graph);
int** to_edges(int vertex_num, int edges_num, int** graph);
vector<vector<int>> copy_edges(int** edges, int vertex_num, int edges_num);


void generate_graph(int edges_num, int vertex_num) {
	int i = 0;
	int** graph = new int* [vertex_num];
	int vertex_1, vertex_2;
	vector<int> vertexes_weight;
	vector<int> energy{ 5, 10, 15 };

	random_device dev;
	mt19937 rng(dev());
	uniform_int_distribution<std::mt19937::result_type> dist6(0, vertex_num - 1);
	uniform_int_distribution<std::mt19937::result_type>enr(0, 2);


	for (int i = 0; i < vertex_num; i++) {
		graph[i] = new int[vertex_num];
		vertexes_weight.push_back(energy[enr(rng)]);
		for (int j = 0; j < vertex_num;j++) {
			graph[i][j] = 0;
		};
	};

	while (i < edges_num) {
		vertex_1 = dist6(rng);
		vertex_2 = dist6(rng);
		//cout << vertex_1 << " <- 1" << endl;
		//cout << vertex_2 << " <- 2" << endl;

		if (vertex_1 == vertex_2) {
			continue;
		}
		else {
			if (graph[vertex_1][vertex_2] == 1) continue;
			//cout << "Tu sie udalo! " << endl;
			graph[vertex_1][vertex_2] = 1;
			graph[vertex_2][vertex_1] = 1;
			i++;
		};
	};

	// PRZYK£ADOWA MATRYCA
	/*graph[0][1] = 1;
	graph[0][2] = 1;
	graph[1][0] = 1;
	graph[2][0] = 1;
	graph[2][3] = 1;
	graph[3][2] = 1;
	vertexes_weight[0] = 5;
	vertexes_weight[1] = 10;
	vertexes_weight[2] = 10;
	vertexes_weight[3] = 5;
	*/

	/* for (int i = 0; i < vertex_num; i++) {
		for (int j = 0; j < vertex_num;j++) {
			cout << graph[i][j] << " ";
		};
		<< "<- To jest jedne wiersz : " << i << " " << endl;
	*/

	print_graph(vertex_num, graph);
	//make_json(vertex_num, graph, energy);

	vector<vector<int>> edges_2;
	int** edges = new int* [edges_num];
	edges = to_edges(vertex_num, edges_num, graph);
	edges_2 = copy_edges(edges, vertex_num, edges_num);
	// greddy_cover(edges, vertex_num, edges_num);



	//string x = bin(3, vertex_num);
	string x = "1000";

	cout << x << endl;

	int best_cover = vertex_num + 1;
	string final_solution_brute;
	vector<int> to_delete;
	vector<int> coverd_vertexes_brute(vertex_num, 0);

	vector<int> final_solution_zach(vertex_num, 0);
	vector<int> coverd_vertexes_zach(vertex_num, 0);

	vector<int> final_solution_los(vertex_num, 0);
	vector<int> coverd_vertexes_los(vertex_num, 0);

	vector<int> final_solution_gen(vertex_num, 0);
	vector<int> coverd_vertexes_gen(vertex_num, 0);
	/*
		int num_of_combo = pow(2, vertex_num);
		for (int i = 0; i < num_of_combo;i++) {
			string z = bin(i, vertex_num);
			for (int j = 0; j < vertex_num; j++) {
				if (z[j] == '1') {
					brute_helper(graph, edges_2, j, vertex_num, vertexes_weight, z, best_cover, final_solution, coverd_vertexes);
					cout << "SPRAWDZAM KOLEJNA OPCJE --------------------- Koniec poprzedniej jest wyzej" << endl;
					cout << endl;
					break;
				};
			};

		};
		*/

	using chrono::high_resolution_clock;
	using chrono::duration_cast;
	using chrono::duration;
	using chrono::milliseconds;



	auto b1 = high_resolution_clock::now();
	brute_force(vertex_num, graph, vertexes_weight, edges_2, final_solution_brute, coverd_vertexes_brute);
	auto b2 = high_resolution_clock::now();

	//auto g1 = high_resolution_clock::now();
	//greddy_cover(edges_2, vertex_num, edges_num, vertexes_weight, graph, 0, final_solution_zach, coverd_vertexes_zach);
	//auto g2 = high_resolution_clock::now();

	//auto gl1 = high_resolution_clock::now();
	//greddy_cover(edges_2, vertex_num, edges_num, vertexes_weight, graph, 1, final_solution_los, coverd_vertexes_los);
	//auto gl2 = high_resolution_clock::now();
	cout << "----" << endl;
	auto gen1 = high_resolution_clock::now();
	genetic(graph, vertex_num, vertexes_weight, final_solution_gen, coverd_vertexes_gen);
	auto gen2 = high_resolution_clock::now();
	cout << "----" << endl;

	auto b_int = duration_cast<milliseconds>(b2 - b1);
	//auto g_int = duration_cast<milliseconds>(g2 - g1);
	//auto gl_int = duration_cast<milliseconds>(gl2- gl1);
	auto gen_int = duration_cast<milliseconds>(gen2 - gen1);
	/*cout << "GREDDY: " << endl;
		for (int j = 0; j < vertex_num;j++) {
			cout << final_solution_gen[j] << " ";
		};
		cout << endl;
	*/
	//cout << "Rozwiazania: " << endl;
//	cout << final_solution_gen[3] << "DD" << endl;
//	cout << "Brute force, czas - " << b_int.count() << ", fabryki - " << final_solution_brute << endl;
	//cout << "Zachlnny, czas - " << g_int.count() << ", fabryki - " << endl;

	//cout << "Zachlanny losowy, czas - " << gl_int.count() << ", fabryki - " << endl;
	cout << "Genetyczny, czas - " << gen_int.count() << ", fabryki - " << endl;

	//cout << "Najlepsza opcja: " << final_solution << endl;
};

void make_json(int vertex_num, int** graph, vector<int> energy) {
	json j;
	for (int i = 0; i < vertex_num; i++) {
		for (int k = 0; k < vertex_num; k++) {
			j["graph"][i][k] = graph[i][k];
		};
	};
	string s = j.dump();
	cout << s;
};

void print_graph(int vertex_num, int** graph) {
	for (int i = 0; i < vertex_num; i++) {
		int first = 0;
		cout << "Wierzcholek " << i << " jest incindenty z: {";
		for (int k = 0; k < vertex_num; k++) {
			if (graph[i][k] == 1) {
				if (first == 1) cout << ", ";
				cout << k;
				first = 1;
			};


		};
		cout << "}." << endl;
	};


};

int** to_edges(int vertex_num, int edges_num, int** graph) {

	int** edges = new int* [edges_num];
	int edge_index = 0;
	for (int i = 0; i < vertex_num; i++) {
		for (int j = i;j < vertex_num;j++) {
			if (graph[i][j] == 1) {
				edges[edge_index] = new int[2];
				edges[edge_index][0] = i;
				edges[edge_index][1] = j;

				cout << "Krawedzie: " << edges[edge_index][0] << " " << edges[edge_index][1] << ". Index: " << edge_index << endl;
				cout << endl;

				edge_index += 1;

			};
		};
	};
	return edges;
};

vector<vector<int>> copy_edges(int** edges, int vertex_num, int edges_num) {

	vector<int> arg = { 0, 0 };
	vector<vector<int>> edge_copy;

	for (int i = 0; i < edges_num; i++) {

		arg[0] = edges[i][0];
		arg[1] = edges[i][1];
		edge_copy.push_back(arg);

	};
	return edge_copy;
};


