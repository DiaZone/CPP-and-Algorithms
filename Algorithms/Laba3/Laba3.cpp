#include <iostream>
#include <algorithm>
#include <stdlib.h>
#include <time.h>
#include <string>
#include <fstream>
#include <stack>
#include <ctime>

using namespace std;

//Структура данных о человеке
struct Person {
    string surname; //Фамилия
    string name; //Имя
    string patronymic; //Отчество
    int day; //День рождения
    int month; //Месяц рождения
    int year; //Год рождения
};

//Шаблонный класс
template <typename T>
class Stack {
public:
    T* arr; //Контейнер для хранения элементов
    int count; //Количество переменных

    class Iterator {
    private:
        Stack<T>* mystack; //Объект стэка
        int index; //Индекс
    public:
        Iterator(int index, Stack<T>* mystack) :
            //Задаем стэк и начальный индекс
            mystack(mystack),
            index(index > mystack->size() ? -1 : index) {
        }

        //Оператор перемещения
        Iterator& operator++() {
            //Если есть следующий элемент, то идем к нему
            if (index != mystack->size() - 1) {
                index++;
            }
            //Иначе элементов нет
            else {
                index = -1;
            }
            //Разыменование указателя
            return *this;
        }

        //Оператор возврата значения
        T operator*() {
            return mystack->arr[index];
        }

        //Оператор сравнения
        bool operator !=(Iterator& it) {
            return it.index != index;
        }
    };

    //Создание пустого стека
    Stack() {
        arr = nullptr;
        count = 0;
    }

    //Проверка, пустой ли стек
    bool empty() {
        //Если стек пуст, то вернется true
        return count == 0;
    }

    //Добавить элемент наверх
    void push(T x) {
        /*
        *@param x - элемент, который помещаем в стек
        *@param temp - временный контейнер
        */
        T* temp;
        temp = arr;

        //Выделяем новую память на 1 элемент больше
        arr = new T[count + 1];

        //Увеличиваем количество элементов
        count++;

        //Перезаписываем основной контейнер с помощью временного
        for (int i = 0; i < count - 1; i++) {
            arr[i] = temp[i];
        }
        //Добавляем новый элемент
        arr[count - 1] = x;

        //Очищаем временный контейнер
        delete[] temp;

    }

    //Получить элемент сверху
    T top() {

        //Если стек не пуст
        if (!empty()) {
            //Возвращаем верхний элемент
            return arr[count - 1];
        }

        //Иначе он пуст
        else {
            cout << "Стэк пуст" << endl;
            return 0;
        }
    }

    //Удалить элемент сверху
    void pop() {
        /*
        *@param temp - временный контейнер
        */
        T* temp;
        temp = arr;
        //Выделяем новую память на 1 элемент меньше
        arr = new T[count - 1];

        //Уменьшаем размер стека
        count--;

        //Перезаписываем основной контейнер с помощью временного
        for (int i = 0; i < count; i++) {
            arr[i] = temp[i];
        }

        //Удаляем временный контейнер
        delete[] temp;
    }

    //Получить размер стэка
    int size() {
        return count;
    }

    //Сортировка
    void sorting() {
        sort(arr, arr + size());
    }

    //Функция вывода стека
    void print() {
        /*
        *@param p - хранилище элементов
        */
        T* p = arr;

        //Если не пуст
        if (!empty()) {
            //Проходимся по количеству элементов
            for (int i = 0; i < count; i++) {

                if ((i < 10) || (i >= (count - 10))) {
                    cout << *p << endl;
                }
                //Идем к следующему
                p++;
            }
        }
        //Инач стек пуст
        else {
            cout << "Стек пуст" << endl;
        }
    }

    //Указатель на начало
    Iterator& begin() {
        return *(new Iterator(0, this));
    }

    //Указатель на конец
    Iterator& end() {
        return *(new Iterator(-1, this));
    }

};

void oneKFillAndSorting(); //Заполнение стека числами
void minMaxSum(Stack<int> my_stack, int count); //Нахождение суммы, Максимального, Минимального и среднего элементов
void pasteAndDel(); //Проверка вставки и изъятия
Stack<int> sortTest(Stack<int> myStack, stack<int> standart_stack); //Проверка сортировки
Stack<int> reverseStack(Stack<int> myStack); //Инверсия стека
void generationPersons(); //Генерация данных о людях
string generationSurname(); //Генератор фамилии
string generationName(); //Генератор имени
string generationPatronymic(); //Генератор отчества
int generationYear(); //Генератор года рождения
int generationMonth(); //Генератор месяца рождения
int generationDay(int month, int year); //Генератор дня рождения
void filter(Stack<Person> persons, int count); //Фильтрация

