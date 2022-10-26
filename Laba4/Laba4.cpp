/*
1) Создайте класс Кот, Собака, Вомбат со свойствами: имя, возраст, сытость (от 0 до 100), усталость.
2) Создайте класс Животное, который будет хранить указатель на пустоту, за которым скрывается конкретное животное, а так же его тип.
3) Создайте класс Контактный зоопарк, который будет хранить в себе массив животных.
4) Заполните зоопарк случайными животными в количестве введенным пользователем, так что бы было хотя бы по одному животному одного вида.
5) Добавьте методы у зоопарка для добавления нового одного животного которого введет пользователь.
6) (Усложнение) Смоделируйте работу смотрителя зоопарка в течении одного рабочего дня(12 часов), задача которого наблюдать за животными, 
    в случае если животное утомилось (усталость > 80), он отправляет его в отдельный вольер спать(скорость восстановления определяется типом животного), 
    если животное хочет кушать, он отправляется на фиксированное время так же в отдельный вольер. 
    Наработка усталости и уменьшения сытости обоих параметров определяется типом животного и интенсивностью посещения зоопарка в текущий момент. 
    Интенсивность меняется каждый час работы в диапазоне от 0 до 1.*/

#include <iostream>
#include <string>
#include <Windows.h>
#include <iomanip>
#include <format>

using namespace std;

class AllAnimals {
public:
    string name; //имя
    int age; //возраст
    double satiation = 100; //сытость
    double fatigue = 0; //усталость

    //Вывод информации о животном
    void animalPrint() {
        cout << "\nКличка: " << name << "\nВозраст: " << age << "\nСытость: " << satiation << "\nУсталость: " << fatigue << endl;
    }

};

class Cat : public AllAnimals {
public:
    double appetite = 5; //Изменение аппетита в тик
    double sleepiness = 16; //Изменение усталости в тик
    double sleepinesrate = 10; //Скорость восстановления усталости
    bool sleep = true; //Флаг, отдыхало животное
};

class Dog : public AllAnimals {
public:
    double appetite = 10; //Изменение аппетита в тик
    double sleepiness = 8; //Изменение усталости в тик
    double sleepinesrate = 5; //Скорость восстановления усталости
    bool sleep = true; //Флаг, отдыхало животное
};

class Wombat : public AllAnimals {
public:
    double appetite = 7;  //Изменение аппетита в тик
    double sleepiness = 12; //Изменение усталости в тик
    double sleepinesrate = 8; //Скорость восстановления усталости
    bool sleep = true; //Флаг, отдыхало животное
};

class Animal {
public:
    void* panimal; //Пустой указатель на тип
    string animal_type; //Тип животного


    //Вывод типа животного
    void animalTypePrint() { 
        cout << "\nЖивотное: " << animal_type << endl;
    }
};

class Zoo {
public:
    int N; //Размерность массива
    Animal* array_animals; //Массив животных
    Animal* new_array_animals; //Временный массив животных

    Zoo() { //Конструктор с вводом размерности массива животных
        cout << "Введите количество животных в зоопарке: ";
        cin >> this->N;
        if (this->N < 3) {
            this->N = 3;
        }
        array_animals = new Animal[N];
    }

    void newArray(int count) { //Создание нового массива при добавлении нового животного пользователем
        N = N + count; //Увеличиваем размерность
        new_array_animals = new Animal[N]; //Создаем новый временный массив
        for (int i = 0; i < N - count; i++) {
            new_array_animals[i] = array_animals[i]; //Копируем старый массив в новый
        }
        delete[] array_animals; //Очистка старого массива
        array_animals = new Animal[N]; //Создаем главный массив
        for (int i = 0; i < N - count; i++) {
            array_animals[i] = new_array_animals[i]; //Перезаписываем из временного массива в главный
        }
        delete[] new_array_animals; //Очистка временного массива
    }

