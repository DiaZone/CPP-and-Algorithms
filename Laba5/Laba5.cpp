/*
1)Создайте класс Животное, со свойствами : имя, возраст, сытость(от 0 до 100), усталость и методом играть, спать, есть и проверками на необходимость спать и есть.
2)Создайте класс Кот, Собака, Вомбат, Крыса, каждый из которых будет обладать собственным способом взаимодействия, длительностью сна и циклами питания.
3)Создайте класс Контактный зоопарк, который будет хранить в себе массив животных.
4)Создайте метод проведения осмотра зоопарка сотрудником, в процессе которого животные которым требуется отдых отправляются отдыхать, 
животные которым требуется еда, кормятся.
5)(Усложненный вариант) Смоделируйте работу зоопарка в течении одного месяца. Зоопарк работает по 16 часов, в течении которых животные могут 
взаимодействовать друг с другом, если находятся в одном вольере(вольер с кошками, вольер с собаками, вольер вомбатами, вольер с крысами) с 
некоторой вероятностью. Животное взаимодействующее с другим животным не может быть отправлено спать, играть с посетителем или есть в 
течении шага моделирования. В процессе работы в зоопарк приходят посетители и играют с животными, а так же кормят их разрешенной едой 
небольшого объема. В случае если животное утомилось (усталость > 80), оно отправляется спать работниками его в отдельный вольер
(скорость восстановления определяется типом животного), если животное хочет кушать, оно отправляется на фиксированное время так же 
в отдельный вольер. Наработка усталости и уменьшения сытости обоих параметров определяется типом животного и интенсивностью 
посещения зоопарка в текущий момент. Интенсивность меняется каждый час работы в диапазоне от 0 до 1. Игры между животными увеличивают 
усталость меньше чем с людьми.*/

#include <iostream>
#include <string>
#include <Windows.h>
#include <iomanip>
#include <format>
#include <vector>

using namespace std;

class Animal { 
public:
    string name = ""; //Кличка
    int age = 0; //Возраст
    string animaltype; //Тип животного
    void* panimal; //Указатель на тип животного
    double foodpoints = 100; //Сытость
    double tirepoints = 0; //Усталость
    bool eatflag = false; //Флаг желания покушать
    bool sleepflag = false; //Флаг желания поспать
    bool playingflag = false; //Флаг взаимодействия с другими животными

    //Функция изменения флага взаимодействия
    void playingFlagChanger() {
        if (playingflag) {
            this->playingflag = false;
        }
    }

    //Функция игры с посетителями
    void playing() {
        cout << "\nПосетитель покормил " << name << endl;
        this->foodpoints += 5;
        this->tirepoints += 8;
    }

    //Функция сна
    int sleeping(int sleepcycle, int sleepduration) {
        if (sleepflag) {
            if (sleepduration == 1) {
                tirepoints = 0;
                sleepduration = sleepcycle;
                sleepflag = false;
            }
            sleepduration--;
        }
        return sleepduration;
    }

    //Функция перекуса
    int eating(int eatcycle, int eatduration) {
        if (eatflag) {
            if (eatduration == 1) {
                foodpoints = 100;
                eatduration = eatcycle;
                eatflag = false;
            }
            eatduration--;
        }
        return eatduration;
    }

    //Функция проверки сна
    void sleepcheck() {
        if (tirepoints > 80) {
            sleepflag = true;
        }
        else {
            sleepflag = false;
        }
    }

    //Функция проверки голода
    void eatcheck() {
        if (foodpoints < 30) {
            eatflag = true;
        }
        else {
            eatflag = false;
        }

    }

    //Функция вывода типа животного
    void animalTypePrint() {
        cout << "\nЖивотное: " << animaltype << endl;
    }

    //Функция вывода информации о животном
    void animalInfoPrint() {
        cout << "\nКличка: " << name << "\nВозраст: " << age << "\nСытость: " << foodpoints << "\nУсталость: " << tirepoints << endl;
    }
};

class Cat : public Animal{
public:
    int sleeppoints_change_in_tick = 2 + rand() % 7; //Изменение усталости в тик(час)
    int sleepcycle = 2 ;//Количество циклов, необходимых на отдых
    int sleepduration = 2; //Количество циклов, которые остались у животного на отдых
    int eatpoints_change_in_tick = 4 + rand() % 6; //Количество циклов, необходимых на перекус
    int eatcycle = 3; //Количество циклов, необходимых на перекус
    int eatduration = 3; //Количество циклов, которые остались у животного на перекус

