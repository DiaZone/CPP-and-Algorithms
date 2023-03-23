#include <iostream>
#include <algorithm>
#include <vector>
#include <list>
#include <random>
#include <stack>
#include <queue>
#include <iomanip>
#include <chrono>
#include <math.h>

using namespace std;

//Структура ребра графа
struct Edge {
	int begin; //Начальная вершина
	int end; //Конечная вершина
};

//Класс графа
/*
* @param count_of_vertex - Количество вершин
* @param count_of_edges - Количество ребер
* @param oriented - Ориентированность графа
* @param matrix_smezh - Матрица смежности
* @param matrix_inc - Матрица инцидентности
* @param list_smezh - Список смежности
* @param list_edges - Список ребер
* @param binds - Количество связанных вершин
*/
class Grapth	 {
public:
	int count_of_vertex;
	int count_of_edges;
	bool oriented;
	int** matrix_smezh;
	int** matrix_inc;
	list <list<int>> list_smezh;
	list <list<int>> list_edges;
	int* binds;

	//Вывод матрицы смежности
	void printMatrixSmezh() {
		cout << "Матрица смежности\n";
		cout << setw(4) << "|";
		for (int i = 0; i < count_of_vertex; i++) {
			cout << setw(2) << (char)(i + 65) << " ";
		}
		cout << endl;
		for (int i = 0; i < count_of_vertex; i++) {
			cout << setw(2) << (char)(i + 65) << " |";
			for (int j = 0; j < count_of_vertex; j++) {
				cout << setw(2) << matrix_smezh[i][j] << " ";
			}
			cout << endl;
		}
	}

	//Вывод матрицы инцидентности
	void printMatrixInc() {
		cout << "Матрица инцендентности\n";
		cout << setw(4) << "|";
		for (int i = 0; i < count_of_edges; i++) {
			cout << setw(2) << i + 1 << " ";
		}
		cout << endl;
		for (int i = 0; i < count_of_vertex; i++) {
			cout << setw(2) << (char)(i + 65) << " |";
			for (int j = 0; j < count_of_edges; j++) {
				cout << setw(2) << matrix_inc[i][j] << " ";
			}
			cout << endl;
		}
	}

	//Вывод списка смежности
	void printListSmezh() {
		list<list<int>> list = list_smezh;
		cout << "Список смежности\n";
		int i = 0;
		while (list.size() > 0) {
			cout << (char)(i + 65) << " | ";
			if (!list.front().empty()) {
				while (list.front().size() > 0) {
					cout << (char)(list.front().front() + 65) << " ";
					list.front().pop_front();
				}
			}
			else {
				cout << "-";
			}
			i++;
			list.pop_front();
			cout << endl;
		}
	}

	//Вывод списка ребер
	void printListReb() {
		cout << "Список ребер\n";
		list<list<int>> list = list_edges;
		int i = 0;
		while (list.size() > 0) {
			cout << i + 1 << " | " << (char)(list.front().front() + 65) <<
				" -> " << (char)(list.front().back() + 65) << endl;
			list.pop_front();
			i++;
		}
	}

};

