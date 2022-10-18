#include <iostream>
#include <stdlib.h>
#include <iomanip>
#include <string>
#include <windows.h>
using namespace std;


struct DataSet {
    /*
    *day - день
    *month - месяц
    *year - год
    */

    int day;
    int month;
    int year;
};

struct Worker {
    /*
    *full_name - полное ФИО
    *sex - пол
    *date - дата рождения
    */
    string full_name;
    string sex;
    DataSet date;
};

int getCount(); //Ввод размера массива
Worker* arrayGenerator(int N, Worker* workers); //Создание массива
string randomFIO(int sexid); //Генерация рандомного ФИО
void searching(int N, Worker* workers); //Поиск по ФИО
void crossSection(int N, Worker* workers); //Срез
void statistics(int N, Worker* workers); //Статистика по полу
void sorting(int N, Worker* workers); //Сортировка по возрастанию
void selector(int N, Worker* workers); //Меню программы

int main()
{
    srand(time(0));
    setlocale(LC_ALL, "Russian"); //Установка поддержки русского языка
    SetConsoleCP(1251); //Установка русского ввода в консоль
    int N = getCount();
    Worker *workers = new Worker[N];
    workers = arrayGenerator(N, workers);
    selector(N, workers);
}

int getCount() {
    /*
    * Функция ввода размерности
    *
    @param N - размерность массива
    */
    int N = 0;
    cout << "Введите размерность массива:" << endl;
    cin >> N;
    return N;
}

Worker* arrayGenerator(int N, Worker* workers) {
    /*
    *@param sex_id - индекс массива
    */
    int sex_id = 0;
    string sex[2] = { "Мужской", "Женский" };
    for (int i = 0; i < N; i++) {

        //рандомная генерация человека по полу
        sex_id = rand() % 2;
        workers[i].sex = sex[sex_id];

        //генерируем ФИО в зависимости от пола
        workers[i].full_name = randomFIO(sex_id);

        //генерация даты рождения
        workers[i].date.day = 1 + rand() % 31;
        workers[i].date.month = 1 + rand() % 12;
        workers[i].date.year = 1955 + rand() % 49;
    }
    cout << "------------------------------------------------------------------------" << endl;
    for (int i = 0; i < N; i++) {
        cout << setiosflags(ios::left) << setw(35) << workers[i].full_name << setw(8) << workers[i].sex << setw(3) << workers[i].date.day << setw(3)
            << workers[i].date.month << setw(4) << workers[i].date.year << endl;
    }
    cout << "------------------------------------------------------------------------" << endl;
    return workers;
}

string randomFIO(int sexid) {
    /*
    *@param name_id - индекс массива с именами
    *@param surname_id - индекс массива с фамилиями
    *@param patronymics_id - индекс массива с отчествами
    *@param names - массив с возможными именами
    *@param surnames - массив с возможными фамилиями
    *@param patronymics - массив с возможными отчествами
    *return - на выходе получаем полное ФИО человека
    */
    int name_id = 0;
    int surname_id = 0;
    int patronymics_id = 0;
    string names[9] = { "Сергей", "Дмитрий", "Андрей", "Олег", "Михаил", "Евгения", "Анна", "Виктория" };
    string surnames[9] = { "Иванов", "Сергеев", "Кузнецов", "Смирнов", "Соколов", "Никитин", "Орлов", "Борисов" };
    string patronymics[9] = { "Александров", "Артемов", "Сергеев", "Константинов", "Дмитриев", "Михайлов", "Максимов", "Алексеев" };

    if (sexid == 1) {

        //Генерация индексов имени, фамилии и отчества
        name_id = 5 + rand() % 3;
        surname_id = rand() % 8;
        patronymics_id = rand() % 8;
        return surnames[surname_id] + "а " + names[name_id] + " " + patronymics[patronymics_id] + "на";
    }
    else {
        //Генерация индексов имени, фамилии и отчества
        name_id = rand() % 5;
        surname_id = rand() % 8;
        patronymics_id = rand() % 8;
        return surnames[surname_id] + " " + names[name_id] + " " + patronymics[patronymics_id] + "ич";
    }
}

void searching(int N, Worker* workers) {
    /*
    *@param name_key - имя человека, которого мы будем искать
    *@param surname_key - фамилия человека, которого мы будем искать
    *@param patronymics_key - отчество человека, которого мы будем искать
    *@param search_key - полное ФИО человека, которого мы будем искать
    */
    string name_key = "";
    string surname_key = "";
    string patronymics_key = "";
    string search_key = "";
    cout << "Введите фамилию, имя и отчество для поиска:" << endl;
    cin >> surname_key >> name_key >> patronymics_key;
    search_key = surname_key + " " + name_key + " " + patronymics_key;
    for (int i = 0; i < N; i++) {
        if (workers[i].full_name == search_key) {
            cout << setiosflags(ios::left) << "\nID: " << i + 1 << " " << setw(35) << workers[i].full_name << setw(8) << workers[i].sex << setw(3) << workers[i].date.day << setw(3)
                << workers[i].date.month << setw(4) << workers[i].date.year << endl;
        }
    }
    selector(N, workers);
}