    //Взаимодействие котов
    void catsInteraction(string name1, string name2) {
        cout << "\n" << name1 << " начал играться с " << name2 << endl;
        int temp = 1 + rand() % 5;
        cout << "Усталость +" << temp << endl;
        this->tirepoints += temp;
    }
};

class Dog : public Animal{
public:
    int sleeppoints_change_in_tick = 3 + rand() % 6; //Изменение усталости в тик(час)
    int sleepcycle = 2; //Количество циклов, необходимых на отдых
    int sleepduration = 2; //Количество циклов, которые остались у животного на отдых
    int eatpoints_change_in_tick = 4 + rand() % 8; //Количество циклов, необходимых на перекус
    int eatcycle = 1; //Количество циклов, необходимых на перекус
    int eatduration = 1; //Количество циклов, которые остались у животного на перекус

    //Взаимодействие собак
    void dogsInteraction(string name1, string name2) {
        cout << "\n" << name1 << " начал бегать за " << name2 << endl;
        int temp = 1 + rand() % 5;
        cout << "Усталость +" << temp << endl;
        this->tirepoints += temp;
    }
};

class Wombat : public Animal{
public:
    int sleeppoints_change_in_tick = 5 + rand() % 8; //Изменение усталости в тик(час)
    int sleepcycle = 4; //Количество циклов, необходимых на отдых
    int sleepduration = 4; //Количество циклов, которые остались у животного на отдых
    int eatpoints_change_in_tick = 4 + rand() % 6; //Изменение сытости в тик(час)
    int eatcycle = 3; //Количество циклов, необходимых на перекус
    int eatduration = 3; //Количество циклов, которые остались у животного на перекус

    //Взаимодействие вомбатов
    void wombatsInteraction(string name1, string name2) {
        cout << "\n" << name1 << " начал драться с " << name2 << endl;
        int temp = 1 + rand() % 5;
        cout << "Усталость +" << temp << endl;
        this->tirepoints += temp;
    }
};

class Rat : public Animal{
public:
    double sleeppoints_change_in_tick = 3 + rand() % 4; //Изменение усталости в тик(час)
    int sleepcycle = 3; //Количество циклов, необходимых на отдых
    int sleepduration = 3; //Количество циклов, которые остались у животного на отдых
    double eatpoints_change_in_tick = 4 + rand() % 6; //Изменение сытости в тик(час)
    int eatcycle = 2; //Количество циклов, необходимых на перекус
    int eatduration = 2; //Количество циклов, которые остались у животного на перекус

    //Взаимодействие крыс
    void ratsInteraction(string name1, string name2) {
        cout << "\n" << name1 << " и " << name2 << " побежали бегать\n в колесе " << endl;
        int temp = 1 + rand() % 5;
        cout << "Усталость +" << temp << endl;
        this->tirepoints += temp;
    }
};

class ContactZoo {
public:
    int N; //Размерность массива
    Animal* array_of_animals; //Массив животных
    vector<int> cats; //Вектор с котами
    vector<int> dogs; //Вектор с собаками
    vector<int> wombats; //Вектор с вомбатами
    vector<int> rats; //Вектор с крысами

    //Конструктор с созданием массива под животных
    ContactZoo() {
        setArrayN();
        array_of_animals = new Animal[N];

    }

    //Функция ввода размерности массива
    void setArrayN() {
        cout << "Введите количество животных в зоопарке: ";
        cin >> N;
        if (N < 4) {
            N = 4;
        }
    }

    //Генерация клички
    string randomName(int type) {
        /*
        *@param name_id - индекс массива с кличками
        *@param names - массив с возможными кличками
        *return - на выходе получаем кличку животного
        */
        int name_id = 0;
        string names[] = { "Снежок", "Пушок", "Михалыч", "Барсик",
            "Бобик", "Шарик", "Нюхач", "Арчи",
            "Дизель", "Тесла", "Уголек", "Голиаф",
            "Джерри", "Сквик", "Сплинтер", "Попкорн" };

        if (type == 0) {

            //Генерация индекса имени
            name_id = rand() % 4;
        }
        else if (type == 1) {
            //Генерация индекса имени
            name_id = 4 + rand() % 4;
        }
        else if (type == 2) {
            //Генерация индекса имени
            name_id = 8 + rand() % 4;
        }
        else {
            //Генерация индекса имени
            name_id = 12 + rand() % 4;
        }
        return names[name_id];
    };