//Генерация матрицы смежности
/*
* @param gr - Граф
* @param max_bind - Максимальное количество связей у вершины
* @param oriented - Ориентированность
* @param M - Максимально возможное количество ребер
* @param array_reb - Вектор ребер
* @param count - Количество расставленных ребер
*
* @return Граф
*/
Grapth genMatSmezh(Grapth gr, int max_bind, bool oriented) {

	int M = gr.count_of_vertex * (gr.count_of_vertex - 1) / 2; //Максимальное количество возможных ребер Количество мест
	vector <int> array_edges(M); //Нулевой вектор ребер
	for (int i = 0; i < gr.count_of_edges; i++) { //Заполняем первые элементы
		array_edges[i] = 1;
	}

	//Перемешиваем вектор
	random_shuffle(array_edges.begin(), array_edges.end());

	//Динамически выделяем память под массивы
	gr.matrix_smezh = new int* [gr.count_of_vertex];
	gr.binds = new int[gr.count_of_vertex];
	for (int i = 0; i < gr.count_of_vertex; i++) {
		gr.matrix_smezh[i] = new int[gr.count_of_vertex];
		gr.binds[i] = 0;
	}

	int k = 0, count = 0;

	//Проходим по вершинам
	for (int i = 0; i < gr.count_of_vertex; i++) {

		//Вершина сама в себя не указывает, поэтому ставим 0
		gr.matrix_smezh[i][i] = 0;

		//Идем выше главной диагонали
		for (int j = i + 1; j < gr.count_of_vertex; j++) {

			//Если есть ребро, но места на этой вершине уже нет, то ребро не ставится
			if (array_edges[k] != 0 && (gr.binds[i] == max_bind || gr.binds[j] == max_bind)) {
				gr.matrix_smezh[i][j] = gr.matrix_smezh[j][i] = 0;
				k++;
			}

			//Иначе ставим ребро и считаем сколько ребер ставится
			else {
				gr.matrix_smezh[i][j] = gr.matrix_smezh[j][i] = array_edges[k];
				k++;
				count++; //Счетчик количества ребер
			}

			//Прибавляем к количеству связей у вершин 
			if (abs(gr.matrix_smezh[i][j]) == 1) {
				gr.binds[i]++;
				gr.binds[j]++;
			}
		}
	}

	//Если есть непроставленные ребра, то находим место для него проходя по вершинам
	while (M - count > 0) {

		//Проходим по всем вершинам
		for (int i = 0; i < gr.count_of_vertex; i++) {

			// Если у вершины есть свободное место
			if (gr.binds[i] < max_bind) {
				for (int j = i + 1; j < gr.count_of_vertex; j++) {
					if (gr.matrix_smezh[i][j] == 0 && gr.binds[j] < max_bind) {
						gr.matrix_smezh[i][j] = gr.matrix_smezh[j][i] = 1;
						count++; //Записываем успешно поставленное ребро
						gr.binds[i]++; //Прибавляем к количеству связей у вершин
					}
					if (M - count <= 0) {
						break;
					}
				}
			}
			if (M - count < 0) { 
				break; 
			}
		}
	}

	return gr;
}

// Генерация матрицы инцидентности
/*
* @param gr - Граф
* @param oriented - Ориентированность
* @param temp_edges - Список ребер
*
* @return gr - Граф
*/
Grapth genMatInc(Grapth gr, bool oriented) {

	//Приравниваем к промежутному списку список ребер
	list<list<int>> temp_edges = gr.list_edges; 

	//Выделяем динамически память
	gr.matrix_inc = new int* [gr.count_of_vertex];
	for (int i = 0; i < gr.count_of_vertex; i++) {
		gr.matrix_inc[i] = new int[gr.count_of_edges];
	}

	//Заполняем матрицу 0
	for (int i = 0; i < gr.count_of_vertex; i++) {
		for (int j = 0; j < gr.count_of_edges; j++) {
			gr.matrix_inc[i][j] = 0;
		}
	}

	int i = 0;
	//Проходимся по списку
	while (temp_edges.size() > 0) {

		//Если граф ориентированный
		if (oriented) {
			gr.matrix_inc[temp_edges.front().front()][i] = 1; //Выход ребра
			gr.matrix_inc[temp_edges.front().back()][i] = -1; //Вход ребра

		}
		else {

			//Для исключения повторений ребер
			if (temp_edges.front().front() < temp_edges.front().back()) {
				gr.matrix_inc[temp_edges.front().front()][i] = gr.matrix_inc[temp_edges.front().back()][i] = 1;
			}
		}

		//Убираем записанное ребро из временного массива
		temp_edges.pop_front(); 
		i++;
	}

	return gr;
}

