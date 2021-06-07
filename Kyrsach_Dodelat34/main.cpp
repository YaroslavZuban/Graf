#include <iostream>
#include <queue>
#include <vector>
#include <algorithm>
using namespace std;

int** adjacency_matrix;	 // ��������� �� ������� ���������
int N;					// ���������� ������

const int M = 9999999; // ����� ������� �����

bool CheckConnectivity()//��������� �� ���������� ��������� ����� �����!
{
	queue<int> queue;//������� ������� � ������� ����������
	bool* visited = new bool[N];//������� ������ ��� ���������� �������� �� ��� ������� ��� ���
	// ������������� 
	for (int i = 0; i < N; i++)
	{
		visited[i] = false; //����������� ���� ��������� �������  ����, ��� ���� ����� ������ �������� �� ������ ������� ��� ���
	}

	queue.push(0); // ������ ����� � ������� ������, ������ ������� ��������� � ������� ������ �����
	while (!queue.empty())//������ ������� ��������� ������� �� ������� ��������� 
	{
		int current = queue.front();// ������� ��������� ���������� 
		queue.pop();//������� �������� �������
		visited[current] = true;// ���������� ��� ������� ������ �������

		for (int i = 0; i < N; i++)
		{
			if (adjacency_matrix[current][i] != M && !visited[i]) // ���� ���� ���� � ������� �� ���� ��������
			{
				queue.push(i);//��������� ������� � ����� �������
			}
		}
	}

	// ���� ���� �� ���� �� �������� - ���� ���������
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
	F = fopen("�����.txt", "r");
	fscanf(F, "%d", &N); 	// ������ ����� - ���������� ������

	// ��������� ������ ��� ������� ���������
	adjacency_matrix = new int* [N];
	for (int i = 0; i < N; i++)
	{
		adjacency_matrix[i] = new int[N];

		// � ����� �������� ��
		for (int j = 0; j < N; j++)
		{
			if (i == j)
			{
				adjacency_matrix[i][j] = 0;
			}
			else
			{
				adjacency_matrix[i][j] = M;	  // M = ���� ���
			}
		}
	}


	// ������ ������ � ����� ��� [������], [����], [����������]
	int from, to, dist;
	while (fscanf(F, "%d %d %d", &from, &to, &dist) != EOF)
	{
		adjacency_matrix[from][to] = dist;
		adjacency_matrix[to][from] = dist;
	}

}

vector<int> FindCity()
{
	// ������� ����� ��� ������� ���. ���������� � ��������� � ��� �������� ������� ���������
	int** distances_matrix = new int* [N];
	for (int i = 0; i < N; i++)
	{
		distances_matrix[i] = new int[N];
		for (int j = 0; j < N; j++)
		{
			distances_matrix[i][j] = adjacency_matrix[i][j];
		}
	}

	// ���������� ������� ���. ���������� ����� ������ ������� �� ��������� ������-��������
	for (int i = 0; i < N; i++)
		for (int u = 0; u < N; u++)
			for (int v = 0; v < N; v++)
			{
				if (distances_matrix[u][v] > distances_matrix[u][i] + distances_matrix[i][v])
					distances_matrix[u][v] = distances_matrix[u][i] + distances_matrix[i][v];
			}

	// ����� �����, ����� ���������� �� �������� �� ��������� ����������
	int city_result = 0;
	int min_sum_dist = M;
	bool isAppropriation = true;
	vector<int> count;//������� ������� � ���������� ����������� � ���� ���������� �������
	for (int u = 0; u < N; u++) {
		for (int i = 0; i < N; i++)
		{
			int sum_dist = 0;
			// ������ ����� ��� ������ � ������� i
			for (int j = 0; j < N; j++)
			{
				sum_dist += distances_matrix[i][j];
			}

			// ������� ���������� ����� � ����� ������
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
	cout << "��������� �����: ";
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
		printf("���� �� ������ ��� �������� �� �����!\n");
		return;
	}
	else {
		vector<int> city = FindCity();
		outputDisplay(city);
	}
}