    //Функция проверки животного сотрудником
    void zooInspection(int i, double rate) {
        if (array_of_animals[i].animaltype == "Кошак") {

            //Проверяем голод
            static_cast<Cat*>(array_of_animals[i].panimal)->eatcheck();

            //Проверяем усталость
            static_cast<Cat*>(array_of_animals[i].panimal)->sleepcheck();

            //Если животное не играет, то оно может отдыхать и кушать
            if (!static_cast<Cat*>(array_of_animals[i].panimal)->playingflag) {

                //Если животное устало
                if (static_cast<Cat*>(array_of_animals[i].panimal)->sleepflag) {

                    //Оно отправляется на цикл отдыха
                    static_cast<Cat*>(array_of_animals[i].panimal)->sleepduration =
                        static_cast<Cat*>(array_of_animals[i].panimal)->sleeping(
                            static_cast<Cat*>(array_of_animals[i].panimal)->sleepcycle,
                            static_cast<Cat*>(array_of_animals[i].panimal)->sleepduration
                        );
                }
                else {
                    //Иначе оно устает от работы
                    tiring(i, rate);
                }

                //Если животное хочет кушать
                if (static_cast<Cat*>(array_of_animals[i].panimal)->eatflag) {

                    //Оно отправляется покушать
                    static_cast<Cat*>(array_of_animals[i].panimal)->eatduration =
                        static_cast<Cat*>(array_of_animals[i].panimal)->eating(
                            static_cast<Cat*>(array_of_animals[i].panimal)->eatcycle,
                            static_cast<Cat*>(array_of_animals[i].panimal)->eatduration
                        );
                }
                else {
                    //Иначе оно теряет сытость
                    hungry(i, rate);
                }
            }
            else {
                //Смена флага игры
                static_cast<Cat*>(array_of_animals[i].panimal)->playingFlagChanger();
            }
        }
        else if (array_of_animals[i].animaltype == "Собака") {
            static_cast<Dog*>(array_of_animals[i].panimal)->eatcheck();
            static_cast<Dog*>(array_of_animals[i].panimal)->sleepcheck();

            if (!static_cast<Dog*>(array_of_animals[i].panimal)->playingflag) {
                if (static_cast<Dog*>(array_of_animals[i].panimal)->sleepflag) {
                    static_cast<Dog*>(array_of_animals[i].panimal)->sleepduration =
                        static_cast<Dog*>(array_of_animals[i].panimal)->sleeping(
                            static_cast<Dog*>(array_of_animals[i].panimal)->sleepcycle,
                            static_cast<Dog*>(array_of_animals[i].panimal)->sleepduration
                        );
                }
                else {
                    tiring(i, rate);
                }

                if (static_cast<Dog*>(array_of_animals[i].panimal)->eatflag) {
                    static_cast<Dog*>(array_of_animals[i].panimal)->eatduration =
                        static_cast<Dog*>(array_of_animals[i].panimal)->eating(
                            static_cast<Dog*>(array_of_animals[i].panimal)->eatcycle,
                            static_cast<Dog*>(array_of_animals[i].panimal)->eatduration
                        );
                }
                else {
                    hungry(i, rate);
                }
            }
            else {
                static_cast<Dog*>(array_of_animals[i].panimal)->playingFlagChanger();
            }
        }
        else if (array_of_animals[i].animaltype == "Вомбат") {
            static_cast<Wombat*>(array_of_animals[i].panimal)->eatcheck();
            static_cast<Wombat*>(array_of_animals[i].panimal)->sleepcheck();

            if (!static_cast<Wombat*>(array_of_animals[i].panimal)->playingflag) {
                if (static_cast<Wombat*>(array_of_animals[i].panimal)->sleepflag) {
                    static_cast<Wombat*>(array_of_animals[i].panimal)->sleepduration =
                        static_cast<Wombat*>(array_of_animals[i].panimal)->sleeping(
                            static_cast<Wombat*>(array_of_animals[i].panimal)->sleepcycle,
                            static_cast<Wombat*>(array_of_animals[i].panimal)->sleepduration
                        );
                }
                else {
                    tiring(i, rate);
                }

                if (static_cast<Wombat*>(array_of_animals[i].panimal)->eatflag) {
                    static_cast<Wombat*>(array_of_animals[i].panimal)->eatduration =
                        static_cast<Wombat*>(array_of_animals[i].panimal)->eating(
                            static_cast<Wombat*>(array_of_animals[i].panimal)->eatcycle,
                            static_cast<Wombat*>(array_of_animals[i].panimal)->eatduration
                        );
                }
                else {
                    hungry(i, rate);
                }
            }
            else{
                static_cast<Wombat*>(array_of_animals[i].panimal)->playingFlagChanger();
            }
        }
        else {
            static_cast<Rat*>(array_of_animals[i].panimal)->eatcheck();
            static_cast<Rat*>(array_of_animals[i].panimal)->sleepcheck();
            if (!static_cast<Rat*>(array_of_animals[i].panimal)->playingflag) {

                if (static_cast<Rat*>(array_of_animals[i].panimal)->sleepflag) {
                    static_cast<Rat*>(array_of_animals[i].panimal)->sleepduration =
                        static_cast<Rat*>(array_of_animals[i].panimal)->sleeping(
                            static_cast<Rat*>(array_of_animals[i].panimal)->sleepcycle,
                            static_cast<Rat*>(array_of_animals[i].panimal)->sleepduration
                        );
                }
                else {
                    tiring(i, rate);
                }

                if (static_cast<Rat*>(array_of_animals[i].panimal)->eatflag) {
                    static_cast<Rat*>(array_of_animals[i].panimal)->eatduration =
                        static_cast<Rat*>(array_of_animals[i].panimal)->eating(
                            static_cast<Rat*>(array_of_animals[i].panimal)->eatcycle,
                            static_cast<Rat*>(array_of_animals[i].panimal)->eatduration
                        );
                }
                else {
                    hungry(i, rate);
                }
            }
            else {
                static_cast<Rat*>(array_of_animals[i].panimal)->playingFlagChanger();
            }
        }
    }