//Генерация списка смежности
/*
* @param gr - Граф
* @param temp - Список для записи
* 
* @return gr - Граф
*/
Grapth genListSmezh(Grapth gr) {
	list<int> temp;

	//Проходим по вершинам
	for (int i = 0; i < gr.count_of_vertex; i++) {
		for (int j = 0; j < gr.count_of_vertex; j++) {

			//Если есть связь(Мы смотрим ниже диагонали чтобы не нарушить ориентированность графа)
			if (gr.matrix_smezh[j][i] == 1) { 

				//Добавляем связь к этой вершине
				temp.push_back(j);
			}
		}

		//Записываем список связей для i-ой вершины
		gr.list_smezh.push_back(temp);
		temp.clear();
	}
	return gr;
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

	//Проходим по вершинам
	for (int i = 0; i < gr.count_of_vertex; i++) {
		for (int j = 0; j < gr.count_of_vertex; j++) {

			//Если есть связь
			if (gr.matrix_smezh[i][j] == 1) {
				temp.push_back(i); //Записываем начало
				temp.push_back(j); //Записываем конец

				//Добавляем ребро
				gr.list_edges.push_back(temp);
				temp.clear();
			}
		}
	}
	return gr;
}


//Получение количества входов/выходов у вершин
/*
* @param count - Количество входов/выходов
* 
* return count - Количество входов/выходов
*/
int countOfEntryExit(int** array, int i, int N, int e) {
	int count = 0;
	for (int j = 0; j < N; j++) {
		if (array[i][j] == e) count++;
	}
	return count;
}

//Дополнение матрицы смежности, если граф ориентирован
void genOriented(Grapth gr, int max_entry, int max_exit) {

	int a[] = { -1, 1 }; //Массив для выбора направления

	//Проходим по вершинам
	for (int i = 0; i < gr.count_of_vertex; i++) {
		for (int j = i + 1; j < gr.count_of_vertex; j++) {

			//Если есть связь
			if (gr.matrix_smezh[i][j] == 1) {

				//Проставляем случайные направления
				gr.matrix_smezh[i][j] = a[rand() % 2];
				gr.matrix_smezh[j][i] = -gr.matrix_smezh[i][j];
			}
		}
	}

	//Проверка на количество входов и выходов у вершины
	for (int i = 0; i < gr.count_of_vertex; i++) {

		//Если входов больше максимального количества входов
		if (countOfEntryExit(gr.matrix_smezh, i, gr.count_of_vertex, -1) > max_entry) {

			//Ищем связанную вершину
			for (int j = 0; j < gr.count_of_vertex; j++) {

				//Если в данной вершине имеется лишний вход, то мы смотрим, можно ли перевернуть связь таким образом, что 
				//вход будет из смежной вершины(Не превышено ли кол-во входов у смежной вершины)
				if (gr.matrix_smezh[i][j] == -1 && countOfEntryExit(gr.matrix_smezh, j, gr.count_of_vertex, -1) < max_entry) {

					//Если да, то переворачиваем связь
					swap(gr.matrix_smezh[i][j], gr.matrix_smezh[j][i]);
					i--;
					break;
				}

			}
		}
		//Если выходов больше максимального количества выходов
		if (countOfEntryExit(gr.matrix_smezh, i, gr.count_of_vertex, 1) > max_exit) {

			//Ищем связанную вершину
			for (int j = 0; j < gr.count_of_vertex; j++) {

				//Если в данной вершине имеется лишний выход, то мы смотрим, можно ли перевернуть связь таким образом, что 
				//выход будет из смежной вершины(Не превышено ли кол-во выходов у смежной вершины)
				if (gr.matrix_smezh[i][j] == 1 && countOfEntryExit(gr.matrix_smezh, j, gr.count_of_vertex, 1) < max_exit) {

					//Если да, то переворачиваем связь
					swap(gr.matrix_smezh[i][j], gr.matrix_smezh[j][i]);
					i--;
					break;
				}
			}
		}
	}

	//Заменяем -1 на 0 (Правильная матрица смежности)
	for (int i = 0; i < gr.count_of_vertex; i++) {
		for (int j = 0; j < gr.count_of_vertex; j++) {
			if (gr.matrix_smezh[i][j] == -1) {
				gr.matrix_smezh[i][j] = 0;
			}
		}
	}
}

//Удаление массива
void deleteDynamicArray(Grapth gr) {
	for (int i = 0; i < gr.count_of_vertex; i++) {
		delete[] gr.matrix_smezh[i];
	}
	delete[] gr.matrix_smezh;

	delete[] gr.binds;

	for (int i = 0; i < gr.count_of_vertex; i++) {
		delete[] gr.matrix_inc[i];
	}
	delete[] gr.matrix_inc;
}