    void addNewAnimal(int count){
        /*
        *@param key - ключ выбора животного
        *@param name - кличка животного
        *@param age - возраст животного
        */
        int key;
        string name = "";
        int age = 0;
        cout << "\nВыберите кого нужно добавить:\n"
            "[0] Кот\n"
            "[1] Собака\n"
            "[2] Вомбат\n";
        cin >> key;
        cout << "Введите кличку животного: ";
        cin >> name;
        cout << "Введите возраст животного: ";
        cin >> age;

        //Если это кот
        if(key == 0){
            Cat * cat = new Cat; //Создаем объект кота
            cat->name = name; //Вводим имя
            cat->age = age; //Вводим возраст
            array_animals[this->N - count].panimal = static_cast<Cat*>(cat); //Смена указателя на тип животного
            array_animals[this->N - count].animal_type = "Кошак"; //Запись типа животного
            cat->animalPrint(); //Вывод характеристик животного
        }

        //Если это собака
        else if (key == 1) {
            Dog* dog = new Dog;
            dog->name = name;
            dog->age = age;
            array_animals[this->N - count].panimal = static_cast<Dog*>(dog);
            array_animals[this->N - count].animal_type = "Собака";
            dog->animalPrint();
        }
        
        //Если это вомбат
        else if (key == 2) {
            Wombat* wombat = new Wombat;
            wombat->name = name;
            wombat->age = age;
            array_animals[this->N - count].panimal = static_cast<Wombat*>(wombat);
            array_animals[this->N - count].animal_type = "Вомбат";
            wombat->animalPrint();
        }
        else{
            cout << "Неверный ключ";
        }
    }
};


string randomName(int type); //Генерация клички
void printAllAnimals(Zoo zoo); //Функция вывод информации о животных
double sleepinessChanger(double sleep, double rate); //Функция изменения состояния усталости
double appetiteChanger(Zoo zoo, int i, double rate); //Функция изменения состояния голода
void hungry(Zoo zoo, int i, double rate); //Функция работы системы голода
void sleeptime(Zoo zoo, int i, double rate); //Функция работы системы отдыха
void sleeping(double fat, Zoo zoo, int i, double rate); //Функция проверки выхода из отдыха
void statsPrinting(Zoo zoo, int i); //Функция вывода информации о животном
void zooGenerator(Zoo zoo); //Генерация животных
void zooPrinting(Zoo zoo); //Вывод шапки таблицы
void zooModeling(Zoo zoo); //Модель работы зоопарка с выводом в табличном виде


int main()
{
    /*
    *@param key - ключ выбора добавления
    *@param count - количество животных для добавления
    */
    srand(time(0)); //Инициализация рандомайзера
    SetConsoleCP(1251); //Поддержка ввода на русском языке
    setlocale(LC_ALL, "Russian"); //Установка русской локали
    Zoo ContactZoo;
    zooGenerator(ContactZoo); //Генерация животных
   
    int key = 0;
    int count = 0;
    cout << "\nДобавить новое животное?\n[0] Нет\n[1] Да\n";
    cin >> key;
    switch (key) {
    case 0:
        break;
    case 1:
        cout << "\nСколько животных хотите добавить?\n";
        cin >> count;
        ContactZoo.newArray(count); //Создание массива
        for (int i = count; i > 0; i--) {
            ContactZoo.addNewAnimal(i); //Добавление животного
            cout << "------------------------------------------------------------";
        }
        printAllAnimals(ContactZoo); //Вывод в консоль животного
        break;
    default:
        cout << "Неверный ключ";
        break;
    }

    zooPrinting(ContactZoo);
    zooModeling(ContactZoo);
    delete[] ContactZoo.array_animals;
}

string randomName(int type) {
    /*
    *@param name_id - индекс массива с кличками
    *@param names - массив с возможными кличками
    *return - на выходе получаем кличку животного
    */
    int name_id = 0;
    string names[] = { "Снежок", "Пушок", "Михалыч", "Барсик", "Бобик", "Шарик", "Нюхач", "Арчи", "Дизель", "Тесла", "Уголек", "Голиаф"};

    if (type == 0) {

        //Генерация индекса имени
        name_id = rand() % 4;
    }
    else if (type == 1) {
        //Генерация индекса имени
        name_id = 4 + rand() % 4;
    }
    else {
        //Генерация индекса имени
        name_id = 8 + rand() % 4;
    }
    return names[name_id];
}

void printAllAnimals(Zoo zoo) {
    /*
    * Функция вывод типа и информации о животном
    */
    for (int i = 0; i < zoo.N; i++) {
        zoo.array_animals[i].animalTypePrint(); //Выводим информацию о типе
        if (zoo.array_animals[i].animal_type == "Кошак") {
            static_cast<Cat*>(zoo.array_animals[i].panimal)->animalPrint(); //Выводим информацию о самом животном
        }
        else if (zoo.array_animals[i].animal_type == "Собака") {
            static_cast<Dog*>(zoo.array_animals[i].panimal)->animalPrint(); //Выводим информацию о самом животном
        }
        else{
            static_cast<Wombat*>(zoo.array_animals[i].panimal)->animalPrint(); //Выводим информацию о самом животном
        }
    }
}