    void zooGeneration() {
        /*
        *Функция генерации животных
        *type - тип животного
        */
        int type = 0;
        string types_of_animals[] = { "Кошак", "Собака", "Вомбат", "Крыса"};

        for (int i = 0; i < N; i++) {
            //Если введено животных, меньше чем 4, то будет создано все равно 4 разных животных
            if (i > 3) {
                type = rand() % 4;
            }
            else {
                type = i;
            }
            array_of_animals[i].animaltype = types_of_animals[type]; //Запись типа животного
            array_of_animals[i].animalTypePrint();
            if (type == 0) {
                Cat* cat = new Cat; //Создаем объект кота
                cat->name = randomName(type); //Рандомим кличку
                cat->age = rand() % 8; //Рандомим возраст
                array_of_animals[i].panimal = static_cast<void*>(cat);
                cat->animalInfoPrint(); //Вывод характеристик животного
            }
            else if (type == 1) {
                Dog* dog = new Dog;
                dog->name = randomName(type);
                dog->age = rand() % 12;
                array_of_animals[i].panimal = static_cast<void*>(dog);
                dog->animalInfoPrint();
            }
            else if (type == 2) {
                Wombat* wombat = new Wombat;
                wombat->name = randomName(type);
                wombat->age = rand() % 10;
                array_of_animals[i].panimal = static_cast<void*>(wombat);
                wombat->animalInfoPrint();
            }
            else {
                Rat* rat = new Rat;
                rat->name = randomName(type);
                rat->age = rand() % 5;
                array_of_animals[i].panimal = static_cast<void*>(rat);
                rat->animalInfoPrint();
            }
        }

        //Функция подсчета количества разных животных
        zooCounts();
    };

    //Функция срабатывания игры животного с посетителями
    void randomIvent(int i) {

        //Генерируем случайное число, и если оно равно 1, то животное будет играть с посетителем
        int ivent = 1 + rand() % 25;
        if (ivent == 1) {
            if (i == 0) {
                if (!static_cast<Cat*>(array_of_animals[i].panimal)->eatflag && !static_cast<Cat*>(array_of_animals[i].panimal)->sleepflag) {
                    static_cast<Cat*>(array_of_animals[i].panimal)->playing();
                }
            }
            else if (i == 1) {
                if (!static_cast<Dog*>(array_of_animals[i].panimal)->eatflag && !static_cast<Dog*>(array_of_animals[i].panimal)->sleepflag) {
                    static_cast<Dog*>(array_of_animals[i].panimal)->playing();
                }
            }
            else if (i == 2) {
                if (!static_cast<Wombat*>(array_of_animals[i].panimal)->eatflag && !static_cast<Wombat*>(array_of_animals[i].panimal)->sleepflag) {
                    static_cast<Wombat*>(array_of_animals[i].panimal)->playing();
                }
            }
            else {
                if (!static_cast<Rat*>(array_of_animals[i].panimal)->eatflag && !static_cast<Rat*>(array_of_animals[i].panimal)->sleepflag) {
                    static_cast<Rat*>(array_of_animals[i].panimal)->playing();
                }
            }
        }
    }