//Поиск в ширину
/*
* @param gr - Граф
* @param start - Начальная вершина
* @param end - Конечная вершина
* @param Queue - Очередь для посещения
* @param Edges - Стек ребер
* @param edg - Ребро
* @param nodes - Список посещенных вершин
* @param node - Вершина
* @param begin - Начало отсчета времени
* @param stop - Окончание отсчета времени
* @param milli_diff - Время
*/
void BFS(Grapth gr, int start, int end) {
	cout << "\n\nПоиск в ширину (Нахождение кратчайшего пути)\n";

	chrono::high_resolution_clock::time_point begin = chrono::high_resolution_clock::now(); //Засекаем время
	queue<int> Queue;
	stack<Edge> Edges;
	Edge edg;

	//Массив прохода по вершинам графа
	int* nodes = new int[gr.count_of_vertex]; 

	//Изначально вершины не посещались
	for (int i = 0; i < gr.count_of_vertex; i++) {
		nodes[i] = 0;
	}

	//Помещаем в очередь первую вершину
	Queue.push(start); 

	//Пока очередь не пуста
	while (!Queue.empty())
	{
		//Вытаскиваем вершину
		int node = Queue.front();
		Queue.pop();

		//Отмечаем ее как посещенную
		nodes[node] = 2;

		//Находим смежные вершины для этой вершины
		for (int j = 0; j < gr.count_of_vertex; j++)
		{
			//Если находится связь и мы в этой вершине еще не были и не обнаруживалась ранее
			if (gr.matrix_smezh[node][j] == 1 && nodes[j] == 0)
			{
				Queue.push(j); //Добавляем ее в очередь
				nodes[j] = 1; //Отмечаем вершину как обнаруженную
				edg.begin = node; //Начало ребра - текущая вершина
				edg.end = j; //Конец ребра - обнаруженная вершина
				Edges.push(edg);
				if (node == end) {
					break; 
				}
			}
		}
	}

	chrono::high_resolution_clock::time_point stop = chrono::high_resolution_clock::now(); //Замеряем время на конец поиска
	chrono::duration<double, milli> milli_diff = stop - begin; //Время в наносекундах
	cout << "\nВремя (млсек): " << milli_diff.count() << endl;
	cout << "\nПуть до вершины " << end + 1 << endl;
	cout << end + 1;

	//Строим маршрут пока есть вершины
	while (!Edges.empty()) { 
		edg = Edges.top();
		Edges.pop();
		if (edg.end == end) {
			end = edg.begin; //Посещенная вершина
			cout << " <- " << end + 1;
		}
	}
	delete[] nodes;
}

