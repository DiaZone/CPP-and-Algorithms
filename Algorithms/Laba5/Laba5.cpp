#include <iostream>
#include <algorithm>
#include <vector>
#include <list>
#include <random>
#include <stack>
#include <iomanip>
#include <chrono>
#include <math.h>

using namespace std;
#define INF 21 //Максимум 20
const int tests = 10; //Количество тестов

//Класс графа
/*
* @param count_of_vortex - Количество вершин
* @param count_of_edges - Количество ребер
* @param matrix_smezh - Матрица смежности
* @param list_edges - Список ребер
* @param binds - Количество связанных вершин
*/
class Grapth {
public:
	int count_of_vortex;
	int count_of_edges;
	list <list<int>> list_edges;
	int** matrix_smezh;
	int* binds;

	//Вывод матрицы смежности
	void printMatrixSmezh() {
		cout << "Матрица смежности\n";
		cout << setw(4) << "|";
		for (int i = 0; i < count_of_vortex; i++) {
			cout << setw(2) << (char)(i + 65) << " ";
		}
		cout << endl;
		for (int i = 0; i < count_of_vortex; i++) {
			cout << setw(2) << (char)(i + 65) << " |";
			for (int j = 0; j < count_of_vortex; j++) {
				cout << setw(2) << matrix_smezh[i][j] << " ";
			}
			cout << endl;
		}
	}
};

//Генератор значения веса
int genWeight() {
	return 1 + rand() % 20;
}

//Генерация матрицы смежности
/*
* @param gr - Граф
* @param max_bind - Максимальное количество связей у вершины
* @param M - Максимально возможное количество ребер
* @param array_reb - Вектор ребер
* @param count - Количество расставленных ребер
*
* @return Граф
*/
Grapth genMatSmezh(Grapth gr, int min_bind) {

	int M = gr.count_of_vortex * (gr.count_of_vortex - 1) / 2; //Максимальное количество возможных ребер
	vector <int> array_reb(M); //Нулевой вектор ребер
	for (int i = 0; i < gr.count_of_edges; i++) { //Заполняем первые элементы
		array_reb[i] = 1;
	}

	//Перемешиваем вектор
	random_shuffle(array_reb.begin(), array_reb.end());

	//Динамически выделяем память под массивы
	gr.matrix_smezh = new int* [gr.count_of_vortex];
	gr.binds = new int[gr.count_of_vortex];
	for (int i = 0; i < gr.count_of_vortex; i++) {
		gr.matrix_smezh[i] = new int[gr.count_of_vortex];
		gr.binds[i] = 0;
	}

	int k = 0, count = 0;

	//Проходим по вершинам
	for (int i = 0; i < gr.count_of_vortex; i++) {

		//Вершина сама в себя не указывает, поэтому ставим 0
		gr.matrix_smezh[i][i] = 0;

		//Идем выше главной диагонали
		for (int j = i + 1; j < gr.count_of_vortex; j++) {

			//Ставим ребро и считаем сколько ребер ставится
			gr.matrix_smezh[i][j] = gr.matrix_smezh[j][i] = array_reb[k];
			k++;
			count++; //Счетчик количества ребер

			//Прибавляем к количеству связей у вершин 
			if (abs(gr.matrix_smezh[i][j]) == 1) {
				gr.binds[i]++;
				gr.binds[j]++;
			}
		}
	}

	//Заполнение недостающих связей
	int temp = 0;

	//Идем по вершинам
	for (int i = 0; i < gr.count_of_vortex; i++) {

		//Если связей не хватает
		if (gr.binds[i] < min_bind) { 
			for (int j = 0; j < gr.count_of_vortex; j++) {

				//Если в этом месте нет связи
				if (gr.matrix_smezh[i][j] == 0 && i != j) {

					//Размещаем связь
					gr.matrix_smezh[i][j] = gr.matrix_smezh[j][i] = 1;

					gr.binds[i]++; //Прибавляем к количеству связей у вершин
					gr.binds[j]++; //Прибавляем к количеству связей у вершин
					temp++;
					i--;
					break;
				}
			}
		}
	}

	//Удаление лишних связей
	while (temp != 0) {

		//Проходим по вершинам
		for (int i = 0; i < gr.count_of_vortex; i++) {

			//Если связей много
			if (gr.binds[i] > min_bind && temp > 0) { 
				for (int j = 0; j < gr.count_of_vortex; j++) {

					//Если в этом месте лишняя связь
					if (gr.binds[j] > min_bind && i != j) {

						//Удаляем связь
						gr.matrix_smezh[i][j] = gr.matrix_smezh[j][i] = 0;

						gr.binds[i]--; //Убавляем количество связей у вершин
						gr.binds[j]--; //Убавляем количество связей у вершин
						temp--;
						break;
					}
				}
			}
		}
	}

	return gr;
}