    //Вывод шапки таблицы
    void zooPrinting() {
        /*
        *Вывод заголовка таблицы
        */
        string str = "";
        cout << setw(37) << "Время" << setw(15) << "Интенсивность";
        for (int i = 0; i < N; i++) {
            str = array_of_animals[i].animaltype + " " + static_cast<Cat*>(array_of_animals[i].panimal)->name + " Возраст " +
                to_string(static_cast<Cat*>(array_of_animals[i].panimal)->age);
            cout << setw(35) << str << "      ";
        }
        cout << endl;
        for (int i = 0; i < N; i++) {
            if (i == 0) {
                cout << setw(62) << " Сытость " << " Питание " << " Усталость " << "  Отдых ";
            }
            else {
                cout << setw(15) << " Сытость " << " Питание " << " Усталость " << "  Отдых ";
            }
        }
        cout << endl;
    }

    //Функция изменения голода
    void hungry(int i, double rate) {
        //Меняем значение голода у животного
        if (array_of_animals[i].animaltype == "Кошак") {
            static_cast<Cat*>(array_of_animals[i].panimal)->foodpoints -= eatpointsChanger(i, rate);
        }
        else if (array_of_animals[i].animaltype == "Собака") {
            static_cast<Dog*>(array_of_animals[i].panimal)->foodpoints -= eatpointsChanger(i, rate);
        }
        else if (array_of_animals[i].animaltype == "Вомбат") {
            static_cast<Wombat*>(array_of_animals[i].panimal)->foodpoints -= eatpointsChanger(i, rate);
        }
        else {
            static_cast<Rat*>(array_of_animals[i].panimal)->foodpoints -= eatpointsChanger(i, rate);
        }
    }

    //Функция получения значения изменения голода
    double eatpointsChanger(int i, double rate) {
        /*
        *return - возвращаем значение, на которое изменится сытость(собственный параметр роста усталости * интенсивность)
        */
        if (array_of_animals[i].animaltype == "Кошак") {
            return static_cast<Cat*>(array_of_animals[i].panimal)->eatpoints_change_in_tick * rate;
        }
        else if (array_of_animals[i].animaltype == "Собака") {
            return static_cast<Dog*>(array_of_animals[i].panimal)->eatpoints_change_in_tick * rate;
        }
        else if (array_of_animals[i].animaltype == "Вомбат") {
            return static_cast<Wombat*>(array_of_animals[i].panimal)->eatpoints_change_in_tick * rate;
        }
        else {
            return static_cast<Rat*>(array_of_animals[i].panimal)->eatpoints_change_in_tick * rate;
        }
    }

    //Функция изменения усталости
    void tiring(int i, double rate) {
        //Меняем значение усталости у животного
        if (array_of_animals[i].animaltype == "Кошак") {
            static_cast<Cat*>(array_of_animals[i].panimal)->tirepoints += tirepointsChanger(i, rate);
        }
        else if (array_of_animals[i].animaltype == "Собака") {
            static_cast<Dog*>(array_of_animals[i].panimal)->tirepoints += tirepointsChanger(i, rate);
        }
        else if (array_of_animals[i].animaltype == "Вомбат") {
            static_cast<Wombat*>(array_of_animals[i].panimal)->tirepoints += tirepointsChanger(i, rate);
        }
        else{
            static_cast<Rat*>(array_of_animals[i].panimal)->tirepoints += tirepointsChanger(i, rate);
        }

    }