//Поиск в глубину
/*
* @param gr - Граф
* @param start - Начальная вершина
* @param end - Конечная вершина
* @param Stack - Стек для посещения
* @param Edges - Стек ребер
* @param edg - Ребро
* @param nodes - Список посещенных вершин
* @param node - Вершина
* @param begin - Начало отсчета времени
* @param stop - Окончание отсчета времени
* @param milli_diff - Время
* @return gr - Граф
*/
Grapth DFS(Grapth gr) {
	cout << "\nПоиск в глубину (Проверка на существование пути)\n";
	chrono::high_resolution_clock::time_point begin = chrono::high_resolution_clock::now(); //Засекаем время
	int startv, endv;
	Edge edg;
	stack<int> Stack;
	stack<Edge> Edges;

	//Массив прохода по вершинам графа
	int* nodes = new int[gr.count_of_vertex]; 

	//Изначально вершины не посещались
	for (int i = 0; i < gr.count_of_vertex; i++) { 
		nodes[i] = 0;
	}
	startv = rand() % gr.count_of_vertex; //Выбираем стартовую вершину
	cout << "\n" << startv;
	endv = rand() % gr.count_of_vertex; //Выбираем конечную вершину
	cout << "\n" << endv << endl;

	//Помещаем в стек первую вершину
	Stack.push(startv); 

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
		nodes[node] = 2; // отмечаем ее как посещенную

		//Находим смежные вершины для этой вершины
		for (int j = gr.count_of_vertex - 1; j >= 0; j--)
		{
			//Если находится связь и мы в этой вершине еще не были
			if (gr.matrix_smezh[node][j] == 1 && nodes[j] != 2)
			{
				Stack.push(j); // добавляем ее в cтек
				nodes[j] = 1; // отмечаем вершину как обнаруженную
				edg.begin = node; //Начало ребра - текущая вершина
				edg.end = j; //Конец ребра - обнаруженная вершина
				Edges.push(edg); //Добавляем ребро в стек ребер 

				//Если пришли на место
				if (node == endv) {
					break;
				}
			}
		}
	}
	chrono::high_resolution_clock::time_point stop = chrono::high_resolution_clock::now(); //Замеряем время на конец поиска
	chrono::duration<double, milli> milli_diff = stop - begin; //Время в наносекундах
	cout << "\nВремя (млсек): " << milli_diff.count() << endl;
	cout << "Путь до вершины " << endv + 1 << endl;
	cout << endv + 1;
	bool temp = 0; //Существует ли путь
	int end = endv;

	//Проходим по стеку ребер
	while (!Edges.empty())
	{
		//Вытаскиваем ребро
		edg = Edges.top();
		Edges.pop();
		
		//Если это ребро приводит в нужную вершину
		if (edg.end == endv)
		{
			endv = edg.begin;
			cout << " <- " << endv + 1; //Выписываем посещенную вершину
			temp = 1;
		}
	}
	delete[] nodes;
	if (!temp) {
		cout << "\nотсутствует";
	}
	
	//Вызываем поиск в ширину для тех же условий
	BFS(gr, startv, end); 
	return gr;
}

//Генератор графа
/*
* @param i - Номер теста
* @param max_vortex - Максимальное количество вершин
* @param min_vortex - Минимальное количество вершин
* @param max_edge - Максимальное количество ребер
* @param min_edge - Минимальное количество ребер
* @param max_bind - Максимальное количество связей у вершины
* @param oriented - Ориентированность
* @param max_entry - Максимальное количество входов
* @param max_exit - Максимальное количество выходов
* @param gr - Граф
* 
* @return gr - Граф
*/
Grapth grapthGeneration(int i) {
	int max_vortex = 100, min_vortex = 10;
	int max_edge, min_edge = 10;
	int max_bind;
	bool oriented;
	int max_entry, max_exit;

	Grapth gr;

	gr.count_of_vertex = (max_vortex) / 10 * (i + 1); //Вершины
	gr.count_of_edges = min_edge + (i * min_edge * 2); //Ребра
	max_bind = ceil(((double)gr.count_of_edges * 2) / (double)gr.count_of_vertex) + rand() % (gr.count_of_vertex + 1); //Количество связей
	max_entry = max_exit = max_bind / 2 + 1; //Входы/выходы
	oriented = rand() % 2; //Ориентированность

	cout << "\nВершин: " << gr.count_of_vertex << "\nРебер: " << gr.count_of_edges <<
		"\nМаксимальное количество связей: " << max_bind <<
		"\nОриентированный: " << oriented <<
		"\nМаксимальное количество входов: " << max_entry <<
		"\nМаксимальное количество выходов: " << max_exit << endl;

	gr = genMatSmezh(gr, max_bind, oriented); //Матрица смежности
	if (oriented) genOriented(gr, max_entry, max_exit); //Ориентированность
	gr = genListReb(gr); //Список ребер
	gr = genListSmezh(gr); //Список смежности
	gr = genMatInc(gr, oriented); //Матрица инцидентности

	gr.printMatrixSmezh(); //Вывод матрицы смежности
	gr.printListSmezh(); //Вывод списка смежности
	gr.printListReb(); //Вывод списка ребер
	gr.printMatrixInc(); //Вывод матрицы инцидентности

	return gr;
}

void main() {
	setlocale(LC_ALL, "Russian");
	srand(time(0));

	int M = 10;
	Grapth* gr = new Grapth[M];
	for (int i = 0; i < M; i++) {
		cout << "\nТест №" << i + 1 << endl;
		gr[i] = grapthGeneration(i);
		gr[i] = DFS(gr[i]);
	}
	delete[] gr;
}
