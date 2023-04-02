#include <iostream>
#include <random>
#include <vector>
#include <string>
#include <fstream>
#include <chrono>

using namespace std;

//Структура узла
struct node {
	int element; //Значение элемента
	unsigned char child_lvl; //Глубина узла
	node* left_child; //Левый потомок
	node* right_child; //Правый потомок

	//Конструктор по-умолчанию
	node(int k) { 
		element = k; 
		left_child = 0;
		right_child = 0;
		child_lvl = 1; 
	} 
};

//Получение глубины узла
unsigned char depth(node* p) {
	return p ? p->child_lvl : 0;
}

//Фактор балансировки
int bfactor(node* p) {
	return depth(p->right_child) - depth(p->left_child);
}

//Фиксирование глубины узла
void fixDepth(node* p) {
	unsigned char hl = depth(p->left_child); //Глубина левого потомка
	unsigned char hr = depth(p->right_child); //Глубина правого потомка
	p->child_lvl = (hl > hr ? hl : hr) + 1; //Определение глубины потомка
}

//Правый поворот вокруг узла
node* rotateRight(node* p) {

	//Перестановка потомков
	node* q = p->left_child; //Левый потомок
	p->left_child = q->right_child;
	q->right_child = p;

	//Фиксирование глубины
	fixDepth(p); 
	fixDepth(q);
	return q;
}

//Левый поворот вокруг узла
node* rotateLeft(node* q) {

	//Перестановка потомков
	node* p = q->right_child; //Правый потомок
	q->right_child = p->left_child;
	p->left_child = q;

	//Фиксирование глубины
	fixDepth(q);
	fixDepth(p);
	return p;
}

//Балансировка
node* balancer(node* p) {

	fixDepth(p); //Фиксирование глубины

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
		if (bfactor(p->left_child) > 0) { 
			p->left_child = rotateLeft(p->left_child); //Левый поворот вокруг левого потомка
		}
		return rotateRight(p); // правый поворот вокруг узла

	}

	//Если не требуется баласировка
	return p;
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

//Удаление узла
/* @param p - узел
* @param value - удаляемое значение
* @param AVL - сбаласированное ли дерево
* @return - возвращает дерево с удаленным узлом
*/
node* removeNode(node* p, int value, bool AVL) {

	//Если узла нет
	if (!p) { 
		return 0; 
	}

	//Если значение меньше текущего узла
	if (value < p->element) {
		p->left_child = removeNode(p->left_child, value, AVL); //Идем в левого потомка
	} 

	//Если значение больше текущего узла
	else if (value > p->element) {
		p->right_child = removeNode(p->right_child, value, AVL); //Идем в правого потомка
	}

	//Если нашли нужный узел
	else { 
		node* q = p->left_child; //Левый потомок
		node* r = p->right_child; //Правый потомок
		delete p; //Удаляем текущий узел

		//Если правого потомка нет 
		if (!r) { 
			return q; 
		}
		node* min = findMinimum(r); //Ищем минимальное значение в правом потомке
		min->right_child = removeMinimum(r); //Удаляем минимальное 
		min->left_child = q;
		return balancer(min);
	}

	//Если необходимо сбалансировать
	if (AVL) { 
		return balancer(p);
	} 
	else { 
		return p; 
	}
}

//Проход по дереву
void passing(node* p, int value) {

	//Если не нашли нужный узел
	if (p->element != value) { 

		//Рекурсивная функция для левого поддерева
		if (value < p->element) { 
			passing(p->left_child, value); 
		}

		//Рекурсивная функция для правого поддерева
		else { 
			passing(p->right_child, value); 
		}
	}
}

//Добавление узла
node* addNode(int k, node* p, bool AVL) {

	//Если есть свободное место
	if (!p) { 
		return new node(k); 
	} 

	//Если свободного места нет
	//Если значение меньше или равно текущему узлу
	if (k <= p->element) 
	{
		p->left_child = addNode(k, p->left_child, AVL); //Идем в левую ветку
	}  
	else
	{
		p->right_child = addNode(k, p->right_child, AVL); //Идем в правую ветку
	} 

	//Если необходимо сбалансировать
	if (AVL) { 
		return balancer(p); 
	} 
	else { 
		return p; 
	}
}

//Освобождение памяти
void freeMemory(node* tree) {

	//Если в дереве есть узлы
	if (tree != NULL) {	
		freeMemory(tree->left_child); //Рекурсивное удаление левой ветки
		freeMemory(tree->right_child);  //Рекурсивное удаление правой ветки
		delete tree; //Удаляем корень
	}
}

//Вставка в дерево 1000 элементов
node* insert1000Tree(vector<char> a, int count_of_elements, bool AVL) {
	struct node* root = 0; //Дерево
	for (int i = 0; i < count_of_elements; i++) {
		root = addNode((int)a[i], root, AVL); //Добавляем узел в дерево
	}
	return root;
}