    //Функция получения значения изменения усталости
    double tirepointsChanger(int i, double rate) {
        /*
        *return - возвращаем значение, на которое изменится усталость(собственный параметр роста усталости * интенсивность)
        */
        if (array_of_animals[i].animaltype == "Кошак") {
            return static_cast<Cat*>(array_of_animals[i].panimal)->sleeppoints_change_in_tick * rate;
        }
        else if (array_of_animals[i].animaltype == "Собака") {
            return static_cast<Dog*>(array_of_animals[i].panimal)->sleeppoints_change_in_tick * rate;
        }
        else if (array_of_animals[i].animaltype == "Вомбат") {
            return static_cast<Wombat*>(array_of_animals[i].panimal)->sleeppoints_change_in_tick * rate;
        }
        else {
            return static_cast<Rat*>(array_of_animals[i].panimal)->sleeppoints_change_in_tick * rate;
        }
    }

    //Функция вывода информации о животном
    void statsPrinting() {
        /*
        *Функция вывода статистики животного в моделировании работа зоопарка
        */
        for (int i = 0; i < N; i++) {
            if (array_of_animals[i].animaltype == "Кошак") {
                cout << setw(11) << static_cast<Cat*>(array_of_animals[i].panimal)->foodpoints << " ";

                //Если животное сыто
                if (static_cast<Cat*>(array_of_animals[i].panimal)->foodpoints > 30) {
                    cout << setw(10) << "Сыт";
                }

                //Иначе оно голодно
                else {
                    cout << setw(10) << "Голоден";
                }
                cout << setw(10) << static_cast<Cat*>(array_of_animals[i].panimal)->tirepoints;

                //Если животное не устало
                if (static_cast<Cat*>(array_of_animals[i].panimal)->tirepoints < 80) {
                    cout << setw(11) << "Работает";
                }

                //Иначе оно отдыхает
                else {
                    cout << setw(11) << "Отдыхает";
                }
            }
            else if (array_of_animals[i].animaltype == "Собака") {
                cout << setw(11) << static_cast<Dog*>(array_of_animals[i].panimal)->foodpoints << " ";

                //Если животное сыто
                if (static_cast<Dog*>(array_of_animals[i].panimal)->foodpoints > 30) {
                    cout << setw(10) << "Сыт";
                }

                //Иначе оно голодно
                else {
                    cout << setw(10) << "Голоден";
                }
                cout << setw(10) << static_cast<Dog*>(array_of_animals[i].panimal)->tirepoints;

                ////Если животное не устало
                if (static_cast<Dog*>(array_of_animals[i].panimal)->tirepoints < 80) {
                    cout << setw(11) << "Работает";
                }

                //Иначе оно отдыхает
                else {
                    cout << setw(11) << "Отдыхает";
                }
            }
            else if (array_of_animals[i].animaltype == "Вомбат") {
                cout << setw(11) << static_cast<Wombat*>(array_of_animals[i].panimal)->foodpoints << " ";

                //Если животное сыто
                if (static_cast<Wombat*>(array_of_animals[i].panimal)->foodpoints > 30) {
                    cout << setw(10) << "Сыт";
                }

                //Иначе оно голодно
                else {
                    cout << setw(10) << "Голоден";
                }
                cout << setw(10) << static_cast<Wombat*>(array_of_animals[i].panimal)->tirepoints;

                ////Если животное не устало
                if (static_cast<Wombat*>(array_of_animals[i].panimal)->tirepoints < 80) {
                    cout << setw(11) << "Работает";
                }

                //Иначе оно отдыхает
                else {
                    cout << setw(11) << "Отдыхает";
                }
            }
            else {
                cout << setw(11) << static_cast<Rat*>(array_of_animals[i].panimal)->foodpoints << " ";

                //Если животное сыто
                if (static_cast<Rat*>(array_of_animals[i].panimal)->foodpoints > 30) {
                    cout << setw(10) << "Сыт";
                }

                //Иначе оно голодно
                else {
                    cout << setw(10) << "Голоден";
                }
                cout << setw(10) << static_cast<Rat*>(array_of_animals[i].panimal)->tirepoints;

                ////Если животное не устало
                if (static_cast<Rat*>(array_of_animals[i].panimal)->tirepoints < 80) {
                    cout << setw(11) << "Работает";
                }

                //Иначе оно отдыхает
                else {
                    cout << setw(11) << "Отдыхает";
                }
            }

        }
    }

