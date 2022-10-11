#include <iostream>
using namespace std;

int getArrayBit(); ///Ввод размерности массива
int** getArray(int N); ///Создание пустого массива
void clearArray(int** arr, int N); ///Очистка массива
void printArray(int** arr, int N); ///Вывод массива в консоль
void standardArrayFilling(int** arr, int N); ///Заполнение массива от 1 до N*N
void mainSymmetry(int** arr, int N); ///Заполнение свойством симметричности относительно главной диагонали
void sideSymmetry(int** arr, int N); ///Заполнение свойством симметричности относительно побочной диагонали
void pascal(int** arr, int N); ///Треугольник Паскаля
void selector(int** arr, int N); ///Меню программы
void minesweeper(int** arr, int N); ///Заполнение массива как в сапёре
bool bomb(int** arr, int i, int j, int N); ///Проверка наличия бомбы
void deleteArray(int** arr, int N); ///Удаление массива и выход из программы

/*
 *@param N - размерность массива N*N
 *@param arr - массив, над которым происходят действия
*/

int main()
{
    /*
    @param N - размерность массива N*N
    */
    int N = 0;
    setlocale(LC_ALL, "Russian"); //Установка поддержки русского языка
    N = getArrayBit();
    int** myArray = getArray(N);
    selector(myArray, N);
}

int getArrayBit() {
    /*
    * Функция ввода размерности
    *
    @param N - размерность массива N*N
    */
    int N = 0;
    cout << "Введите размерность массива N:" << endl;
    cin >> N;
    return N;
}

int** getArray(int N) {

    ///Выделение памяти на массив

    int** arr = new int* [N]; 
    for (int i = 0; i < N; i++) {
        arr[i] = new int[N];
    }
    return arr; //Возврат пустого массива
}

void clearArray(int** arr, int N) {

    ///Заполнение массива нулями

    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            arr[i][j] = 0;
        }
    }
}

void printArray(int** arr, int N) {

    ///Вывод массива в консоль

    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            cout.width(4);
            cout << arr[i][j];
        }
        cout << endl;
    }
}

void standardArrayFilling(int** arr, int N) {

    ///Заполнение массива от числами по возрастанию от 0 до N*N-1 (Данная функция используется для симметрии у главной и побочной диагонали)

    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            arr[i][j] = i * N + j;
        }
    }
}

void mainSymmetry(int** arr, int N) {

    ///Функция построения симметрии относительно главной диагонали

    standardArrayFilling(arr, N);
    for (int i = 0; i < N - 1; i++) {
        for (int j = i + 1; j < N; j++) {
            arr[i][j] = arr[j][i]; //Симметричная расстановка чисел от главной диагонали
        }
    }
    printArray(arr, N);
}

void sideSymmetry(int** arr, int N) {

    ///Функция построения симметрии относительно побочной диагонали

    standardArrayFilling(arr, N);
    for (int i = 0; i < N - 1; i++) {
        for (int j = N - 2 - i; j >= 0; j--) {
            arr[i][j] = arr[N - j - 1][N - i - 1]; //Симметричная расстановка чисел от побочной диагонали
        }
    }
    printArray(arr, N);
}

void pascal(int** arr, int N) {

    ///Построение треуголника Паскаля

    clearArray(arr, N); //Предварительная очистка массива
    arr[0][0] = 1; //Устанавливаем первоначальную единицу для построения треугольника
    for (int i = 1; i < N; i++) {
        arr[i][0] = 1;
        for (int j = 1; j <= i; j++) {
            arr[i][j] = arr[i - 1][j - 1] + arr[i - 1][j]; //Суммирование двух предыдущих соседних ячеек
        }
    }
    printArray(arr, N);
}

void selector(int** arr, int N) {
    /*
    @param key - ключ управления меню
    */
    int key = 0;
    cout << "[0] Очистка массива \n" <<
        "[1] Симметрия относительно главной диагонали\n" <<
        "[2] Симметрия относительно побочной диагонали\n" <<
        "[3] Треугольник Паскаля\n" <<
        "[4] Построение сапёра\n" <<
        "[5] Выход из программы\n";
    cin >> key;
    switch (key) {
    case 0:
        clearArray(arr, N);
        printArray(arr, N);
        selector(arr, N);
        break;
    case 1:
        mainSymmetry(arr, N);
        selector(arr, N);
        break;
    case 2:
        sideSymmetry(arr, N);
        selector(arr, N);
        break;
    case 3:
        pascal(arr, N);
        selector(arr, N);
        break;
    case 4:
        minesweeper(arr, N);
        selector(arr, N);
        break;
    case 5:
        deleteArray(arr, N);
        break;
    default:
        cout << "Введён неправильный ключ\n";
        selector(arr, N);
    }
}

void minesweeper(int** arr, int N) {

    /*
    @param count_of_bombs - количество бомб на массив
    @param place - место, в которое будет установлена бомба
    @param i - координата бомбы в строке
    @param j - координата бомбы в столбце
    */

    clearArray(arr, N);
    int count_of_bombs = 0;
    int place = 0;
    int i = 0;
    int j = 0;
    srand(time(0));
    cout << "Введите количество бомб: " << endl;
    cin >> count_of_bombs;

    ///Генерация бомб

    while (count_of_bombs > 0) {

        ///Генерируем случайную координату от 0 до N*N-1 для бомбы и переводим эту координату в двумерный вид

        place = rand() % (N * N - 1);
        i = place / N;
        j = place % N;

        ///Проверяем, есть ли уже в данной ячейке бомба, и если её нет, то ставим её

        if (arr[i][j] != -1) {
            arr[i][j] = -1;
            count_of_bombs--;
        }
    }
    ///Заполняем массив поля цифрами
    for (i = 0; i < N; i++)
    {
        for (j = 0; j < N; j++)
        {

            ///Если в ячейке нет бомбы, то смотрим на соседние ячейки и считаем сколько бомб рядом

            if (arr[i][j] != -1)
            {
                if (bomb(arr, i - 1, j - 1, N)) arr[i][j]++;
                if (bomb(arr, i - 1, j, N)) arr[i][j]++;
                if (bomb(arr, i - 1, j + 1, N)) arr[i][j]++;
                if (bomb(arr, i, j - 1, N)) arr[i][j]++;
                if (bomb(arr, i, j + 1, N)) arr[i][j]++;
                if (bomb(arr, i + 1, j - 1, N)) arr[i][j]++;
                if (bomb(arr, i + 1, j, N)) arr[i][j]++;
                if (bomb(arr, i + 1, j + 1, N)) arr[i][j]++;
            }
        }
    }
    printArray(arr, N);
}

bool bomb(int** arr, int i, int j, int N)
{
    ///Проверка, что мы не выходим за размер массива
    if ((i >= 0) && (i < N))
    {
        if ((j >= 0) && (j < N))
        {
            ///Если в данной ячейке есть бомба, то возвращаем true
            if (arr[i][j] == -1)
                return true;
        }
    }
    return false;
}

void deleteArray(int** arr, int N) {

    ///Очистка памяти и завершение программы
    for (int i = 0; i < N; i++) {
        delete[]arr[i];
    }
    delete[]arr;
    exit(0);
}