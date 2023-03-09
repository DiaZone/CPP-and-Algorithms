#include <iostream>
#include <random>
#include <chrono>
#include <fstream>
#include <vector>

using namespace std;

const int M = 20; //Количество тестов
unsigned int count_of_recursion = 0; //Счетчик рекурсий
int height = 0; //Высота стека

void fileCreator(); //Cоздание и очистка файла
void fileWriter(int i, int N, double time); //Запись результата в файл
void quickSort(vector <double>& myVector, int low, int high, int depth);
int partition(vector <double>& myVector, int low, int high);
void millisecundomer(vector <double>& myVector, int N, int i);
void sortingSort(vector <double>& myVector, int N, int i);
void nullSort(vector <double>& myVector, int N);
void antiQSort(vector <double>& myVector, int N, int i);


//Создание и очистка файла
void fileCreator() {
    ofstream outdatafile("D:\\Games\\data.txt");
}

//Запись результата в файл
void fileWriter(int i, int N, double time) {
    ofstream outdatafile;
    outdatafile.open("D:\\Games\\data.txt", ios::app);
    if (outdatafile.is_open())
    {
        outdatafile << i << " " << N << " " << time << " " << count_of_recursion - 1 << " " << height << endl;
    }
}


//Поиск опорного элемента
/* 
* @param piv - опорный элемент
* @param i - новое начало
* return i + 1 - возврат индекса опорного элемента
*/
int partition(vector <double> &myVector, int low, int high) {
    double piv = myVector[(high + low)/2]; //Выбираем опорный элемент
    int i = low - 1; //Свободный элемент

    //Пока не дойдем до последнего элемента
    for (int j = low; j < high; j++) {

        //Если элемент меньше опорного
        if (myVector[j] <= piv) {

            //Увеличиваем индекс для элемента
            i++;
            //Ставим элемент на нужное место
            swap(myVector[i], myVector[j]);
        }
    }
    swap(myVector[i + 1], myVector[high]);
    return i + 1;
}

//Быстрая сортировка
/*
* @param piv - опорный элемент
* @param depth - глубина ветки
* @param count_of_recursion количество рекурсий
* @param height - максимальная высота стека
* @param v массив
*/
void quickSort(vector <double> &myVector, int low, int high, int depth) {
    double piv;
    count_of_recursion++;

    //Если глубина ветки меньше текущей высоты стека, то высота стека становится равной глубине
    if (depth > height) height = depth;

    //Если индекс первого элемента меньше последнего, то находим индекс опорного элемента и вызываем сортировку для двух частей вектора
    if (low < high) {
        piv = partition(myVector, low, high);
        quickSort(myVector, low, piv - 1, depth + 1);
        quickSort(myVector, piv + 1, high, depth + 1);
    }
}



//Подсчет времени на сортировку
/* 
* @param i - Номер теста
* @param myVector - Вектор с элементами
* @param N - Кол-во элементов в массиве
* @param low - Начало сортировки
* @param high - Конец сортировки
*/
void millisecundomer(vector <double> &myVector, int N, int i) {
    int low = 0;
    int high = myVector.size() - 1;
    int depth = 0;
    height = 0; //Обнуление счетчика высоты
    count_of_recursion = 0; //Обнуление счетчика рекурсий
    chrono::high_resolution_clock::time_point start = chrono::high_resolution_clock::now(); //Время начала сортировки
    quickSort(myVector, low, high, depth); //Сортировка
    chrono::high_resolution_clock::time_point end = chrono::high_resolution_clock::now(); //Время на конец сортировки
    chrono::duration<double, milli> milli_diff = end - start; //Время в миллисекундах
    fileWriter((i + 1), myVector.size(), milli_diff.count()); //Запись в файл
}

//Сортировка отсортированного массива
/*
* @param i - Номер теста
* @param myVector - Вектор с элементами
* @param N - Кол-во элементов в массиве
*/
void sortingSort(vector <double> &myVector, int N, int i) {
    millisecundomer(myVector, N, i); //Отправлявяем в таймер сортировать отсортированный массив
}

//Сортировка массива одинаковых элементов
/*
* @param i - Номер теста
* @param myVector - Вектор с элементами
* @param N - Кол-во элементов в массиве
*/
void nullSort(vector <double> &myVector, int N) {
    millisecundomer(myVector, N, 0); //Отправлявяем в таймер сортировать массив одинаковых элементов
}

//Перестановка отсортированного массива
/*
* @param i - Номер теста
* @param myVector - Вектор с элементами
* @param N - Кол-во элементов в массиве
*/
void antiQSort(vector <double> &myVector, int N, int i) {
    for (int j = 2; j <= myVector.size(); j++) {
        int added_index = N - j;
        int middle = (N - j + (N - 1)) / 2; //Корневой индекс для быстрой сортировки
        swap(myVector[N - j], myVector[(N - j + (N - 1)) / 2]);
    }
    millisecundomer(myVector, myVector.size(), i);
}

//Перестановка отсортированного массива для детермированного метода
/*
* @param i - Номер теста
* @param myVector - Вектор с элементами
* @param N - Кол-во элементов в массиве
*/
void determQSort(vector <double> &myVector, int N, int i) {
    for (int j = 0; j < N; j++) {
        swap(myVector[N - 1 - j], myVector[(N - 1 - j) / 2]);
    }
    millisecundomer(myVector, N, i);
}

int main()
{
    setlocale(LC_ALL, "Russian");
    mt19937 engine(time(0));
    int N = 100;
    vector <double> myVector; //Вектор
    fileCreator(); //Очистка файла

    for (int j = 0; j < 10; j++) {
        vector <double> nullVector(N); //Вектор из одинаковых элементов
        nullSort(nullVector, myVector.size()); //Сортировка нулевого массива

        for (int i = 0; i < M; i++) { //Выполняем все тесты       

            //Генерация случайных чисел в [-1; 1]
            uniform_real_distribution<double> gen(-1.0, 1.0);
            for (int el = 0; el < N; el++) {
                myVector.push_back(gen(engine));
            }

            millisecundomer(myVector, myVector.size(), i); //Замер времени и сортировка нашего вектора
            sortingSort(myVector, myVector.size(), i); //Сортировка отсортированного вектора
            antiQSort(myVector, myVector.size(), i); //Сортировка вектора с макс колвом сравнений
            determQSort(myVector, myVector.size(), i); //Сортировка вектора с макс колвом сравнений при детерминированном выборе опорного элемента
            myVector.clear(); //Очистка массива
        }
        nullVector.clear();//Очистка нулевого массива
        N *= 2; //Увеличиваем количество элементов
    }

    cout << "Успешно";
}