//Расстановка весов
void weights(Grapth gr) {

	//Проходим по вершинам
	for (int i = 0; i < gr.count_of_vortex; i++) {
		for (int j = i + 1; j < gr.count_of_vortex; j++) {

			//Если есть связь
			if (gr.matrix_smezh[i][j] == 1) {

				//Размещаем вес на этой связи
				gr.matrix_smezh[i][j] = gr.matrix_smezh[j][i] = genWeight();
			}
			//Иначе нет пути
			else {
				gr.matrix_smezh[i][j] = gr.matrix_smezh[j][i] = INF; 
			}
		}
	}
}

//Удаление динамических массивов
void deleteDynamicArray(Grapth gr) {

	//Проход по вершинам
	for (int i = 0; i < gr.count_of_vortex; i++) {
		delete[] gr.matrix_smezh[i];
	}

	//Удаляем матрицу смежности
	delete[] gr.matrix_smezh;

	//Удаляем связи
	delete[] gr.binds;
}

//Соединение вершин
void edgeConnector(int* nodes, Grapth gr) {
	int not_bind_1 = 0, with_bind_1 = 0, not_bind_2 = 0, with_bind_2 = 0;

	//Проходимся по вершинам
	for (int i = 0; i < gr.count_of_vortex; i++) {

		//Если не связанная часть
		if (nodes[i] == 0) {
			not_bind_1 = i; 
		}
		//Связанная часть
		else { 
			with_bind_1 = i; 
		}
	}

	//Проходимся по вершинам
	for (int i = 0; i < gr.count_of_vortex; i++) {

		//Связанная вершина
		if (gr.matrix_smezh[not_bind_1][i] == 1) { 
			not_bind_2 = i;
		}

		//Связанная вершина
		if (gr.matrix_smezh[with_bind_1][i] == 1) { 
			with_bind_2 = i; 
		} 
	}

	gr.matrix_smezh[with_bind_1][with_bind_2] = gr.matrix_smezh[with_bind_2][with_bind_1] = 0; //Удаляем связь
	gr.matrix_smezh[with_bind_1][not_bind_1] = gr.matrix_smezh[not_bind_1][with_bind_1] = genWeight(); //Генерируем новую связь
	gr.matrix_smezh[not_bind_1][not_bind_2] = gr.matrix_smezh[not_bind_2][not_bind_1] = 0; //Удаляем связь
	gr.matrix_smezh[not_bind_2][with_bind_2] = gr.matrix_smezh[with_bind_2][not_bind_2] = genWeight(); //Генерируем новую связь
}

//Поиск в глубину
/*
* @param gr - Граф
* @param Stack - Стек для посещения
* @param nodes - Список посещенных вершин
* @param node - Вершина
*/
bool DFS(Grapth gr) {
	cout << "\nПоиск в глубину (Проверка на существование пути)\n";
	stack<int> Stack;

	//Массив прохода по вершинам графа
	int* nodes = new int[gr.count_of_vortex];

	//Изначально вершины не посещались
	for (int i = 0; i < gr.count_of_vortex; i++) {
		nodes[i] = 0;
	}

	//Помещаем в стек первую вершину
	Stack.push(0);

	//Пока в стеке есть вершины
	while (!Stack.empty())
	{
		//Извлекаем вершину
		int node = Stack.top();
		Stack.pop();

		//Если мы в этой вершине уже были
		if (nodes[node] == 2) { 
			continue; 
		}
		//Посещаем вершину
		nodes[node] = 2;

		//Находим смежные вершины для этой вершины
		for (int j = gr.count_of_vortex - 1; j >= 0; j--)
		{ 
			//Если находится связь и мы в этой вершине еще не были
			if (gr.matrix_smezh[node][j] != 0 && nodes[j] != 2)
			{
				Stack.push(j);  //Добавляем её в стек
				nodes[j] = 1; //Отмечаем вершину как обнаруженную
			}
		}
	}

	bool check = 1; //Проверка связности

	//Идем по вершинам
	for (int i = 0; i < gr.count_of_vortex; i++) {

		//Если вершина не была обнаружена
		if (nodes[i] == 0) {
			check = 0;

			//Исправляем связанность
			edgeConnector(nodes, gr);
			break;
		}
	}
	delete[] nodes;
	return check;
}

