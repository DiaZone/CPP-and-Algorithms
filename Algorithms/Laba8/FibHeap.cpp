#include <cmath>
#include <cstdlib>
#include <iostream>
#include <malloc.h>
#include <vector>
#include <chrono>
#include <random>
#include <string>
#include <fstream>
using namespace std;

//Структура узла
struct node {
	node* parent; //Указатель на родителя
	node* child; //Указатель на ребенка
	node* left; //Указатель на левый элемент
	node* right; //Указатель на правый элемент
	int key; //Значение узла
	int degree; //Количество дочерних узлов
};

//Указатель на минимальный элемент
struct node* mini = NULL;

//Количество элементов в куче
int no_of_nodes = 0;

//Функция добавления узла
void insertion(int val)

{	//Инициализируем новое значение
	struct node* new_node = new node();
	new_node->key = val;
	new_node->degree = 0;
	new_node->parent = NULL;
	new_node->child = NULL;
	new_node->left = new_node;
	new_node->right = new_node;

	//Если есть минимальный элемент, ищем куда разместить новый
	if (mini != NULL) {
		(mini->left)->right = new_node;
		new_node->right = mini;
		new_node->left = mini->left;
		mini->left = new_node;

		//Если новое значение меньше минимального
		if (new_node->key < mini->key) {
			mini = new_node;
		}
	}

	//Иначе этот элемент становится минимальным
	else {
		mini = new_node;
	}
	no_of_nodes++;
}

//Связывание узлов по типу родитель-ребенок
void Fibonnaci_link(struct node* ptr2, struct node* ptr1)
{
	(ptr2->left)->right = ptr2->right;
	(ptr2->right)->left = ptr2->left;
	if (ptr1->right == ptr1) {
		mini = ptr1;
	}
	ptr2->left = ptr2;
	ptr2->right = ptr2;
	ptr2->parent = ptr1;
	if (ptr1->child == NULL) {
		ptr1->child = ptr2;
	}
	ptr2->right = ptr1->child;
	ptr2->left = (ptr1->child)->left;
	((ptr1->child)->left)->right = ptr2;
	(ptr1->child)->left = ptr2;
	if (ptr2->key < (ptr1->child)->key) {
		ptr1->child = ptr2;
	}
	ptr1->degree++;
}

//Консолидация(Слияние)
void Consolidate()
{
	int temp1;
	float temp2 = (log(no_of_nodes)) / (log(2));
	int temp3 = temp2+1;

	vector<node*> arr;
	for (int i = 0; i <= temp3; i++) {
		arr.push_back(NULL);
	}
	node* ptr1 = mini;
	node* ptr2;
	node* ptr3;
	node* ptr4 = ptr1;
	do {
		ptr4 = ptr4->right;
		temp1 = ptr1->degree;
		while (arr[temp1] != NULL) {
			ptr2 = arr[temp1];
			if (ptr1->key > ptr2->key) {
				ptr3 = ptr1;
				ptr1 = ptr2;
				ptr2 = ptr3;
			}
			if (ptr2 == mini) {
				mini = ptr1;
			}
			Fibonnaci_link(ptr2, ptr1);
			if (ptr1->right == ptr1) {
				mini = ptr1;
			}
			arr[temp1] = NULL;
			temp1++;
		}
		arr[temp1] = ptr1;
		ptr1 = ptr1->right;
	} while (ptr1 != mini);
	mini = NULL;
	for (int j = 0; j <= temp3; j++) {
		if (arr[j] != NULL) {
			arr[j]->left = arr[j];
			arr[j]->right = arr[j];
			if (mini != NULL) {
				(mini->left)->right = arr[j];
				arr[j]->right = mini;
				arr[j]->left = mini->left;
				mini->left = arr[j];
				if (arr[j]->key < mini->key)
					mini = arr[j];
			}
			else {
				mini = arr[j];
			}
			if (mini == NULL)
				mini = arr[j];
			else if (arr[j]->key < mini->key)
				mini = arr[j];
		}
	}
	arr.clear();
}

