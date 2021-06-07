#include <iostream>
#include <queue>
#include <vector>
#include <algorithm>
using namespace std;

int** adjacency_matrix;	 // указатель на матрицу смежности
int N;					// количество вершин

const int M = 9999999; // очень большое число

bool CheckConnectivity()//проверяем на связаность элементов между собой!
{
	queue<int> queue;//создаем очередь с помощью библиотеки
	bool* visited = new bool[N];//создаем массив для простомтра посещаем мы эту вершину или нет
	// инициализация 
	for (int i = 0; i < N; i++)
	{
		visited[i] = false; //присваиваем всем элементам массива  ложь, для того чтобы понять посещали мы данную вершину или нет
	}

	queue.push(0); // начнем обход с первого города, данная функция добавляет в очередь первый город
	while (!queue.empty())//данная функция проверяет очередь на пустату структуру 
	{
		int current = queue.front();// доступк следующей переменной 
		queue.pop();//удаляем следущий элемент
		visited[current] = true;// обозначаем что посщали данную вершину

		for (int i = 0; i < N; i++)
		{
			if (adjacency_matrix[current][i] != M && !visited[i]) // если есть путь и вершина не была посещена
			{
				queue.push(i);//добавляем элемент в конец очереди
			}
		}
	}

	// если хотя бы одна не посещена - граф несвязный
	for (int i = 0; i < N; i++)
	{
		if (!visited[i])
			return false;
	}

	return true;
}

void FillAdjacencyMatrix()
{
	FILE* F;
	F = fopen("Текст.txt", "r");
	fscanf(F, "%d", &N); 	// первое число - количество вершин

	// выделение памяти для матрицы смежности
	adjacency_matrix = new int* [N];
	for (int i = 0; i < N; i++)
	{
		adjacency_matrix[i] = new int[N];

		// и сразу заполним ее
		for (int j = 0; j < N; j++)
		{
			if (i == j)
			{
				adjacency_matrix[i][j] = 0;
			}
			else
			{
				adjacency_matrix[i][j] = M;	  // M = пути нет
			}
		}
	}


	// каждая строка в файле это [откуда], [куда], [расстояние]
	int from, to, dist;
	while (fscanf(F, "%d %d %d", &from, &to, &dist) != EOF)
	{
		adjacency_matrix[from][to] = dist;
		adjacency_matrix[to][from] = dist;
	}

}

vector<int> FindCity()
{
	// выделим место под матрицу мин. расстояний и скопируем в нее значения матрицы смежности
	int** distances_matrix = new int* [N];
	for (int i = 0; i < N; i++)
	{
		distances_matrix[i] = new int[N];
		for (int j = 0; j < N; j++)
		{
			distances_matrix[i][j] = adjacency_matrix[i][j];
		}
	}

	// просчитать матрицу мин. расстояний между парами городов по алгоритму Флойда-Уоршелла
	for (int i = 0; i < N; i++)
		for (int u = 0; u < N; u++)
			for (int v = 0; v < N; v++)
			{
				if (distances_matrix[u][v] > distances_matrix[u][i] + distances_matrix[i][v])
					distances_matrix[u][v] = distances_matrix[u][i] + distances_matrix[i][v];
			}

	// найти город, сумма расстояний от которого до остальных наименьшая
	int city_result = 0;
	int min_sum_dist = M;
	bool isAppropriation = true;
	vector<int> count;//создаем векторв в дальнейшем заприсываем в него расстояние городов
	for (int u = 0; u < N; u++) {
		for (int i = 0; i < N; i++)
		{
			int sum_dist = 0;
			// найдем сумму для города с номером i
			for (int j = 0; j < N; j++)
			{
				sum_dist += distances_matrix[i][j];
			}

			// обновим наименьшую сумму и номер города
			if (sum_dist <= min_sum_dist)
			{
				city_result = i;
				if (!count.empty()) {
					if (min_sum_dist == sum_dist && find(count.begin(), count.end(), city_result)==count.end()) {
						count.push_back(city_result);
					}
				}

				min_sum_dist = sum_dist;
			}
		}

		if (isAppropriation) {
			count.push_back(city_result);
			isAppropriation = false;
		}
	}

	return count;
}

void outputDisplay(vector<int> city) {
	cout << "Результат равен: ";
	for (int i = 0; i < city.size(); i++) {
		cout << city[i];
		if (i + 1 < city.size())
			cout << ", ";
	}
}

void main()
{
	setlocale(LC_ALL, "Russian");
	FillAdjacencyMatrix();

	if (!CheckConnectivity())
	{
		printf("Граф не найден или построен не верно!\n");
		return;
	}
	else {
		vector<int> city = FindCity();
		outputDisplay(city);
	}
}