//Тестирование работы
int main() {
	struct node* AVL_root = 0; // AVL деревце
	struct node* root = 0; // Бинарное деревце
	int series = 10; //Кол-во серий
	int cycle = 20; //Кол-во циклов
	int operations = 1000; // Кол-во операций
	int count_of_elements; //Количество элементов в дереве
	vector<char> elements; //Вектор элементов

	chrono::high_resolution_clock::time_point begin; //Стартовое время
	chrono::high_resolution_clock::time_point end; //Конечное время
	chrono::duration<double, milli> milli_diff; //Разница времен

	srand(time(0));
	setlocale(LC_ALL, "Russian");

	ofstream out;
	out.open("D:\\Games\\Laba6\\output.txt");

	for (int ser = 0; ser < series; ser++) {
		count_of_elements = pow(2, 10 + ser);

		for (int c = 0; c < cycle; c++) {

			//Первые 10 серий - случайные значения
			if (c < 10) {
				for (int i = 0; i < count_of_elements; i++) {
					elements.push_back(1 + rand() % count_of_elements);
				}
			}

			//Остальные 10 - последовательные значения
			else {
				for (int i = 0; i < count_of_elements; i++) {
					elements.push_back(i + 1);
				}
			}


			/*Операция вставки в обычное дерево*/
			begin = chrono::high_resolution_clock::now(); //Засекаем начальное время
			root = insert1000Tree(elements, count_of_elements, 0); //Вставка в обычное дерево
			end = chrono::high_resolution_clock::now(); //Засекаем конечное время
			milli_diff = end - begin; //Время в микросекундах
			out << milli_diff.count() << " " << milli_diff.count() / operations << endl;
			/******************/


			/*Операция вставки в AVL дерево*/
			begin = chrono::high_resolution_clock::now(); //Засекаем начальное время
			AVL_root = insert1000Tree(elements, count_of_elements, 1); //Вставка в AVL дерево
			end = chrono::high_resolution_clock::now(); //Засекаем конечное время
			milli_diff = end - begin; //Время в микросекундах
			out << milli_diff.count() << " " << milli_diff.count() / operations << endl;
			/******************/

			random_shuffle(elements.begin(), elements.end()); //Перемешивание вектора

			/*Операция поиска в обычное дереве*/
			begin = chrono::high_resolution_clock::now(); //Засекаем начальное время
			for (int i = 0; i < operations; i++) {
				passing(root, (int)elements[i]);
			}
			end = chrono::high_resolution_clock::now(); //Засекаем конечное время
			milli_diff = end - begin; //Время в микросекундах
			out << milli_diff.count() << " " << milli_diff.count() / operations << endl;
			/******************/


			/*Операция поиска в AVL дереве*/
			begin = chrono::high_resolution_clock::now(); //Засекаем начальное время
			for (int i = 0; i < operations; i++) {
				passing(AVL_root, (int)elements[i]);
			}
			end = chrono::high_resolution_clock::now(); //Засекаем конечное время
			milli_diff = end - begin; //Время в микросекундах
			out << milli_diff.count() << " " << milli_diff.count() / operations << endl;
			/******************/


			/*Операция поиска в массиве*/
			begin = chrono::high_resolution_clock::now(); //Засекаем начальное время
			for (int i = 0; i < operations; i++) {
				int k = (int)elements[rand() % elements.size()];
				for (int i = 0; i < count_of_elements; i++) {
					if (k == (int)elements[i]) break;
				}
			}
			end = chrono::high_resolution_clock::now(); //Засекаем конечное время
			milli_diff = end - begin; //Время в микросекундах
			out << milli_diff.count() << " " << milli_diff.count() / operations << endl;
			/******************/


			vector<char> b = elements; //Создаем временный вектор из вектора а

			/*Операция удаления в обычном дереве*/
			begin = chrono::high_resolution_clock::now(); //Засекаем начальное время
			for (int i = 0; i < operations; i++) {
				root = removeNode(root, (int)b.back(), 0);
				b.pop_back();
			}
			end = chrono::high_resolution_clock::now(); //Засекаем конечное время
			milli_diff = end - begin; //Время в микросекундах
			out << milli_diff.count() << " " << milli_diff.count() / operations << endl;
			/******************/


			/*Операция удаления в AVL дереве*/
			begin = chrono::high_resolution_clock::now(); //Засекаем начальное время
			for (int i = 0; i < operations; i++) {
				AVL_root = removeNode(AVL_root, (int)elements.back(), 1);
				elements.pop_back();
			}
			end = chrono::high_resolution_clock::now(); //Засекаем конечное время
			milli_diff = end - begin; //Время в микросекундах
			out << milli_diff.count() << " " << milli_diff.count() / operations << endl;
			/******************/

			//Освобождение памяти
			elements.clear();
			b.clear();
			freeMemory(root); 
			freeMemory(AVL_root);
		}
	}
	out.close();
	return 0;
}
