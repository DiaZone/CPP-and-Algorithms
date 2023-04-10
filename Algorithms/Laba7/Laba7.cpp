#include <iostream>
#include <random>
#include <chrono>
#include <list>
#include <vector>
#include <fstream>

using namespace std;

//Структура узла
struct node {
	int element; //Значение элемента
	int size; //Размер узла
	node* left_child; //Левый потомок
	node* right_child; //Правый потомок

	//Конструктор по-умолчанию
	node(int k) { 
		element = k; 
		left_child = 0;
		right_child = 0;
		size = 1; 
	}
};

//Получение размера узла
int getSize(node* p) {
	if (!p) {
		return 0;
	}
	return p->size;
}

//Расчет размера узла
void fixSize(node* p) {
	p->size = getSize(p->left_child) + getSize(p->right_child) + 1;
}

//Фактор балансировки
int bfactor(node* p) {
	return getSize(p->right_child) - getSize(p->left_child);
}

//Правый поворот вокруг узла p
node* rotateRight(node* p) {

	//Проверяем существует ли левый потомок
	node* q = p->left_child; //Левый потомок
	if (!q) { 
		return p; 
	}

	//Перестановка узлов
	p->left_child = q->right_child;
	q->right_child = p;
	q->size = p->size;

	//Фиксируем размер
	fixSize(p); 
	return q;
}

//Левый поворот вокруг узла p
node* rotateLeft(node* q) {

	//Проверяем существует ли правый потомок
	node* p = q->right_child; //Правый потомок
	if (!p) { 
		return q; 
	}

	//Перестановка узлов
	q->right_child = p->left_child;
	p->left_child = q;
	p->size = q->size;

	//Фиксируем размер
	fixSize(q);
	return p;
}

//Балансировка
node* balancer(node* p) {

	fixSize(p); //Фиксирование размера

	//Если правый потомок глубже левого
	if (bfactor(p) == 2) {

		//Если у правого потомка левый потомок глубже
		if (bfactor(p->right_child) < 0) {
			p->right_child = rotateRight(p->right_child); //Правый поворот вокруг правого потомка
		}
		return rotateLeft(p); //Левый поворот вокруг узла
	}

	//Если по левый потомок глубже правого
	if (bfactor(p) == -2) {

		//Если у левого потомка правый потомок глубже
		if (bfactor(p->left_child) > 0)
			p->left_child = rotateLeft(p->left_child); //Левый поворот вокруг левого потомка
		return rotateRight(p); //Правый поворот вокруг узла
	}

	//Если не требуется балансировка
	return p;
}

//Добавление узла
node* addNode(int k, node* p, bool AVL) {

	//Если есть свободное место
	if (!p) { 
		return new node(k); 
	}

	//Если свободного места нет
	//Если значение меньше или равно текущему узлу
	if (k <= p->element) {
		p->left_child = addNode(k, p->left_child, AVL); //Идем искать в левую ветку
		return AVL ? balancer(p) : rotateRight(p);
	}
	else {
		p->right_child = addNode(k, p->right_child, AVL); //Идем искать в правую ветку
		return AVL ? balancer(p) : rotateLeft(p);
	}
}


//Поиск минимального узла в дереве
node* findMinimum(node* p) {
	return p->left_child ? findMinimum(p->left_child) : p;
}

//Удаление минимального узла в дереве
node* removeMinimum(node* p) {

	//Если нет левого потомка
	if (p->left_child == 0) {
		return p->right_child;
	}
	//Иначе рекурсивно удаляем
	p->left_child = removeMinimum(p->left_child);
	return balancer(p); //Балансировка
}

//Объединение деревьев
node* joinTree(node* p, node* q) {

	//Если нет левого потомка, возвращаем правого
	if (!p) { 
		return q; 
	}

	//Если нет правого потомка, возвращаем левого
	if (!q) { 
		return p; 
	}

	//Cлучайное объединение
	if (rand() % (p->size + q->size) < p->size) {
		p->right_child = joinTree(p->right_child, q); //Идем в первое дерево
		fixSize(p); //Фиксируем размер
		return p;
	}
	else {
		q->left_child = joinTree(p, q->left_child); //Идем во второе дерево
		fixSize(q); //Фиксируем размер
		return q;
	}
}

//Объединение AVL-деревьев
node* joinAVLTree(node* q, node* r) {

	//Если правого потомка нет 
	if (!r) {
		return q;
	}
	node* min = findMinimum(r); //Ищем минимальное значение в правом потомке
	min->right_child = removeMinimum(r); //Удаляем минимальное 
	min->left_child = q;
	return balancer(min);
}

