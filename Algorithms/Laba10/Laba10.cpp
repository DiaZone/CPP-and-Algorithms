//Метод отжига
//Реализовать алгоритм отжига для поиска глобального оптимума(минимума) произвольной функции.В качестве примера взять функцию F(x) = x ^ 2 + 10 - 10 * cos(2 * pi * x) 
//Сам алгоритм выглядит следующим образом:
//1)Задать начальное значение(можно выбирать случайно)+
//2)Изменить значение температуры при помощи заданной функции T(k), где k это номер итерации, получив температуру T[k]
//3)Сгенерировать новую точку x[k + 1], с которой будет сравниваться текущий вариант(возможна случайная генерация, или использование какой либо функции от температуры)
//4)Вычислить значение искомой функции F(x) в точке x[k + 1] и вычислить разницу между F(x[k + 1]) - F(x[k]) = dF
//5)Проверка решения на вероятность принятий : { 1 при dF < 0
//P(x[k], x[k + 1]) = { { exp(-dF / T[k])
//6)Проверяем критерий завершения, критерием является некоторая температура окончания.

#include <iostream>
#include <math.h>
#include <random>
#include <chrono>

using namespace std;
const double PI = 3.141592653589793;


//Функция вычисления значения функции
double myFunc(double x) {
    return pow(x, 2) + 10 - 10 * cos(2 * PI * x);
}

//Функция получения случайного вещественного числа от -10 до 10
double randomizer() {
    return (double)(-100 + rand() % 201) / 10;
}

//Функция получения нового значения температуры(быстрый отжиг)
double newTemperature(double old_T, double x) {
    std::mt19937 gen(1701);
    cauchy_distribution<double> C(0, 1);
    return x + old_T * C(gen);
}


int main()
{
    setlocale(LC_ALL, "Russian");
    srand(time(0));
    chrono::high_resolution_clock::time_point begin; 
    chrono::high_resolution_clock::time_point end;
    chrono::duration<double, milli> milli_diff;

    double x_start = randomizer(); //Генерация начального значения
    double T_min = 0; //Минимальное значение температуры
    double y, del_y, x_prob, y_prob;

    for (int i = 0; i < 5; i++) {
        double x = x_start; //Начальное 
        int k = 0; //Счетчик итераций
        double T = 100; //Стартовая температура
        begin = chrono::high_resolution_clock::now(); //Стартовое время
        cout << T_min << endl;

        while (T > T_min) {
            x_prob = randomizer(); //Получаем новое случайное значение возможного х
            y_prob = myFunc(x_prob); //Вычисляем для него значение функции
            y = myFunc(x); //Вычисляем значение функции для текущего х

            del_y = y_prob - y; //Вычисление разницы между значениями функций

            //Если меньше нуля, то мы принимаем решение о том, что наш новый х - это возможный
            if (del_y < 0) {
                x = x_prob;
            }

            //Иначе рассчитываем новое значение х через температуру
            else {
                
                x = exp(-(del_y / T));
            }

            T = newTemperature(T, abs(x)); //Задаем новое значение температуры
            k++;

        }

        end = chrono::high_resolution_clock::now(); //Конечное время
        milli_diff = end - begin; //Время в миллисекундах
        T_min = pow(0.1, i + 1); //Новое значение минимальной температуры

        cout << "Начальная точка: " << x_start << endl;
        cout << "Конечная точка: " << x << " " << myFunc(x) << endl;
        cout << "Количество итераций: " << k << endl;
        cout << "Затраченное время: " << milli_diff.count() << endl;
    }
}