//Генерация списка ребер
/*
* @param gr - Граф
* @param temp - Список для записи
*
* @return gr - Граф
*/
Grapth genListReb(Grapth gr) {
	list<int> temp;
	int j;

	//Проходим по вершинам
	for (int i = 0; i < gr.count_of_vortex; i++) {
		j = i + 1;
		for (j; j < gr.count_of_vortex; j++) {

			//Если есть связь
			if (gr.matrix_smezh[i][j] != 0) {
				temp.push_back(i); //Записываем начало
				temp.push_back(j); //Записываем конец
				temp.push_back(gr.matrix_smezh[i][j]);
				//Добавляем ребро
				gr.list_edges.push_back(temp);
				temp.clear();
			}
		}
	}
	return gr;
}

//Генератор графа
/*
* @param i - Номер теста
* @param vortex[] - массив количества вершин для тестов
* @param min_bind[] - минимальное количество связей у вершины
* @param max_edge - Максимальное количество ребер
* @param min_edge - Минимальное количество ребер
* @param gr - Граф
*
* @return gr - Граф
*/
Grapth grapthGeneration(int i) {
	int vortex[] = { 10, 20, 50, 100 };
	int min_bind[] = { 3, 4, 10, 20 };
	int max_edge, min_edge;

	i /= tests;

	Grapth gr;
	gr.count_of_vortex = vortex[i]; //Вершины
	cout << "Вершин: " << gr.count_of_vortex << endl;

	min_edge = ceil((double)gr.count_of_vortex / 2) * min_bind[i]; //Минимум ребер
	max_edge = gr.count_of_vortex * (gr.count_of_vortex - 1) / 2; //Максимум ребер
	gr.count_of_edges = min_edge + rand() % (max_edge - min_edge + 1); //Ребра

	gr = genMatSmezh(gr, min_bind[i]); //Матрица смежности
	weights(gr); //Расстановка весов
	gr = genListReb(gr); //Список ребер

	return gr;
}

//Структура ребра для алгоритма Крускала
struct Node
{
	int start; //Начальная вершина
	int end; //Конечная вершина
	int weight; //Вес ребра
};

//Функция сравнения веса у разных ребер
bool compare(Node a, Node b)
{
	return a.weight < b.weight;
};

//Функция создания списка вершин в ребрах ОД
void KruskalEdgeNodes(int vortex, vector<int> &nodes, vector<Node> &KruskalEdges) {

	//Добавляем вершину в список вершин ОД
	nodes.push_back(vortex); 
	for (int j = 0; j < KruskalEdges.size(); j++) {

		//Добавляем в список вершин ОД все связные с этой вершиной вершины в ОД
		if (count(nodes.begin(), nodes.end(), KruskalEdges[j].start) == 0) {
			nodes.push_back(KruskalEdges[j].start);
		}
		if (count(nodes.begin(), nodes.end(), KruskalEdges[j].end) == 0) {
			nodes.push_back(KruskalEdges[j].end);
		}
	}

}

//Функция поиска вершины в списке
bool findNode(vector<int> nodes, int a, int b) {
	return (count(nodes.begin(), nodes.end(), a) > 0 && count(nodes.begin(), nodes.end(), b) == 0);
}

//Функция поиска последнего ребра для ОД
void graphConnect(vector<int> &nodes, int M, vector<Node> myedges, vector<Node> &KruskalEdges) {

	//Проходим по вершинам
	while (nodes.size() < M) {
		for (int i = 0; i < myedges.size(); i++) {

			//Если нет ребра в списке ребер ОД
			if (findNode(nodes, myedges[i].start, myedges[i].end)) {
				//Мы добавляем это ребро
				KruskalEdgeNodes(myedges[i].end, nodes, KruskalEdges);
				KruskalEdges.push_back(myedges[i]);

			}
			if (findNode(nodes, myedges[i].end, myedges[i].start)) {
				//Мы добавляем это ребро
				KruskalEdgeNodes(myedges[i].start, nodes, KruskalEdges);
				KruskalEdges.push_back(myedges[i]);
			}

		}
	}
}

