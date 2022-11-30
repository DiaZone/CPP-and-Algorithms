#include <iostream>
#include <Windows.h>
#include <malloc.h>

using namespace std;

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
        arr = (T*)malloc((count + 1) * sizeof(T));

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
    T pop() {

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
    void del() {
        /*
        *@param temp - временный контейнер
        */
        T* temp;

        temp = arr;

        //Выделяем новую память на 1 элемент меньше
        arr = (T*)malloc((count - 1) * sizeof(T));

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

                //Выводим текущий элемент
                cout << *p << endl;

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

int main()
{
    SetConsoleCP(1251); //Поддержка ввода на русском языке
    setlocale(LC_ALL, "Russian"); //Установка русской локали

    cout << "Создание стека\n" << endl;

    Stack<int> myStack;
    myStack.print();

    cout << "\nПроверка на пустоту\n" << endl;
    cout << myStack.empty();

    cout << "\n\nДобавление элементов в стек\n" << endl;
    myStack.push(1);
    myStack.push(2);
    myStack.push(3);
    myStack.push(4);
    myStack.print();

    cout << "\nПроверка получения элемента сверху\n" << endl;
    int x = myStack.pop();
    cout << x << "\n" << endl;
    myStack.print();

    cout << "\nПроверка удаления\n" << endl;
    myStack.del();
    myStack.del();
    myStack.print();

    cout << "\nПроверка получения элемента сверху после удаления\n" << endl;
    x = myStack.pop();
    cout << x << "\n" << endl;
    myStack.print();


    cout << "\nПроверка на пустоту\n" << endl;
    cout << myStack.empty();

    cout << "\n\nПолучение размера стека\n" << endl;
    int size = myStack.size();
    cout << size << "\n" << endl;

    cout << "Проверка for-each" << endl;
    for (auto el : myStack) {
        cout << "--- " << el << " ---" << endl;
    }
    
    free(myStack.arr);
}