int main()
{
    setlocale(LC_ALL, "Russian"); //Установка русской локали
    srand(time(0));
    oneKFillAndSorting(); //Заполнение стека
    pasteAndDel(); //Вcтавка и изъятие
    generationPersons(); //Данные о людях и фильтрация
}


//Заполнение стека
/*
* @param - my_stack созданный стек
* @param - default_stack стек из стандартной библиотеки
* @param count количество элементов в стеке
*/
void oneKFillAndSorting() {
    Stack<int> my_stack; //Мой стек
    stack<int> default_stack; //Стандартный стек
    int count = 1000;
    int temp;

    //Заполняем два стека 1000 рандомными элементами
    for (int i = 0; i < count; i++) {
        temp = -1000 + rand() % 2001;
        default_stack.push(temp); // Добавление элемента в стандартный стек
        my_stack.push(temp); // Добавление элемента в созданный стек
    }

    cout << "Отсортированный стек\n" << endl;

    //Проверка сортировки стека на правильность
    my_stack = sortTest(my_stack, default_stack); 

    //Вывод стека
    my_stack.print(); 

    cout << "\nИнверсия стека" << endl;

    //Инверсия стека
    my_stack = reverseStack(my_stack); 

    //Вывод стека
    my_stack.print();

    cout << "\nСумма, максимум, мининимум, среднее\n";
    cout << "------------------------";
    minMaxSum(my_stack, count);

    //Очистка системного стека
    while (!default_stack.empty()) {
        default_stack.pop();
    }
}

//Нахождение суммы, максимального, минимального, среднего элементов
/*
* @param my_stack - созданный стек
* @param count - длина стека
* @param s - сумма элементов
* @param min - минимальный элемент
* @param max - максимальный элемент
*/
void minMaxSum(Stack<int> my_stack, int count) {
    int s = 0;
    int el, min = my_stack.top(), max = my_stack.top();

    //Пока стек не пуст
    while (!my_stack.empty()) {

        //Элемент
        el = my_stack.top();
        if (min > el) min = el;
        if (max < el) max = el;
        s += el;

        //Удаление элемента
        my_stack.pop(); 
    }
    cout << "\nМинимум: " << min << "\nMaксимум: " << max << "\nСреднее: " << s / count << "\nСумма: " << s << endl;
}

//Проверка вставки и изъятия
/*
* @param my_stack - созданный стек
* @param count - длина стека
*/
void pasteAndDel() {
    Stack<string> my_stack;
    int count = 10;
    cout << "\nВставка и изъятие элементов\n";
    cout << "------------------------\n";

    //Вставка элемента
    for (int i = 0; i < count; i++) {
        my_stack.push(to_string(i + 1) + " строка");
        cout << "Добавление: " << my_stack.top() << endl;
    }

    //Изъятие элемента
    while (!my_stack.empty()) {
        cout << "Изъятие: " << my_stack.top() << endl;
        my_stack.pop(); // Удаление элемента из стека
    }

}

//Проверка сортировки
/*
* @param my_stack - созданный стек
* @param test_stack - cтек для инверсии
* @param default_stack - стек из стандартной библиотеки
* @param count длина - стека
* @return my_stack - возвращает отсортированный стек
*/
Stack<int> sortTest(Stack<int> my_stack, stack<int> default_stack) {
    Stack<int> test_stack;

    //Сортировка стека
    my_stack.sorting();

    using cnt = decltype(default_stack)::container_type;

    //Сортировка стека из стандартной библиотеки
    sort(reinterpret_cast<cnt*>(&default_stack)->begin(), reinterpret_cast<cnt*>(&default_stack)->end());

    //Длина стека
    int count = default_stack.size();

    //Проверка совпадения длин стеков
    if (default_stack.size() == my_stack.size()) {
        for (int i = 0; i < count; i++) {

            //Запись в тестовый стек
            test_stack.push(my_stack.top()); 

            //Проверка совпадения элементов
            if (my_stack.top() != default_stack.top()) { 
                break; 
            }
            my_stack.pop();
            default_stack.pop();
        }
    }

    //Проверка на совпадение стеков
    if (default_stack.empty() && my_stack.empty()) { 
        cout << "Стеки равны\n"; 
    }
    else { 
        cout << "Стеки неравны\n"; 
    }

    return reverseStack(test_stack);
}

//Инверсия стека
/*
* @param test_stack - созданный стек
* @param reverse_test_stack - инвертированный стек
* @return reverse_test_stack - возвращает инвертированный стек
*/
Stack<int> reverseStack(Stack<int> test_stack) {
    Stack<int> reverse_test_stack;
    while (!test_stack.empty()) {
        reverse_test_stack.push(test_stack.top());
        test_stack.pop();
    }
    return reverse_test_stack;
}