//Удаление из дерева p первого найденного узла со значением k 
node* remove(node* p, int k, bool AVL) {
	if (!p) { 
		return p; 
	}

	//Если найден нужный узел
	if (p->element == k) {
		node* q;

		//Если это AVL дерево
		if (AVL) {
			q = joinAVLTree(p->left_child, p->right_child); //Объединение узлов
		}
		else {
			q = joinTree(p->left_child, p->right_child); //Объединение узлов
		}
		delete p; //Удаление узла
		return q;
	}

	//Если узел не найден
	else if (k < p->element) {
		p->left_child = remove(p->left_child, k, AVL); //Идем в левую ветку
	}
	else {
		p->right_child = remove(p->right_child, k, AVL); //Идем в правую ветку
	}

	//Если это AVL дерево
	if (AVL) { 
		return balancer(p); //Балансировка
	} 
	else { 
		return p; 
	}
}

//Проход по дереву
vector<int> treePassing(node* p, int level, vector<int>& a) {

	if (p) {

		//Если конец ветки
		if (p->left_child == NULL && p->right_child == NULL) { 
			a.push_back(level); //Записываем глубину
		}
		//Рекурсивная функция для левого поддерева
		treePassing(p->right_child, level + 1, a);
		//Рекурсивная функция для правого поддерева
		treePassing(p->left_child, level + 1, a);
	}
	return a;
}

//Вывод всех глубин веток
void printTreePassing(vector<int> a) {
	ofstream out1;
	out1.open("D:\\Games\\Laba7\\output_alltree.txt", ios::app);
	out1 << "\n";
	for (int i = 0; i < a.size(); i++) {
		out1 << a[i] << " ";
	}
	out1 << "\n";
	out1.close();
}

//Поиск в дереве
void reverseSearch(node* p, int k) {

	//Если не нашли нужный узел
	if (p->element != k) {
		if (k < p->element) { 
			reverseSearch(p->left_child, k); //Рекурсивная функция для левого поддерева
		}
		else { 
			reverseSearch(p->right_child, k); //Рекурсивная функция для правого поддерева
		}
	}
}

//Вставка N элементов в дерево
node* insertNTree(vector<char> a, int N, bool AVL) {
	struct node* root = 0; //Дерево

	for (int i = 0; i < N; i++) {
		root = addNode((int)a[i], root, AVL); //Добавляем узел в дерево
	}
	return root;
}

//Dывод максимальной глубины дерева
void maxsize(vector<int> a) {
	ofstream out1;
	out1.open("D:\\Games\\Laba7\\output_maxtree.txt", ios::app);
	out1 << *max_element(a.begin(), a.end());
	out1 << endl;
	out1.close();
}

//Освобождение памяти
void freeMemory(node* tree) {

	//Если в дереве есть узлы
	if (tree != NULL) {
		freeMemory(tree->left_child); //Рекурсивное удаление левой ветки
		freeMemory(tree->right_child); //Рекурсивное удаление правой ветки
		delete tree; //Удаляем корень
	}
}