//Удаление минимального
void Extract_min()
{
	//Если значения нет
	if (mini == NULL) {
		cout << "Куча пуста" << endl;
	}
	else {
		node* temp = mini;
		node* pntr;
		pntr = temp;
		node* x = NULL;
		if (temp->child != NULL) {
			x = temp->child;
			do {
				pntr = x->right;
				(mini->left)->right = x;
				x->right = mini;
				x->left = mini->left;
				mini->left = x;
				if (x->key < mini->key)
					mini = x;
				x->parent = NULL;
				x = pntr;
			} while (pntr != temp->child);
		}
		(temp->left)->right = temp->right;
		(temp->right)->left = temp->left;
		mini = temp->right;
		if (temp == temp->right && temp->child == NULL)
			mini = NULL;
		else {
			mini = temp->right;
			Consolidate();
		}
		no_of_nodes--;
	}
}

//Поиск минимального
int Find(struct node* mini)
{
	return mini->key;
}


int main() {
	
	int k;
	srand(time(0));
	system("chcp 1251");
	system("cls");

	int test = 5; //Количество тестов
	int N, count = 1000; //Количество операций
	double all_time = 0;
	vector<double> times;
	chrono::high_resolution_clock::time_point begin;
	chrono::high_resolution_clock::time_point end;
	chrono::high_resolution_clock::time_point begin_max;
	chrono::high_resolution_clock::time_point end_max;
	chrono::duration<double, milli> milli_diff;
	chrono::duration<double, milli> milli_diff_max;

	ofstream out;
	out.open("D:\\Games\\Laba8Fibonach\\fibonachOutput2.txt");

	for (int i = 0; i < test; i++) {
		no_of_nodes = 0;
		N = pow(10, 3 + i); //Рассчитываем количество элементов
		for (int j = 0; j < N; j++) {
			insertion(j + 1); //Заполняем кучу N элементами
		}

		cout << N << " " << no_of_nodes << endl;

		//Поиск
		all_time = 0; //Обнуляем время
		for (int j = 0; j < count; j++) {
			begin_max = chrono::high_resolution_clock::now();
			Find(mini); //Поиск максимального элемента
			end_max = chrono::high_resolution_clock::now();
			milli_diff_max = end_max - begin_max; //Время в микросекундах 
			times.push_back(milli_diff_max.count()); //Записываем время
			all_time += milli_diff_max.count(); //Прибавляем в общее время
		}
		out << "Найти " << all_time << " " << all_time / count << " " << *max_element(times.begin(), times.end()) << endl;
		times.clear();

		cout << N << " " << no_of_nodes << endl;

		//Добавление
		all_time = 0; //Обнуляем время
		for (int j = 0; j < count; j++) {
		    begin_max = chrono::high_resolution_clock::now();
			insertion(count + j + 1); // Добавление элемента
		    end_max = chrono::high_resolution_clock::now();
		    milli_diff_max = end_max - begin_max; //Время в микросекундах 
		    times.push_back(milli_diff_max.count()); //Записываем время
			all_time += milli_diff_max.count(); //Прибавляем в общее время
		}
		out << "Добавление " << all_time << " " << all_time / count << " " << *max_element(times.begin(), times.end()) << endl;
		times.clear();

		//Удаление
		all_time = 0; //Обнуляем время
		for (int j = 0; j < count; j++) {
			begin_max = chrono::high_resolution_clock::now();
			Extract_min(); // Удаление элемента
			end_max = chrono::high_resolution_clock::now();
			milli_diff_max = end_max - begin_max; //Время в микросекундах 
			times.push_back(milli_diff_max.count()); //Записываем время
			all_time += milli_diff_max.count(); //Прибавляем в общее время
		}
		end = chrono::high_resolution_clock::now();
		milli_diff = end - begin; //Время в микросекундах 
		out << "Удалить " << all_time << " " << all_time / count << " " << *max_element(times.begin(), times.end()) << endl;
		times.clear();
		
		
		cout << N << " " << no_of_nodes << endl;

	}

	return 0;
}