    void zooModeling() {
        /*
        *@param rate - интенсивность посещения
        */
        double rate = 0.0;

        //Проходим по дням месяца
        for (int months = 0; months < 2; months++) {

            //Проходим по часам
            for (int hours = 0; hours < 16; hours++) {
                rate = (rand() % 100) / static_cast<double>(100); //Рандомно генерируем интенсивность
                zooContact();
                for (int i = 0; i < N; i++) {
                    zooInspection(i, rate);

                }
                for (int i = 0; i < N; i++) {
                    randomIvent(i);
                }
                cout << setw(30) << "";
                printf("  %02d %02d", months + 1, hours + 1);
                cout << setw(10) << rate << " ";
                statsPrinting(); //Выводим статистику
                cout << endl;
            }
        }
    }

    //Функция подсчета количества разных животных
    void zooCounts() {

        string types_of_animals[] = { "Кошак", "Собака", "Вомбат", "Крыса" };
        for (int i = 0; i < N; i++) {
            if (array_of_animals[i].animaltype == "Кошак") {
                cats.push_back(i);
            }
            else if (array_of_animals[i].animaltype == "Собака") {
                dogs.push_back(i);
            }
            else if (array_of_animals[i].animaltype == "Вомбат") {
                wombats.push_back(i);
            }
            else{
                rats.push_back(i);
            }
        }

        for (auto i : cats) {
            cout << i << ' ';
        };
        cout << endl;
        for (auto i : dogs) {
            cout << i << ' ';
        };
        cout << endl;
        for (auto i : wombats) {
            cout << i << ' ';
        };
        cout << endl;
        for (auto i : rats) {
            cout << i << ' ';
        };
    }

