#include <iostream>
#include <stdlib.h>
#include <chrono>
#include <random>
#include <fstream>

const int TESTS = 20; //Количество тестов

using namespace std;

void fileCreator(); //Создание и очистка файла
void fileWriter(int i, int N, double time); //Запись результата в файл
void bubbleSort(double* arr, int N); //Сортировка пузырьком
void millisecundomer(double* arr, int N, int i); //Подсчет времени на сортировку

int main()
{
    int N[] = { 1000, 2000, 4000, 8000, 16000, 32000, 64000, 128000 }; //Массив длин массива для сортировки

    fileCreator(); //Создание файла

    for (int j = 0; j < size(N); j++) {
        double* arr = new double[N[j]]; //Выделение памяти под массив элементов
        for (int i = 0; i < TESTS; i++) {

            //Генерация случайных чисел в [-1; 1]
            mt19937 engine(time(0));
            uniform_real_distribution<double> gen(-1.0, 1.0);
            for (int el = 0; el < N[j]; el++) {
                arr[el] = gen(engine);
            }

            millisecundomer(arr, N[j], i); //Замер времени и сортировка

        }
        delete[] arr; //Удаление массива
    }
}

//Сортировка пузырьком
/*
* @param arr - Массив
* @param flag - Флаг требования сортировки
* @param N - Кол-во элементов
*/
void bubbleSort(double* arr, int N) { 
    bool flag = true;
    while (flag) {
        for (int i = 0; i < N; i++) {
            flag = false;
            for (int j = 0; j < N - i - 1; j++) {
                if (arr[j] > arr[j + 1]) {
                    swap(arr[j], arr[j + 1]);
                    flag = true;
                }
            }
        }
    }
}

//Создание и очистка файла
void fileCreator() {
    ofstream outdatafile("D:\\Users\\DiaZo\\Desktop\\bubblesort.txt");
}

//Запись результата в файл
void fileWriter(int i, int N, double time) {
    ofstream outdatafile("D:\\Users\\DiaZo\\Desktop\\bubblesort.txt", ios::app); //Открываем файл для записи
    if (outdatafile.is_open())
    {
        outdatafile << "Тест " << i << " | Число элементов: " << N << " | Время: " << time << " миллисекунд" << endl;
    }
}

//Подсчет времени на сортировку
/*
* @param arr - Массив
* @param i - Номер теста
* @param N - Кол-во элементов
*/
void millisecundomer(double* arr, int N, int i) {
    chrono::high_resolution_clock::time_point start = chrono::high_resolution_clock::now(); //Время начала сортировки
    bubbleSort(arr, N); //Сама сортировка
    chrono::high_resolution_clock::time_point end = chrono::high_resolution_clock::now(); //Время на конец сортировки
    chrono::duration<double, milli> milli_diff = end - start; //Итоговое время сортировки в миллисекундах
    fileWriter((i + 1), N, milli_diff.count()); //Запись в файл
}