double appetiteChanger(Zoo zoo, int i, double rate) {
    /*
    *return - возвращаем значение, на которое изменится сытость
    */
    if (zoo.array_animals[i].animal_type == "Кошак") {
        return static_cast<Cat*>(zoo.array_animals[i].panimal)->appetite * rate;
    }
    else if (zoo.array_animals[i].animal_type == "Собака") {
        return static_cast<Dog*>(zoo.array_animals[i].panimal)->appetite * rate;
    }
    else {
        return static_cast<Wombat*>(zoo.array_animals[i].panimal)->appetite * rate;
    }
}

void hungry(Zoo zoo, int i, double rate) {
    /*
    *Функция голода животного
    */

    if (zoo.array_animals[i].animal_type == "Кошак") {

        //Если животное голодно, то оно идет кушать и сразу восстанавливает голод
        if (static_cast<Cat*>(zoo.array_animals[i].panimal)->satiation <= 30) {
            //cout << static_cast<Cat*>(zoo.array_animals[i].panimal)->name << " кушоет" << endl;
            static_cast<Cat*>(zoo.array_animals[i].panimal)->satiation = 100;
        }

        //Иначе животное голодает
        else {
            static_cast<Cat*>(zoo.array_animals[i].panimal)->satiation -= appetiteChanger(zoo, i, rate);
        }
    }
    else if (zoo.array_animals[i].animal_type == "Собака") {

        //Если животное голодно, то оно идет кушать и сразу восстанавливает голод
        if (static_cast<Dog*>(zoo.array_animals[i].panimal)->satiation <= 30) {
            //cout << static_cast<Dog*>(zoo.array_animals[i].panimal)->name << " кушоет" << endl;
            static_cast<Dog*>(zoo.array_animals[i].panimal)->satiation = 100;
        }

        //Иначе животное голодает
        else {
            static_cast<Dog*>(zoo.array_animals[i].panimal)->satiation -= appetiteChanger(zoo, i, rate);
        }
    }
    else {

        //Если животное голодно, то оно идет кушать и сразу восстанавливает голод
        if (static_cast<Wombat*>(zoo.array_animals[i].panimal)->satiation <= 30) {
            //cout << static_cast<Wombat*>(zoo.array_animals[i].panimal)->name << " кушоет" << endl;
            static_cast<Wombat*>(zoo.array_animals[i].panimal)->satiation = 100;
        }
        //Иначе животное голодает
        else {
            static_cast<Wombat*>(zoo.array_animals[i].panimal)->satiation -= appetiteChanger(zoo, i, rate);
        }
    }
}

double sleepinessChanger(double rate, double sleep) {
    /*
    *return - возвращаем значение, на которое изменится усталость
    */
    return sleep * rate;
}

void sleeptime(Zoo zoo, int i, double rate) {
    /*
    *Функция изменения усталости животного
    *@param fat - усталость животного
    */
    double fat = 0.0;
    if (zoo.array_animals[i].animal_type == "Кошак") {
        fat = static_cast<Cat*>(zoo.array_animals[i].panimal)->fatigue;

        //Если усталость выше 80 или животное еще не отдохнуло, то оно продолжает отдыхать
        if (fat >= 80 || !static_cast<Cat*>(zoo.array_animals[i].panimal)->sleep) {
            static_cast<Cat*>(zoo.array_animals[i].panimal)->sleep = false;
            sleeping(fat, zoo, i, rate);
        }
        else {
            //Иначе животное бодрствует и устает
            static_cast<Cat*>(zoo.array_animals[i].panimal)->fatigue +=
                sleepinessChanger(static_cast<Cat*>(zoo.array_animals[i].panimal)->sleepiness, rate);
        }
    }
    else if (zoo.array_animals[i].animal_type == "Собака") {

        //Если усталость выше 80 или животное еще не отдохнуло, то оно продолжает отдыхать
        fat = static_cast<Dog*>(zoo.array_animals[i].panimal)->fatigue;
        if (fat >= 80 || !static_cast<Dog*>(zoo.array_animals[i].panimal)->sleep) {
            static_cast<Dog*>(zoo.array_animals[i].panimal)->sleep = false;
            sleeping(fat, zoo, i, rate); //Проверяем, отдохнуло ли животное
        }
        else {
            //Иначе животное бодрствует и устает
            static_cast<Dog*>(zoo.array_animals[i].panimal)->fatigue +=
                sleepinessChanger(static_cast<Dog*>(zoo.array_animals[i].panimal)->sleepiness, rate); //Меняем значение усталости животного
        }
    }
    else {

        //Если усталость выше 80 или животное еще не отдохнуло, то оно продолжает отдыхать
        fat = static_cast<Wombat*>(zoo.array_animals[i].panimal)->fatigue;
        if (fat >= 80 || !static_cast<Wombat*>(zoo.array_animals[i].panimal)->sleep) {
            static_cast<Wombat*>(zoo.array_animals[i].panimal)->sleep = false;
            sleeping(fat, zoo, i, rate);
        }
        else {
            //Иначе животное бодрствует и устает
            static_cast<Wombat*>(zoo.array_animals[i].panimal)->fatigue +=
                sleepinessChanger(static_cast<Wombat*>(zoo.array_animals[i].panimal)->sleepiness, rate); //Меняем значение усталости животного
        }
    }

}