//Генерация данных о человеке
/*
* @param persons стек людей
* @param new_person новый человек
* @param count количество людей
*/
void generationPersons() {
    int count = 100;
    Stack<Person> persons;
    Person new_person;

    for (int i = 0; i < count; i++) {

        new_person.name = generationName(); //Генерация имени
        new_person.surname = generationSurname(); //Генерация фамилии
        new_person.patronymic = generationPatronymic();  //Генерация отчества
        new_person.year = generationYear();  //Генерация года рождения
        new_person.month = generationMonth(); //Генерация месяца рождения
        new_person.day = generationDay(new_person.month, new_person.year); //Генерация дня рождения
        persons.push(new_person); //Добавление нового человека в стек

    }
    filter(persons, count); //Фильтрация по возрасту
}

//Генерация фамилии
/*
* @param surnames - возможные фамилии
*/
string generationSurname() {
    string surnames[] = { "Кузнецов", "Попов", "Соколов", "Егоров", "Петров" };
    return surnames[rand() % 5];
}

//Генерация имен
/*
* @param names - возможные имя
*/
string generationName() {
    string names[] = { "Дмитрий", "Сергей", "Алексей", "Иван", "Артем" };
    return names[rand() % 5];
}

//Генерация отчества
/*
* @param patronymic - возможные отчества
*/
string generationPatronymic() {
    string patronymic[] = { "Сергеевич", "Иванович", "Михайлович", "Андреевич", "Артемович" };
    return patronymic[rand() % 5];
}

//Генерация года рождения
int generationYear() {
    return 1980 + rand() % 41;
}

//Генерация месяца рождения
int generationMonth() {
    return 1 + rand() % 13;
}

//Генерация дня рождения
/*
* @param month месяц
* @param year год
*/
int generationDay(int month, int year) {
    if (month == 1 || month == 3 || month == 5 || month == 7 || month == 8 || month == 10 || month == 12) {
        return 1 + rand() % 32; //Январь, Март, Май, Июнь, Август, Октябрь, Декабрь
    }
    else if (month == 2) { //Февраль
        if (year % 4 != 0 || (year % 100 == 0 && year % 400 != 0)){
            return 1 + rand() % 29; //Невисокосный год
        }
        else{
            return 1 + rand() % 30; //Високосный год
        }
    }
    else {
        return 1 + rand() % 31; //Апрель, Июнь, Сентябрь, Ноябрь
    }
}

//Фильтрация
/*
* @param persons стек людей
* @param count количество элементов в стеке
* @param max_age максимальный возраст
* @param min_age минимальный возраст
* @param age возраст
* @param count_filter количество подходящих людей
* @param count_not_filter количество не подходящих людей
* @param t текущее время
*/
void filter(Stack<Person> persons, int count) {

    cout << "\nФильтр на возраст" << endl;

    Stack<Person> filter_persons;
    cout << "------------------------\n";
    int max_age = 30, min_age = 20;
    int age, count_filter = 0, count_not_filter = 0;
    time_t t1 = time(NULL);
    struct tm timet;
    localtime_s(&timet, &t1);

    //Текущая дата
    printf("Текущая дата: %.2d.%.2d.%.2d\n", timet.tm_mday, timet.tm_mon + 1, timet.tm_year + 1900);

    for (int i = 0; i < count; i++) {

        //Подсчет возраста
        age = timet.tm_year + 1900 - persons.arr[i].year;

        //Не было дня рождения в этом году
        if (timet.tm_mon + 1 < persons.arr[i].month) {
            age--;
        }
        else if (timet.tm_mon + 1 == persons.arr[i].month) {

            //Не было дня рождения в этом году
            if (timet.tm_mday < persons.arr[i].day) {
                age--; 
            }
        }

        //Проверка условий фильтра
        if (age <= min_age || age >= max_age) {

            //Добавление в стек
            filter_persons.push(persons.arr[i]);

            cout << filter_persons.arr[count_filter].surname << " " << filter_persons.arr[count_filter].name << " " << filter_persons.arr[count_filter].patronymic << " " << 
                filter_persons.arr[count_filter].day << "." << filter_persons.arr[count_filter].month << "." << filter_persons.arr[count_filter].year << endl;

            //Прибавление длины стека
            count_filter++; 
        }
        else count_not_filter++;
    }

    //Проверка количества отфильтрованных людей
    if (count == count_filter + count_not_filter) { 
        cout << "Фильтрация верна\n";
    }
    else { 
        cout << "Фильтрация неверна\n"; 
    }

}