int main() {
	struct node* AVL_root = 0; //AVL-дерево
	struct node* rand_root = 0; //Рандомизированное дерево
	int series = 5; //Количество серий
	int	cycle = 50; //Количество циклов
	int	operations = 1000; //Количество операций
	int count_of_elements; // Количество элементов в дереве
	vector<char> elements; // Вектор элементов
	vector<int> rand_depths, AVL_depths;

	chrono::high_resolution_clock::time_point begin; //Стартовое время
	chrono::high_resolution_clock::time_point end; //Конечное время
	chrono::duration<double, milli> milli_diff;//Разница времен

	srand(time(0));
	setlocale(LC_ALL, "Russian");

	ofstream out;
	out.open("D:\\Games\\Laba7\\output.txt");
	ofstream out1;
	out1.open("D:\\Games\\Laba7\\output_alltree.txt");
	out1.close();
	ofstream out2;
	out2.open("D:\\Games\\Laba7\\output_maxtree.txt");
	out2.close();

	for (int ser = 0; ser < 5; ser++) {
		count_of_elements = pow(2, 10 + ser);

		for (int c = 0; c < cycle; c++) {

			cout << "\nСерия " << ser << " цикл " << c << endl;

			for (int i = 0; i < count_of_elements; i++) {
				elements.push_back(1 + rand() % count_of_elements); //Вставка случайных чисел
			}

			//Создание деревьев
			rand_root = insertNTree(elements, count_of_elements, 0); //Рандомизированное дерево
			AVL_root = insertNTree(elements, count_of_elements, 1); //AVL дерево

			//Создаем случайные значения
			vector<char> t;
			for (int i = 0; i < operations; i++) {
				t.push_back(1 + rand() % count_of_elements);
				elements.push_back(t.back());
			}

			/*Операция вставки в рандомизированное дерево*/
			begin = chrono::high_resolution_clock::now(); //Засекаем начальное время
			for (int i = 0; i < operations; i++) {
				rand_root = addNode(t[i], rand_root, 0); //Вставка в рандомизированное дерево
			}
			end = chrono::high_resolution_clock::now(); //Засекаем конечное время
			milli_diff = end - begin; //Время в микросекундах
			out << milli_diff.count() << " " << milli_diff.count() / operations << endl;
			/******************/


			/*Операция вставки в AVL дерево*/
			begin = chrono::high_resolution_clock::now(); //Засекаем начальное время
			for (int i = 0; i < operations; i++) {
				AVL_root = addNode(t[i], AVL_root, 1); //Вставка в AVL дерево
			}
			end = chrono::high_resolution_clock::now(); //Засекаем конечное время
			milli_diff = end - begin; //Время в микросекундах
			out << milli_diff.count() << " " << milli_diff.count() / operations << endl;
			/******************/

			//Глубина каждой ветки
			rand_depths = treePassing(rand_root, 0, rand_depths); 
			AVL_depths = treePassing(AVL_root, 0, AVL_depths);

			//Максимальная глубина у дерева
			if (ser == series - 1) {
				//maxsize(rand_depths);
				maxsize(AVL_depths);
			}

			random_shuffle(elements.begin(), elements.end()); //Перемешивание вектора
			vector<char> b = elements; //Создаем временный вектор из вектора элементов

			/*Операция удаления в рандомизированном дереве*/
			begin = chrono::high_resolution_clock::now(); //Засекаем начальное время
			for (int i = 0; i < operations; i++) {
				rand_root = remove(rand_root, (int)b.back(), 0);
				b.pop_back();
			}
			end = chrono::high_resolution_clock::now(); //Засекаем конечное время
			milli_diff = end - begin; //Время в микросекундах
			out << milli_diff.count() << " " << milli_diff.count() / operations << endl;
			/******************/


			/*Операция удаления в AVL дереве*/
			begin = chrono::high_resolution_clock::now(); //Засекаем начальное время
			for (int i = 0; i < operations; i++) {
				AVL_root = remove(AVL_root, (int)elements.back(), 1);
				elements.pop_back();
			}
			end = chrono::high_resolution_clock::now(); //Засекаем конечное время
			milli_diff = end - begin; //Время в микросекундах
			out << milli_diff.count() << " " << milli_diff.count() / operations << endl;
			/******************/


			/*Операция поиска в рандомизированном дереве*/
			begin = chrono::high_resolution_clock::now(); //Засекаем начальное время
			for (int i = 0; i < operations; i++) {
				reverseSearch(rand_root, (int)elements[i]);
			}
			end = chrono::high_resolution_clock::now(); //Засекаем конечное время
			milli_diff = end - begin; //Время в микросекундах
			out << milli_diff.count() << " " << milli_diff.count() / operations << endl;
			/******************/


			/*Операция поиска в AVL дереве*/
			begin = chrono::high_resolution_clock::now(); //Засекаем начальное время
			for (int i = 0; i < operations; i++) {
				reverseSearch(AVL_root, (int)elements[i]);
			}
			end = chrono::high_resolution_clock::now(); //Засекаем конечное время
			milli_diff = end - begin; //Время в микросекундах
			out << milli_diff.count() << " " << milli_diff.count() / operations << endl;
			/******************/

			//Освобождение глубины
			rand_depths.clear();
			AVL_depths.clear();

			//Снова смотрим глубину веток
			rand_depths = treePassing(rand_root, 0, rand_depths);
			AVL_depths = treePassing(AVL_root, 0, AVL_depths);

			if (ser == series - 1) {
				printTreePassing(rand_depths);
				printTreePassing(AVL_depths);
			}
			
			//Освобождение памяти
			elements.clear();
			t.clear();
			b.clear();
			freeMemory(rand_root);
			freeMemory(AVL_root);
		}
	}
	cout << "Programm end!";
	out.close();
	return 0;
}