void sleeping(double fat, Zoo zoo, int i, double rate) {
    /*
    *Функция проверки выхода животного из отдыха
    */
    if (fat < 30) {
        //Если животное отдохнуло, то меняем флаг с отдыха на нет и оно начинает уставать
        static_cast<Wombat*>(zoo.array_animals[i].panimal)->sleep = true;
        //cout << static_cast<Wombat*>(zoo.array_animals[i].panimal)->name << " отдохнул" << endl;
        static_cast<Wombat*>(zoo.array_animals[i].panimal)->fatigue +=
            sleepinessChanger(static_cast<Wombat*>(zoo.array_animals[i].panimal)->sleepiness, rate);
    }
    else {
        //Иначе оно потихоньку отдыхает
        static_cast<Wombat*>(zoo.array_animals[i].panimal)->fatigue -=
            sleepinessChanger(static_cast<Wombat*>(zoo.array_animals[i].panimal)->sleepinesrate, 1);
        //cout << static_cast<Wombat*>(zoo.array_animals[i].panimal)->name << " отдыхает" << endl;
    }
}

void statsPrinting(Zoo zoo, int i) {
    /*
    *Функция вывода статистики животного в моделировании работа зоопарка
    */
    if (zoo.array_animals[i].animal_type == "Кошак") {
        cout << setw(11) << static_cast<Cat*>(zoo.array_animals[i].panimal)->satiation << " ";

        //Если животное сыто
        if (static_cast<Cat*>(zoo.array_animals[i].panimal)->satiation > 30) {
            cout << setw(10) << "Сыт";
        }

        //Иначе оно голодно
        else {
            cout << setw(10) << "Голоден";
        }
        cout << setw(10) << static_cast<Cat*>(zoo.array_animals[i].panimal)->fatigue;

        //Если животное не устало
        if (static_cast<Cat*>(zoo.array_animals[i].panimal)->sleep) {
            cout << setw(11) << "Работает";
        }

        //Иначе оно отдыхает
        else {
            cout << setw(11) << "Отдыхает";
        }
    }
    else if (zoo.array_animals[i].animal_type == "Собака") {
        cout << setw(11) << static_cast<Dog*>(zoo.array_animals[i].panimal)->satiation << " ";

        //Если животное сыто
        if (static_cast<Dog*>(zoo.array_animals[i].panimal)->satiation > 30) {
            cout << setw(10) << "Сыт";
        }

        //Иначе оно голодно
        else {
            cout << setw(10) << "Голоден";
        }
        cout << setw(10) << static_cast<Dog*>(zoo.array_animals[i].panimal)->fatigue;

        ////Если животное не устало
        if (static_cast<Dog*>(zoo.array_animals[i].panimal)->sleep) {
            cout << setw(11) << "Работает";
        }

        //Иначе оно отдыхает
        else {
            cout << setw(11) << "Отдыхает";
        }
    }
    else {
        cout << setw(11) << static_cast<Wombat*>(zoo.array_animals[i].panimal)->satiation << " ";

        //Если животное сыто
        if (static_cast<Wombat*>(zoo.array_animals[i].panimal)->satiation > 30) {
            cout << setw(10) << "Сыт";
        }

        //Иначе оно голодно
        else {
            cout << setw(10) << "Голоден";
        }
        cout << setw(10) << static_cast<Wombat*>(zoo.array_animals[i].panimal)->fatigue;

        //Если животное не устало
        if (static_cast<Wombat*>(zoo.array_animals[i].panimal)->sleep) {
            cout << setw(11) << "Работает";
        }

        //Иначе оно отдыхает
        else {
            cout << setw(11) << "Отдыхает";
        }
    }
} 