    //Функция осуществления игры животных друг с другом
    void zooContact() {
        //Генерируем случайное число, и если оно равно 1, то животное будет играть друг с другом
        int ivent = 1 + rand() % 10;
        if (ivent == 1) {
            if (cats.size() > 1) {

                //Если котов несколько, то они могут играть друг с другом
                int cats1 = cats[rand() % cats.size()]; //выбираем рандомно двух животных
                int cats2 = cats[rand() % cats.size()]; //выбираем рандомно двух животных
                while (cats1 == cats2) {
                    cats2 = cats[rand() % cats.size()]; 
                }
                //Проверяем, что эти животные не спят и не играют
                if (!static_cast<Cat*>(array_of_animals[cats1].panimal)->eatflag && !static_cast<Cat*>(array_of_animals[cats1].panimal)->sleepflag &&
                    !static_cast<Cat*>(array_of_animals[cats2].panimal)->eatflag && !static_cast<Cat*>(array_of_animals[cats2].panimal)->sleepflag) {

                    //Вызываем функцию взаимодействия двух животных
                    static_cast<Cat*>(array_of_animals[cats1].panimal)->
                        catsInteraction(static_cast<Cat*>(array_of_animals[cats1].panimal)->name, static_cast<Cat*>(array_of_animals[cats2].panimal)->name);

                    //Меняем флаг игры на true у первого животного
                    static_cast<Cat*>(array_of_animals[cats1].panimal)->playingflag = true;

                    //Вызываем функцию взаимодействия двух животных
                    static_cast<Cat*>(array_of_animals[cats2].panimal)->
                        catsInteraction(static_cast<Cat*>(array_of_animals[cats2].panimal)->name, static_cast<Cat*>(array_of_animals[cats1].panimal)->name);

                    //Меняем флаг игры на true у второго животного
                    static_cast<Cat*>(array_of_animals[cats2].panimal)->playingflag = true;
                }
            }
            else if (dogs.size() > 1) {
                //Если собак несколько, то они могут играть друг с другом
                int dogs1 = dogs[rand() % cats.size()]; //выбираем рандомно двух животных
                int dogs2 = dogs[rand() % cats.size()]; //выбираем рандомно двух животных
                while (dogs1 == dogs2) {
                    dogs2 = dogs[rand() % cats.size()];
                }
                //Проверяем, что эти животные не спят и не играют
                if (!static_cast<Dog*>(array_of_animals[dogs1].panimal)->eatflag && !static_cast<Dog*>(array_of_animals[dogs1].panimal)->sleepflag &&
                    !static_cast<Dog*>(array_of_animals[dogs2].panimal)->eatflag && !static_cast<Dog*>(array_of_animals[dogs2].panimal)->sleepflag) {

                    //Вызываем функцию взаимодействия двух животных
                    static_cast<Dog*>(array_of_animals[dogs1].panimal)->
                        dogsInteraction(static_cast<Dog*>(array_of_animals[dogs1].panimal)->name, static_cast<Dog*>(array_of_animals[dogs2].panimal)->name);

                    //Меняем флаг игры на true у первого животного
                    static_cast<Dog*>(array_of_animals[dogs1].panimal)->playingflag = true;

                    //Вызываем функцию взаимодействия двух животных
                    static_cast<Dog*>(array_of_animals[dogs2].panimal)->
                        dogsInteraction(static_cast<Dog*>(array_of_animals[dogs2].panimal)->name, static_cast<Dog*>(array_of_animals[dogs1].panimal)->name);

                    //Меняем флаг игры на true у второго животного
                    static_cast<Dog*>(array_of_animals[dogs2].panimal)->playingflag = true;
                }
            }
            else if (wombats.size() > 1) {
                //Если вомбатов несколько, то они могут играть друг с другом
                int wombats1 = wombats[rand() % wombats.size()]; //выбираем рандомно двух животных 
                int wombats2 = wombats[rand() % wombats.size()]; //выбираем рандомно двух животных
                while (wombats1 == wombats2) {
                    wombats2 = wombats[rand() % wombats.size()];
                }
                //Проверяем, что эти животные не спят и не играют
                if (!static_cast<Wombat*>(array_of_animals[wombats1].panimal)->eatflag && !static_cast<Wombat*>(array_of_animals[wombats1].panimal)->sleepflag &&
                    !static_cast<Wombat*>(array_of_animals[wombats2].panimal)->eatflag && !static_cast<Wombat*>(array_of_animals[wombats2].panimal)->sleepflag) {

                    //Вызываем функцию взаимодействия двух животных
                    static_cast<Wombat*>(array_of_animals[wombats1].panimal)->
                        wombatsInteraction(static_cast<Wombat*>(array_of_animals[wombats1].panimal)->name, static_cast<Wombat*>(array_of_animals[wombats2].panimal)->name);

                    //Меняем флаг игры на true у первого животного
                    static_cast<Wombat*>(array_of_animals[wombats1].panimal)->playingflag = true;

                    //Вызываем функцию взаимодействия двух животных
                    static_cast<Wombat*>(array_of_animals[wombats2].panimal)->
                        wombatsInteraction(static_cast<Wombat*>(array_of_animals[wombats2].panimal)->name, static_cast<Wombat*>(array_of_animals[wombats1].panimal)->name);

                    //Меняем флаг игры на true у второго животного
                    static_cast<Wombat*>(array_of_animals[wombats2].panimal)->playingflag = true;
                }
            }
            else if (rats.size() > 1) {
                //Если крыс несколько, то они могут играть друг с другом
                int rats1 = rats[rand() % rats.size()]; //выбираем рандомно двух животных
                int rats2 = rats[rand() % rats.size()]; //выбираем рандомно двух животных
                while (rats1 == rats2) {
                    rats2 = rats[rand() % rats.size()];
                }
                //Проверяем, что эти животные не спят и не играют
                if (!static_cast<Rat*>(array_of_animals[rats1].panimal)->eatflag && !static_cast<Rat*>(array_of_animals[rats1].panimal)->sleepflag &&
                    !static_cast<Rat*>(array_of_animals[rats2].panimal)->eatflag && !static_cast<Rat*>(array_of_animals[rats2].panimal)->sleepflag) {

                    //Вызываем функцию взаимодействия двух животных
                    static_cast<Rat*>(array_of_animals[rats1].panimal)->
                        ratsInteraction(static_cast<Rat*>(array_of_animals[rats1].panimal)->name, static_cast<Rat*>(array_of_animals[rats2].panimal)->name);

                    //Меняем флаг игры на true у первого животного
                    static_cast<Rat*>(array_of_animals[rats1].panimal)->playingflag = true;

                    //Вызываем функцию взаимодействия двух животных
                    static_cast<Rat*>(array_of_animals[rats2].panimal)->
                        ratsInteraction(static_cast<Rat*>(array_of_animals[rats2].panimal)->name, static_cast<Rat*>(array_of_animals[rats1].panimal)->name);

                    //Меняем флаг игры на true у второго животного
                    static_cast<Rat*>(array_of_animals[rats2].panimal)->playingflag = true;
                }
            }
        }
    }
};


int main()
{
    srand(time(0)); //Инициализация рандомайзера
    SetConsoleCP(1251); //Поддержка ввода на русском языке
    setlocale(LC_ALL, "Russian"); //Установка русской локали
    ContactZoo zoo;
    zoo.zooGeneration();
    zoo.zooPrinting();
    zoo.zooModeling();
}