void crossSection(int N, Worker* workers) {
    /*
    *@param year_key - год, от которого будет срез
    *@param type_key - режим среза (больше/меньше)
    */
    int year_key = 0;
    int type_key = 0;
    cout << "Введите год для среза:" << endl;
    cin >> year_key;
    cout << "[0] Срез больше указанного года\n[1] Срез меньше указанного года" << endl;
    cin >> type_key;
    switch (type_key) {

    //type_key = 0, то выполняем срез больше указанного года
    case 0:
        for (int i = 0; i < N; i++) {
            if (workers[i].date.year > year_key) {
                cout << setiosflags(ios::left) << "\nID: " << i + 1 << " " << setw(35) << workers[i].full_name << setw(8) << workers[i].sex << setw(3) << workers[i].date.day << setw(3)
                    << workers[i].date.month << setw(4) << workers[i].date.year << endl;
            }
        }
        break;

    //type_key = 1, то выполняем срез меньше указанного года
    case 1:
        for (int i = 0; i < N; i++) {
            if (workers[i].date.year < year_key) {
                cout << setiosflags(ios::left) << "\nID: " << i + 1 << " " << setw(35) << workers[i].full_name << setw(8) << workers[i].sex << setw(3) << workers[i].date.day << setw(3)
                    << workers[i].date.month << setw(4) << workers[i].date.year << endl;
            }
        }
        break;
    default:
        cout << "Введен неверный ключ команды";
        break;
    }
    selector(N, workers);
}

void statistics(int N, Worker* workers) {
    /*
    *@param male_count - количество человек мужского пола
    */
    int male_count = 0;
    for (int i = 0; i < N; i++) {
        if (workers[i].sex == "Мужской") male_count++;
    }
    cout << "Количество работаников мужского пола: " << male_count << endl;
    cout << "Количество работаников женского пола: " << N - male_count << endl;
    selector(N, workers);
}

void sorting(int N, Worker* workers) {
    /*
    *@param temp - буфферный экзмепляр структуры для сортировки
    *Здесь выполнена сортировка пузырьком. Сравниваются соседние ячейки, и при необходимом условии выполняется сдвиг.
    */
    Worker temp = {};
    for (int i = 0; i < N - 1; i++) {
        for (int j = 0; j < N - i - 1; j++) {
            if (workers[j].date.year > workers[j + 1].date.year) {
                temp = workers[j];
                workers[j] = workers[j + 1];
                workers[j + 1] = temp;
            }
        }
    }
    for (int i = 0; i < N - 1; i++) {
        for (int j = 0; j < N - i - 1; j++) {
            if ((workers[j].date.month > workers[j + 1].date.month) && (workers[j].date.year == workers[j + 1].date.year)) {
                temp = workers[j];
                workers[j] = workers[j + 1];
                workers[j + 1] = temp;
            }
        }
    }
    for (int i = 0; i < N - 1; i++) {
        for (int j = 0; j < N - i - 1; j++) {
            if ((workers[j].date.day > workers[j + 1].date.day) && (workers[j].date.year == workers[j + 1].date.year)
                && ((workers[j].date.month == workers[j + 1].date.month))) {
                temp = workers[j];
                workers[j] = workers[j + 1];
                workers[j + 1] = temp;
            }
        }
    }
    for (int i = 0; i < N; i++) {
        cout << setiosflags(ios::left) << setw(35) << workers[i].full_name << setw(8) << workers[i].sex << setw(3) << workers[i].date.day << setw(3)
            << workers[i].date.month << setw(4) << workers[i].date.year << endl;
    }
    selector(N, workers);
}

void selector(int N, Worker* workers) {
    /*
    *@param key - ключ работы меню
    */
    int key = 0;
    cout << "\n[0] Поиск по ФИО (Вводить полностью)\n"
        "[1] Срез работников по году рождения\n"
        "[2] Статистика по полу\n"
        "[3] Сортировка массива по возрастанию\n"
        "[4] Выход из программы\n"<< endl;
    cin >> key;
    switch (key) {
    case 0:
        searching(N, workers);
        break;
    case 1:
        crossSection(N, workers);
        break;
    case 2:
        statistics(N, workers);
        break;
    case 3:
        sorting(N, workers);
        break;
    case 4:
        delete[] workers;
        exit(0);
        break;
    default:
        cout << "Неверный запрос\n";
        break;
    }
}