void zooGenerator(Zoo zoo) {
    /*
    *Функция генерации животных
    *type - тип животного
    */
    int type = 0;
    string types_of_animals[] = { "Кошак", "Собака", "Вомбат" };
    for (int i = 0; i < zoo.N; i++) {
        //Если введено животных, меньше чем 3, то будет создано все равно 3 разных животных
        if (i > 2) {
            type = rand() % 3;
        }
        else {
            type = i;
        }
        zoo.array_animals[i].animal_type = types_of_animals[type]; //Запись типа животного
        zoo.array_animals[i].animalTypePrint();
        if (type == 0) {
            Cat* cat = new Cat; //Создаем объект кота
            cat->name = randomName(type); //Рандомим кличку
            cat->age = rand() % 8; //Рандомим возраст
            zoo.array_animals[i].panimal = static_cast<Cat*>(cat); //Смена указателя на тип животного
            cat->animalPrint(); //Вывод характеристик животного
        }
        else if (type == 1) {
            Dog* dog = new Dog;
            dog->name = randomName(type);
            dog->age = rand() % 12;
            zoo.array_animals[i].panimal = static_cast<Dog*>(dog);
            dog->animalPrint();
        }
        else {
            Wombat* wombat = new Wombat;
            wombat->name = randomName(type);
            wombat->age = rand() % 10;
            zoo.array_animals[i].panimal = static_cast<Wombat*>(wombat);
            wombat->animalPrint();
        }
    }
}

void zooPrinting(Zoo zoo) {
    /*
    *Вывод заголовка таблицы
    */
    string str = "";
    cout << setw(7) << "Время" << setw(15) << "Интенсивность";
    for (int i = 0; i < zoo.N; i++) {
        if (zoo.array_animals[i].animal_type == "Кошак") {
            str = zoo.array_animals[i].animal_type + " " + static_cast<Cat*>(zoo.array_animals[i].panimal)->name + " Возраст " +
                to_string(static_cast<Cat*>(zoo.array_animals[i].panimal)->age);

        }
        else if (zoo.array_animals[i].animal_type == "Собака") {
            str = zoo.array_animals[i].animal_type + " " + static_cast<Dog*>(zoo.array_animals[i].panimal)->name + " Возраст " +
                to_string(static_cast<Dog*>(zoo.array_animals[i].panimal)->age);
        }
        else {
            str = zoo.array_animals[i].animal_type + " " + static_cast<Wombat*>(zoo.array_animals[i].panimal)->name + " Возраст " +
                to_string(static_cast<Wombat*>(zoo.array_animals[i].panimal)->age);
        }
        cout << setw(35) << str << "      ";
    }
    cout << endl;
    for (int i = 0; i < zoo.N; i++) {
        if (i == 0) {
            cout << setw(32) << " Сытость " << " Питание " << " Усталость " << "  Отдых ";
        }
        else {
            cout << setw(15) << " Сытость " << " Питание " << " Усталость " << "  Отдых ";
        }
    }
    cout << endl;
}

void zooModeling(Zoo zoo) {
    /*
    *@param rate - интенсивность посещения
    */
    double rate = 0.0;

    //Проходим по часам
    for (int hours = 0; hours < 12; hours++) {
        rate = (rand() % 100) / static_cast<double>(100); //Рандомно генерируем интенсивность

        //Проходим по минутам(каждые 15 минут)
        for (int minutes = 0; minutes < 4; minutes++) {
            printf("  %02d:%02d", hours + 8, minutes * 15);
            cout << setw(10) << rate << " ";
            for (int i = 0; i < zoo.N; i++) {
                if (zoo.array_animals[i].animal_type == "Кошак") {
                    statsPrinting(zoo, i); //Выводим статистику
                    hungry(zoo, i, rate); //Меняем голод
                    sleeptime(zoo, i, rate); //Меняем усталость
                }
                else if (zoo.array_animals[i].animal_type == "Собака") {
                    statsPrinting(zoo, i);
                    hungry(zoo, i, rate);
                    sleeptime(zoo, i, rate);
                }
                else {
                    statsPrinting(zoo, i);
                    hungry(zoo, i, rate);
                    sleeptime(zoo, i, rate);
                }
            }
            cout << endl;
        }
    }
}