//Алгоритм Крускала
/*
* @param M - Количество вершин
* @param N - Количество ребер
* @param visited - вектор посещения вершин
* @param myedges - вектор ребер
* @param edges - список ребер
* @param nodes - вектор вершин, которые уже описаны в остовном дереве
* @param KruskalEdges - вектор ребер, которые есть в остовном дереве
* @param weight - суммарный вес остовного дерева
* @param gr - Граф
*/
void Kruskal(Grapth gr)
{
	list<list<int>> edges = gr.list_edges; //Получаем временный список ребер
	int M, N;
	M = gr.count_of_vortex; //Количество вершин
	vector<bool> visited(M); //Вектор посещений
	N = gr.count_of_edges; //Количество ребер
	vector<Node> myedges(N); //Вектор ребер
	vector<int> nodes; //Вектор вершни в ОД
	vector<Node> KruskalEdges; //Вектор ребер в ОД
	int weight = 0; //Суммарный вес ОД

	//Заполняем вектор ребрами
	for (int i = 0; i < N; i++)
	{
		myedges[i].start = edges.front().front(); //Вытаскиваем начальную вершину
		edges.front().pop_front(); //Удаляем вершину из временного списка
		myedges[i].end = edges.front().front(); //Вытаскиваем конечную вершину
		edges.front().pop_front(); //Удаляем вершину из временного списка
		myedges[i].weight = edges.front().front(); //Вытаскиваем вес
		edges.pop_front(); //Удаляем вес из временного списка
	}

	//Сортируем вектор так, чтобы веса были по возрастанию
	sort(myedges.begin(), myedges.end(), compare);

	//Запихиваем в вектор вершин первую вершину
	nodes.push_back(myedges.front().start);

	//Вывод всех ребер
	/*
	for (int i = 0; i < N; i++)
	{
		cout << myedges[i].start << "---" << myedges[i].end << "---" << myedges[i].weight << endl;
	}

	cout << endl;
	cout << gr.count_of_edges << endl;
	*/

	cout << "Метод Краскала" << endl;
	for (int i = 0; i < N; i++)
	{
		//Если мы еще не посещали вершины этого ребра
		if (!visited[myedges[i].start] || !visited[myedges[i].end])
		{
			//Если в списке вершин ОД нет одной из вершины ребра
			if (findNode(nodes, myedges[i].start, myedges[i].end)) {
				nodes.push_back(myedges[i].end);
			}
			if (findNode(nodes, myedges[i].end, myedges[i].start)) {
				nodes.push_back(myedges[i].start);
			}

			weight += myedges[i].weight; //Прибавляем вес
			visited[myedges[i].start] = true; //Посещаем начало
			visited[myedges[i].end] = true; //Посещаем конец
			KruskalEdges.push_back(myedges[i]); //Добавляем ребро в список ребер ОД
		}
	}

	//Ищем последние ребра для полного остовного дерева
	graphConnect(nodes, M, myedges, KruskalEdges);

	//Выводим список ребер ОД
	/*for (int i = 0; i < KruskalEdges.size(); i++) {
		cout << KruskalEdges[i].start << "---" << KruskalEdges[i].end << "---" << KruskalEdges[i].weight << endl;
	}*/

	//Вывод вершин в списке ОД
	/*
	for (int i = 0; i < nodes.size(); i++) {
		cout << nodes[i] << endl;
	}*/
	cout << "Минимальный вес связующего дерева:";
	cout << weight << endl;
}


void main() {
	setlocale(LC_ALL, "Russian");
	srand(time(0));

	int M = 4 * tests;
	Grapth* gr = new Grapth[M];
	for (int i = 0; i < M; i++) {

		cout << "\nТест " << (i % tests) + 1 << endl;

		gr[i] = grapthGeneration(i); //Генерируем граф
		while (!DFS(gr[i])); //Проверка связности
		//gr[i].printMatrixSmezh(); //Вывод матрицы

		chrono::high_resolution_clock::time_point begin = chrono::high_resolution_clock::now(); //Засекаем время
		Kruskal(gr[i]); //Крускал
		chrono::high_resolution_clock::time_point end = chrono::high_resolution_clock::now(); //Останавливаем время

		chrono::duration<double, milli> milli_diff = end - begin; //Время в микросекундах
		cout << milli_diff.count() << endl;

		deleteDynamicArray(gr[i]); //Очищение массивов
	}
	delete[] gr;
}





