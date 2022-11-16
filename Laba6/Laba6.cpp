/*
Вам необходимо реализовать некоторое количество классов оружий которые будут тестироваться пользователем на стрельбище с целью его подбора. При этом:

1)У каждого оружия есть свой магазин и вариант его перезарядки со своим временем требуемым на его перезарядку++
2)пистолет перезаряжается быстро, простым изъятием магазина и вставкой нового++
3)однозарядная винтовка, перезаряжается по 1 патрону++
4)помповый дробовик имеет несъемный магазин и поэтому перезаряжается по 1 патрону до заполнения всего магазина++
5)дробовики и винтовки со съемными магазинами перезаряжаются подобно пистолетам++
6)Каждое оружие имеет следующие характеристики: как калибр, количество патронов,
количество режимов стрельбы, вес, прицельная дальность стрельбы, отдельные экземпляры кучность стрельбы, количество устанавливаемых модификаций.++
7)Шанс попадания по мишени зависит исключительно от расстояния до нее и характеристик самого оружия, ++
так дробовик имеет практически 0 вероятность попасть в мишень в полукилометре, пистолет на том же расстоянии имеет 
незначительный шанс попадания, штурмовая винтовка средние шансы, а снайперская винтовка очень приличные шансы. ++

В итоге шанс зависит от прицельной дальности стрельбы конкретного оружия. Формулу расчета шанса попадания требуется придумать,
так чтобы она описывалась графиком в котором точка максимальной прицельной дальности стрельбы соответствовала бы 100% шансу попадания из 
этого оружия, а дальше резко падала в зависимости от расстояния, для этого можно использовать обратную экспоненты, для которой точка 0.1 
будет прицельной дальностью, а дальше будет резко падать но никогда не приблизится к 0.++

8)Шанс попадания каждый последующим выстрелом для режима стрельбы очередями должен падать.+
9)Любое оружие должно предоставлять следующие методы:
1. Зарядка магазина, на вход принимается количество патронов на выходе выдается время зарядки,
    при этом количество патроном не должно превышать максимальный объем магазина, но может быть меньше.++
2. Стрельба по мишени одним из режимов стрельбы, при этом оружие на вход получает мишень, а на выходе выдает количество попаданий по цели++
3. Вывод характеристик оружия ++

Требуется:

1)Реализовать класс стрельбища на котором будет находится оружие и мишени одного типа на разном расстоянии от стрелка.++
2)Реализовать интерфейсный или абстрактный класс оружия и несколько его конечных вариантов.++
3)(На половину баллов) Продемонстрировать работу стрельбища и оружий.++
4)(Усложненный вариант) Провести симуляцию проверки работоспособности оружия стрелком, 
при проверки работоспособности оружия, требуется вывести информацию о нем, а также провести стрельбы на все расстояния, 
по каждой мишени требуется совершить по 100 выстрелов и замерить среднюю скорострельность в минуту(с учетом времени перезарядки на все выстрелы) 
и точность на каждое из доступных расстояний мишеней.
5)(Усложненный вариант) В конце симуляции вывести собранную статистику по всем оружиям.
*/

#include <iostream>
#include <Windows.h>
#include <iomanip>
#include <format>
#include <string>
#include <math.h>

using namespace std;

//Мишень
class Target {
public:
    double distance = 0.0; //Дистанция до мишени
    double chance = 0.0;

    //Вывод дистанции
    void printDistance() {
        cout << "\nДистанция до мишени: " << distance << endl;
    }

    //Рассчет шанса попадания по мишени
    void chanceCalc(double sighting_range) {
        /*
        *@param my_chance - шанс на попадание в цель
        *return - количество попаданий
        */
        //Расчет шанса(сама формула была подобрана методом подстановки, учитывая что оружия должны и попадать, и промахиваться(возможно не самая лучшая))
        chance = (double)(1 / exp((distance - sighting_range) * 0.003));

        //Если у нас оружие стреляет дальше, чем стоит цель, то логично, что оно попадет в неё
        if (chance > 0.95) {
            chance = 0.95;
        }
        cout << "Шанс попадания: " << chance << endl;

    };
};

//Оружие
class Weapon {
public:
    string weapon_name = ""; //Модель оружия
    string caliber = ""; //Калибр оружия
    int magazine_capacity = 0; //Емкость магазина
    int count_of_rounds_in_magazine = 0; //Количество патронов в магазине
    int count_of_shooting_mode = 0; //Количество режимов стрельбы
    double weight = 0.0; //Вес
    double sighting_range = 0.0; //Прицельная дальность
    double accuracy_of_shooting = 0.0; //Кучность стрельбы
    int number_of_modifications = 0; //Количество модификаций
    double chance = 0; //Шанс попадания
    int shooting_mode = 0; //Режим стрельбы
    double time_for_shoot = 0; //Время на выстрелы

    //Функция перезарядки магазина
    virtual double reloadMagazine(int rounds) = 0;

    //Функция стрельбы(только для одиночного режима);
    virtual int shooting(Target target, int distance, double chance) = 0;

    //Функция вывода названия оружия
    virtual void printInfo() {
        cout << weapon_name << endl;
    };

    //Случайный режим стрельбы(работает только для автомата)
    void shootingMode() {

        //Если режимов больше, чем 1(одиночный), то есть только он
        if (count_of_shooting_mode == 1) {
            shooting_mode = 1;
        }

        //Иначе у нас есть стрельба очередью или автоматический огонь
        else if (count_of_shooting_mode > 1) {
            shooting_mode = 1 + rand() % count_of_shooting_mode;
        }
    }

};

//Класс пистолетов
class Pistol : public Weapon {
public:
    Pistol() {
        int type = rand() % 6; //Случайно генерируем ключ
        weapon_name = pistolName(type); //Вытаскиваем название по ключу
        caliber = pistolCaliber(type); //Вытаскиваем калибр по ключу
        magazine_capacity = pistolMagazineCapacity(type); //Вытаскиваем емкость магазина по ключу
        count_of_rounds_in_magazine = magazine_capacity; //Изначально магазин заряжен полностью
        count_of_shooting_mode = 1; //Количество режимов стрельбы
        weight = pistolWeight(type); //Вес
        sighting_range = 150; //Дальность стрельбы
        accuracy_of_shooting = 0.15; //Кучность стрельбы
        number_of_modifications = rand() % 3; //Случайно получаем количество модификаций
        number_of_modifications == 1 ? accuracy_of_shooting -= 0.05 : accuracy_of_shooting -= 0; //Если одна, то это глушитель
        number_of_modifications == 2 ? sighting_range += 10, accuracy_of_shooting -= 0.05 : sighting_range += 0, accuracy_of_shooting -= 0; //Если два, то глушитель и целик
        }

    //Функция стрельбы(для одинарного режима)
    int shooting(Target target, int distance, double chance){
        /*
        *@param count_of_hits - счетчик попаданий
        *@param patron - количество патронов в магазине
        *@param chance_checker - шанс, который нужно превысить шансу на попадание, чтобы попасть
        *return - количество попаданий
        */
        time_for_shoot = 0; //Обнуляем время на отстрел магазина
        int count_of_hits = 0;
        int patron = 0;
        double chance_checker;
        int patrons = count_of_rounds_in_magazine; //Получаем количество патрон в магазине

        //Проходимся по всем доступным патронам
        for (int i = 0; i < patrons; i++) {

            //Добавляем немного рандома во время на выстрел(чтобы значения не повторялись при различных дистанциях)
            time_for_shoot += (1.0 * (distance / static_cast<double>(100)) * ((double)rand() / (double)RAND_MAX));

            //Отстреливаем патрон
            count_of_rounds_in_magazine--;

            //Получаем шанс
            chance_checker = (double)rand() / (double)RAND_MAX;
            cout << "Шанс: " << chance_checker << endl;

            //Если шанс на выстрел выше этого шанса, то попадание было
            if (chance >= chance_checker) {
                count_of_hits++;
            }
        }
        return count_of_hits;
    }

    //Функция вывода информации об оружии
    void printInfo() {
        cout << "Пистолет: " << weapon_name << endl;
        cout << "Калибр: " << caliber << endl;
        cout << "Ёмкость магазина: " << magazine_capacity << endl;
        cout << "Количество режимов стрельбы: " << count_of_shooting_mode << endl;
        cout << "Вес оружия: " << weight << endl;
        cout << "Дальность стрельбы: " << sighting_range << endl;
        cout << "Кучность стрельбы: " << accuracy_of_shooting << endl;
        cout << "Модификации: ";
        if (number_of_modifications == 1) {
            cout << "глушитель" << endl;
        }
        else if (number_of_modifications == 2) {
            cout << "глушитель, лазерный целеуказатель" << endl;
        }
        else {
            cout << "нет" << endl;
        }
        cout << "Режим стрельбы: одиночными\n" << endl;
    }

    //Функция перезарядка оружия(магазин)
    double reloadMagazine(int rounds) {
        /*
        *@param time - фиксированое время перезарядки оружия
        *return - время перезарядки
        */
        cout << "\n\n[Перезарядка]" << endl;
        double time = 5;

        //Если патронов больше чем доступно мест в магазине, то "достаем" полный магазин и убираем патроны, которые в нем(сделано для учета отстрела 100 патронов)
        if (rounds > magazine_capacity) {
            count_of_rounds_in_magazine = magazine_capacity;
        }
        //Если патронов меньше максимального места, то даем магазин с патронами, которые остались
        else {
            count_of_rounds_in_magazine = rounds;
        }
        cout << "[Вставлен новый магазин]\n" << endl;
        if (rounds - count_of_rounds_in_magazine > 0) {
            cout << "[Осталось лишних патронов: " << rounds - count_of_rounds_in_magazine << "]\n" << endl;
        }
        return time;
    }

    //Функция "генерации" модели
    string pistolName(int i) {
        string names[] = {"Beretta M9", "Colt M1911", "Glock 17" , "H&K P2000" , "Desert Eagle" , "SIG Sauer P226"};
        return names[i];
    }

    //Функция "генерации" калибра
    string pistolCaliber(int i) {
        string calibers[] = { "9x19", ".45 ACP", "9x19" , "9x19" , ".44 Magnum" , ".357 SIG" };
        return calibers[i];
    }

    //Функция "генерации" емкости магазина
    int pistolMagazineCapacity(int i) {
        int magazineCapacity[] = {15, 7, 17, 13, 8, 13};
        return magazineCapacity[i];
    }

    //Функция "генерации" веса
    double pistolWeight(int i) {
        double weight[] = {0.952, 1.12, 0.905, 0.620, 2, 0.867};
        return weight[i];
    }
};

//Класс однозарядных винтовок
class SingleShotRifle : public Weapon {
public:

    SingleShotRifle() {
        int type = rand() % 3;
        weapon_name = singleShotRifleName(type);
        caliber = singleShotRifleCaliber(type);
        magazine_capacity = singleShotRifleMagazineCapacity(type);
        count_of_rounds_in_magazine = magazine_capacity;
        count_of_shooting_mode = 1;
        weight = singleShotRifleWeight(type);
        sighting_range = 300;
        accuracy_of_shooting = 0.07;
        number_of_modifications = 0;
    }

    //Функция стрельбы(для одинарного режима)
    int shooting(Target target, int distance, double chance) {
        /*
        *@param count_of_hits - счетчик попаданий
        *@param patron - количество патронов в магазине
        *@param chance_checker - шанс, который нужно превысить шансу на попадание, чтобы попасть
        *return - количество попаданий
        */
        time_for_shoot = 0; //Обнуляем время на отстрел магазина
        int count_of_hits = 0;
        int patron = 0;
        double chance_checker;
        int patrons = count_of_rounds_in_magazine; //Получаем количество патрон в магазине

        //Проходимся по всем доступным патронам
        for (int i = 0; i < patrons; i++) {

            //Добавляем немного рандома во время на выстрел(чтобы значения не повторялись при различных дистанциях)
            time_for_shoot += (1.0 * (distance / static_cast<double>(100)) * ((double)rand() / (double)RAND_MAX));

            //Отстреливаем патрон
            count_of_rounds_in_magazine--;

            //Получаем шанс
            chance_checker = (double)rand() / (double)RAND_MAX;
            cout << "Шанс: " << chance_checker << endl;

            //Если шанс на выстрел выше этого шанса, то попадание было
            if (chance >= chance_checker) {
                count_of_hits++;
            }
        }
        return count_of_hits;
    }

    //Функция перезарядки оружия по патрону
    double reloadMagazine(int rounds) {
        /*
        *@param time - время с которым перезаряжается оружие
        *return - время перезарядки
        */
        double time = 0.0;

        //Проходимся по пустому месту в магазине
        for (int i = count_of_rounds_in_magazine; i < magazine_capacity; i++) {
            time += 2.5;

            //Если патроны закончились
            if (rounds == 0) {
                break;
            }
            //+1 патрон в магазине, -1 патрон в хранилище
            count_of_rounds_in_magazine++;
            rounds--;
        }

        //Если остались лишние патроны
        if (rounds > 0) {
            cout << "Осталось лишних патронов: " << rounds << endl;
        }
        return time;
    }

    //Функция вывода информации об оружии
    void printInfo() {
        cout << "\nОднозарядная винтовка: " << weapon_name << endl;
        cout << "Калибр: " << caliber << endl;
        cout << "Ёмкость магазина: " << magazine_capacity << endl;
        cout << "Количество режимов стрельбы: " << count_of_shooting_mode << endl;
        cout << "Вес оружия: " << weight << endl;
        cout << "Дальность стрельбы: " << sighting_range << endl;
        cout << "Кучность стрельбы: " << accuracy_of_shooting << endl;
        cout << "Модификации: " << "нет\n" << endl;
    }

    //Функция "генерации" модели
    string singleShotRifleName(int i) {
        string names[] = { "Winchester Model 67", "Winchester Model 121", "Винтовка Бердана"};
        return names[i];
    }

    //Функция "генерации" калибра
    string singleShotRifleCaliber(int i) {
        string calibers[] = { ".22 Long", ".22 Long", "10,75x58"};
        return calibers[i];
    }

    //Функция "генерации" емкости магазина
    int singleShotRifleMagazineCapacity(int i) {
        int magazineCapacity[] = { 1, 1, 1, 1, 1 };
        return magazineCapacity[i];
    }

    //Функция "генерации" веса
    double singleShotRifleWeight(int i) {
        double weight[] = { 2.3, 2.3, 4.2, 10, 12.4 };
        return weight[i];
    }
};

//Класс снайперских винтовок(с магазином)
class SniperRifle : public Weapon {
public:
    SniperRifle() {
        int type = rand() % 5;
        weapon_name = sniperRifleName(type);
        caliber = sniperRifleCaliber(type);
        magazine_capacity = sniperRifleMagazineCapacity(type);
        count_of_rounds_in_magazine = magazine_capacity;
        count_of_shooting_mode = 1;
        weight = sniperRifleWeight(type);
        sighting_range = 500;
        accuracy_of_shooting = 0.07;
        number_of_modifications = rand() % 2;
        number_of_modifications == 1 ? sighting_range += 100 : sighting_range += 0;
        number_of_modifications == 2 ? sighting_range += 200 : sighting_range += 0;
    }
    
    //Функция стрельбы(для одинарного режима)
    int shooting(Target target, int distance, double chance) {
        /*
        *@param count_of_hits - счетчик попаданий
        *@param patron - количество патронов в магазине
        *@param chance_checker - шанс, который нужно превысить шансу на попадание, чтобы попасть
        *return - количество попаданий
        */
        time_for_shoot = 0; //Обнуляем время на отстрел магазина
        int count_of_hits = 0;
        int patron = 0;
        double chance_checker;
        int patrons = count_of_rounds_in_magazine; //Получаем количество патрон в магазине

        //Проходимся по всем доступным патронам
        for (int i = 0; i < patrons; i++) {

            //Добавляем немного рандома во время на выстрел(чтобы значения не повторялись при различных дистанциях)
            time_for_shoot += (1.0 * (distance / static_cast<double>(100)) * ((double)rand() / (double)RAND_MAX));

            //Отстреливаем патрон
            count_of_rounds_in_magazine--;

            //Получаем шанс
            chance_checker = (double)rand() / (double)RAND_MAX;
            cout << "Шанс: " << chance_checker << endl;

            //Если шанс на выстрел выше этого шанса, то попадание было
            if (chance >= chance_checker) {
                count_of_hits++;
            }
        }
        return count_of_hits;
    }

    //Функция вывода информации об оружии
    void printInfo() override{
        cout << "\nСнайперская винтовка: " << weapon_name << endl;
        cout << "Калибр: " << caliber << endl;
        cout << "Ёмкость магазина: " << magazine_capacity << endl;
        cout << "Количество режимов стрельбы: " << count_of_shooting_mode << endl;
        cout << "Вес оружия: " << weight << endl;
        cout << "Дальность стрельбы: " << sighting_range << endl;
        cout << "Кучность стрельбы: " << accuracy_of_shooting << endl;
        cout << "Модификации: ";
        if (number_of_modifications == 1) {
            cout << "глушитель\n" << endl;
        }
        else if (number_of_modifications == 2) {
            cout << "глушитель, сошки\n" << endl;
        }
        else {
            cout << "нет\n" << endl;
        }
    }

    //Функция перезарядка оружия(магазин)
    double reloadMagazine(int rounds) {
        /*
        *@param time - фиксированое время перезарядки оружия
        *return - время перезарядки
        */
        cout << "\n\n[Перезарядка]" << endl;

        //Если патронов больше чем доступно мест в магазине, то "достаем" полный магазин и убираем патроны, которые в нем(сделано для учета отстрела 100 патронов)
        double time = 20;

        //Если патронов меньше максимального места, то даем магазин с патронами, которые остались
        if (rounds > magazine_capacity) {
            count_of_rounds_in_magazine = magazine_capacity;
        }

        //Если патронов меньше максимального места, то даем магазин с патронами, которые остались
        else {
            count_of_rounds_in_magazine = rounds;
        }
        cout << "[Вставлен новый магазин]\n" << endl;
        if (rounds - count_of_rounds_in_magazine > 0) {
            cout << "[Осталось лишних патронов: " << rounds - count_of_rounds_in_magazine << "]\n" << endl;
        }
        return time;
    }

    //Функция "генерации" модели
    string sniperRifleName(int i) {
        string names[] = {"Barrett M82", "ВСС Винторез", "СВ-98", "M24", "ВССК Выхлоп" };
        return names[i];
    }

    //Функция "генерации" калибра
    string sniperRifleCaliber(int i) {
        string calibers[] = {"12,7x99" , "СП-5", ".338 Lapua Magnum", "7,62x51", "12,7x55" };
        return calibers[i];
    }

    //Функция "генерации" емкости магазина
    int sniperRifleMagazineCapacity(int i) {
        int magazineCapacity[] = { 10, 10, 10, 5, 5 };
        return magazineCapacity[i];
    }

    //Функция "генерации" веса
    double sniperRifleWeight(int i) {
        double weight[] = { 13.5, 3.7, 6.2, 7.26, 6.5 };
        return weight[i];
    }
};

//Класс помповых дробовиков
class SlideShotgun : public Weapon {
public:
    SlideShotgun() {
        int type = rand() % 2;
        weapon_name = slideShotgunName(type);
        caliber = slideShotgunCaliber(type);
        magazine_capacity = slideShotgunMagazineCapacity(type);
        count_of_rounds_in_magazine = magazine_capacity;
        count_of_shooting_mode = 1;
        weight = slideShotgunWeight(type);
        sighting_range = 50;
        accuracy_of_shooting = 0.40;
        number_of_modifications = rand() % 1;
    }

    //Функция стрельбы(для одинарного режима)
    int shooting(Target target, int distance, double chance) {
        /*
        *@param count_of_hits - счетчик попаданий
        *@param patron - количество патронов в магазине
        *@param chance_checker - шанс, который нужно превысить шансу на попадание, чтобы попасть
        *return - количество попаданий
        */
        time_for_shoot = 0; //Обнуляем время на отстрел магазина
        int count_of_hits = 0;
        int patron = 0;
        double chance_checker;
        int patrons = count_of_rounds_in_magazine; //Получаем количество патрон в магазине

        //Проходимся по всем доступным патронам
        for (int i = 0; i < patrons; i++) {

            //Добавляем немного рандома во время на выстрел(чтобы значения не повторялись при различных дистанциях)
            time_for_shoot += (1.0 * (distance / static_cast<double>(100)) * ((double)rand() / (double)RAND_MAX));

            //Отстреливаем патрон
            count_of_rounds_in_magazine--;

            //Получаем шанс
            chance_checker = (double)rand() / (double)RAND_MAX;
            cout << "Шанс: " << chance_checker << endl;

            //Если шанс на выстрел выше этого шанса, то попадание было
            if (chance >= chance_checker) {
                count_of_hits++;
            }
        }
        return count_of_hits;
    }

    //Функция перезарядки оружия по патрону
    double reloadMagazine(int rounds) {
        /*
        *@param time - время с которым перезаряжается оружие
        *return - время перезарядки
        */
        double time = 0.0;

        //Проходимся по пустому месту в магазине
        for (int i = count_of_rounds_in_magazine; i < magazine_capacity; i++) {
            time += 2.5;

            //Если патроны закончились
            if (rounds == 0) {
                break;
            }
            //+1 патрон в магазине, -1 патрон в хранилище
            count_of_rounds_in_magazine++;
            rounds--;
        }

        //Если остались лишние патроны
        if (rounds > 0) {
            cout << "Осталось лишних патронов: " << rounds << endl;
        }
        return time;
    }

    //Функция вывода информации об оружии
    void printInfo() {
        cout << "Помповый дробовик: " << weapon_name << endl;
        cout << "Калибр: " << caliber << endl;
        cout << "Ёмкость магазина: " << magazine_capacity << endl;
        cout << "Количество режимов стрельбы: " << count_of_shooting_mode << endl;
        cout << "Вес оружия: " << weight << endl;
        cout << "Дальность стрельбы: " << sighting_range << endl;
        cout << "Кучность стрельбы: " << accuracy_of_shooting << endl;
        cout << "Модификации: ";
        if (number_of_modifications == 1) {
            cout << "фонарик\n" << endl;
        }
        else {
            cout << "нет\n" << endl;
        }

    }

    //Функция "генерации" модели
    string slideShotgunName(int i) {
        string names[] = { "Benelli M4", "Remington 870" };
        return names[i];
    }

    //Функция "генерации" калибра
    string slideShotgunCaliber(int i) {
        string calibers[] = { "12" , "12" };
        return calibers[i];
    }

    //Функция "генерации" емкости магазина
    int slideShotgunMagazineCapacity(int i) {
        int magazineCapacity[] = { 6, 7 };
        return magazineCapacity[i];
    }

    //Функция "генерации" веса
    double slideShotgunWeight(int i) {
        double weight[] = { 3.82, 3.2 };
        return weight[i];
    }
};

//Класс обычных дробовиков(с магазином)
class Shotgun : public Weapon {
public:
    Shotgun() {
        int type = rand() % 2;
        weapon_name = shotgunName(type);
        caliber = shotgunCaliber(type);
        magazine_capacity = shotgunMagazineCapacity(type);
        count_of_rounds_in_magazine = magazine_capacity;
        count_of_shooting_mode = 1;
        weight = shotgunWeight(type);
        sighting_range = 50;
        accuracy_of_shooting = 0.40;
        number_of_modifications = rand() % 1;
    }

    //Функция стрельбы(для одинарного режима)
    int shooting(Target target, int distance, double chance) {
        /*
        *@param count_of_hits - счетчик попаданий
        *@param patron - количество патронов в магазине
        *@param chance_checker - шанс, который нужно превысить шансу на попадание, чтобы попасть
        *return - количество попаданий
        */
        time_for_shoot = 0; //Обнуляем время на отстрел магазина
        int count_of_hits = 0;
        int patron = 0;
        double chance_checker;
        int patrons = count_of_rounds_in_magazine; //Получаем количество патрон в магазине

        //Проходимся по всем доступным патронам
        for (int i = 0; i < patrons; i++) {

            //Добавляем немного рандома во время на выстрел(чтобы значения не повторялись при различных дистанциях)
            time_for_shoot += (1.0 * (distance / static_cast<double>(100)) * ((double)rand() / (double)RAND_MAX));

            //Отстреливаем патрон
            count_of_rounds_in_magazine--;

            //Получаем шанс
            chance_checker = (double)rand() / (double)RAND_MAX;
            cout << "Шанс: " << chance_checker << endl;

            //Если шанс на выстрел выше этого шанса, то попадание было
            if (chance >= chance_checker) {
                count_of_hits++;
            }
        }
        return count_of_hits;
    }

    //Функция вывода информации об оружии
    void printInfo() {
        cout << "\nДробовик: " << weapon_name << endl;
        cout << "Калибр: " << caliber << endl;
        cout << "Ёмкость магазина: " << magazine_capacity << endl;
        cout << "Количество режимов стрельбы: " << count_of_shooting_mode << endl;
        cout << "Вес оружия: " << weight << endl;
        cout << "Дальность стрельбы: " << sighting_range << endl;
        cout << "Кучность стрельбы: " << accuracy_of_shooting << endl;
        cout << "Модификации: ";
        if (number_of_modifications = 1) {
            cout << "фонарик\n" << endl;
        }
        else {
            cout << "нет\n" << endl;
        }
    }

    //Функция перезарядка оружия(магазин)
    double reloadMagazine(int rounds) {
        cout << "\n\n[Перезарядка]" << endl;
        double time = 10;
        if (rounds > magazine_capacity) {
            count_of_rounds_in_magazine = magazine_capacity;
        }
        else {
            count_of_rounds_in_magazine = rounds;
        }
        cout << "[Вставлен новый магазин]\n" << endl;
        if (rounds - count_of_rounds_in_magazine > 0) {
            cout << "[Осталось лишних патронов: " << rounds - count_of_rounds_in_magazine << "]\n" << endl;
        }
        return time;
    }

    //Функция "генерации" модели
    string shotgunName(int i) {
        string names[] = { "Вепрь-12 Молот", "Сайга-12" };
        return names[i];
    }

    //Функция "генерации" калибра
    string shotgunCaliber(int i) {
        string calibers[] = { "12" , "12/70" };
        return calibers[i];
    }

    //Функция "генерации" емкости магазина
    int shotgunMagazineCapacity(int i) {
        int magazineCapacity[] = { 8, 5 };
        return magazineCapacity[i];
    }

    //Функция "генерации" веса
    double shotgunWeight(int i) {
        double weight[] = { 4.2, 3.6 };
        return weight[i];
    }
};

//Класс штурмовых винтовок
class AssaultRifle : public Weapon {
public:
    AssaultRifle() {
        int type = rand() % 5;
        weapon_name = assaultRifleName(type);
        caliber = assaultRifleCaliber(type);
        magazine_capacity = assaultRifleCapacity(type);
        count_of_rounds_in_magazine = magazine_capacity;
        count_of_shooting_mode = 3;
        weight = assaultRifleWeight(type);
        sighting_range = 250;
        accuracy_of_shooting = 0.25;
        number_of_modifications = //rand() % 3;
        number_of_modifications > 0 ? accuracy_of_shooting -= 0.05 : accuracy_of_shooting -= 0;
        number_of_modifications > 1 ? magazine_capacity += 10, count_of_rounds_in_magazine += 10 : magazine_capacity += 0;
        number_of_modifications > 2 ? sighting_range += 20 : sighting_range += 20;
        shootingMode();
    }

    //Функция вывода информации об оружии
    void printInfo() override {
        cout << "\nАвтомат: " << weapon_name << endl;
        cout << "Калибр: " << caliber << endl;
        cout << "Ёмкость магазина: " << magazine_capacity << endl;
        cout << "Количество режимов стрельбы: " << count_of_shooting_mode << endl;
        cout << "Вес оружия: " << weight << endl;
        cout << "Дальность стрельбы: " << sighting_range << endl;
        cout << "Кучность стрельбы: " << accuracy_of_shooting << endl;
        cout << "Модификации: ";
        if (number_of_modifications == 1) {
            cout << "глушитель\n" << endl;
        }
        else if(number_of_modifications == 2) {
            cout << "глушитель, увеличенный магазин\n" << endl;
        }
        else if (number_of_modifications == 3) {
            cout << "глушитель, увеличенный магазин, лазерный целеуказатель\n" << endl;
        }
        cout << "Режим стрельбы: ";
        if (shooting_mode == 1) {
            cout << "одиночными\n" << endl;
        }
        else if (shooting_mode == 2) {
            cout << "очередью\n" << endl;
        }
        else {
            cout << "автоматический\n" << endl;
        }
    }

    //Функция перезарядка оружия(магазин)
    double reloadMagazine(int rounds) {
        cout << "\n\n[Перезарядка]" << endl;
        double time = 14.0;
        if (rounds > magazine_capacity) {
            count_of_rounds_in_magazine = magazine_capacity;
        }
        else {
            count_of_rounds_in_magazine = rounds;
        }
        cout << "[Вставлен новый магазин]\n" << endl;
        if (rounds - count_of_rounds_in_magazine > 0) {
            cout << "[Осталось лишних патронов: " << rounds - count_of_rounds_in_magazine << "]\n" << endl;
        }
        return time;
    }

    //Функция стрельбы(для трех режимов)
    int shooting(Target target, int distance, double chance) {
        /*
        *@param count_of_hits - счетчик попаданий
        *@param patron - количество патронов в магазине
        *@param chance_checker - шанс, который нужно превысить шансу на попадание, чтобы попасть
        *return - количество попаданий
        */
        time_for_shoot = 0;
        int count_of_hits = 0;
        int patron = 0;
        double chance_checker;

        int patrons = count_of_rounds_in_magazine;

        //Проходимся по всем доступным патронам
        for (int i = 0; i < patrons; i++) {

            //Отстреливаем патрон
            count_of_rounds_in_magazine--;

            //Получаем шанс
            chance_checker = (double)rand() / (double)RAND_MAX;

            cout << "Шанс: " << chance_checker << endl;
            if (shooting_mode == 2) {

                //Добавляем немного рандома во время на выстрел(чтобы значения не повторялись при различных дистанциях)
                //При стрельбе отсечкой требует меньше времени, чем при одиночном режиме, но больше, чем при автоматическим
                time_for_shoot += (0.77 * (distance / static_cast<double>(100)) * ((double)rand() / (double)RAND_MAX));
                patron = (i + 1) % 3;

                //При стрельбе в режиме отсечки, я уменьшаю шанс попадания каждого следующего патрона на 0,1, но
                //однако первый патрон летит со стандартной точностью
                if (patron != 0) {
                    chance -= 0.1;
                }
                else {
                    chance += 0.2;
                }
            }
            else if (shooting_mode == 1) {

                //Добавляем немного рандома во время на выстрел(чтобы значения не повторялись при различных дистанциях)
                //Стандартное время на выстрел 1 патрона в одиночном режиме
                time_for_shoot += (1.0 * (distance / static_cast<double>(100)) * ((double)rand() / (double)RAND_MAX));
            }
            else {

                //Добавляем немного рандома во время на выстрел(чтобы значения не повторялись при различных дистанциях)
                //При стрельбе автоматическим огнем логично, что пули вылетать будут быстрее, и понадобиться меньшее время
                time_for_shoot += (0.44 * (distance / static_cast<double>(100)) * ((double)rand() / (double)RAND_MAX));
                patron = (i + 1);

                //При стрельбе в автоматическом режиме, я уменьшаю шанс попадания каждого следующего патрона на 0,07, но
                //только на первые 6 патронов после первого выстрела
                if ((patron != 1) && (patron <= 7)) {
                    chance -= 0.07;
                }
            }
            //Если достигаем нужного шанса, то есть пробитие
            if (chance >= chance_checker) {
                count_of_hits++;
            }
        }
        return count_of_hits;
    }

    //Функция "генерации" модели
    string assaultRifleName(int i) {
        string names[] = { "AK-103", "FN SCAR-L", "HK416", "Steyr AUG", "Zastava M70"};
        return names[i];
    }

    //Функция "генерации" калибра
    string assaultRifleCaliber(int i) {
        string calibers[] = { "7.62x39" , "5.56x45", "5.56x45", "5.56x45", "7.62x39"};
        return calibers[i];
    }

    //Функция "генерации" емкости магазина
    int assaultRifleCapacity(int i) {
        int magazineCapacity[] = { 30, 30, 30, 30, 30 };
        return magazineCapacity[i];
    }

    //Функция "генерации" веса
    double assaultRifleWeight(int i) {
        double weight[] = { 3.6, 3.04, 3.6, 3.850, 3.70 };
        return weight[i];
    }
};

//Класс стрельбища
class ShootingRange {
public:
    int N = 4; //Количество мишеней
    Weapon* my_weapon; //Оружие на стрельбище
    Target* my_targets; //Мишени на стрельбище


    ShootingRange(Weapon* weapon) {
        /*
        *@param dist - дистанция мишени, увеличивается с каждой мишенью в 2 раза
        */
        int dist = 100;

        //Задаем цели
        my_targets = new Target[N];

        //Кладем нужное оружие на стойку стрельбища
        my_weapon = weapon; 

        //Устанавливаем мишени на свои места
        for (int i = 0; i < N; i++) {
            my_targets[i].distance = dist;
            dist *= 2;
        }
    }
};

//Функция проведения испытаний и вывода отчета о нем
void simulationOfWeaponsTest(const int N, Weapon* all_weapons[]);

int main()
{
    const int N = 6;    //Количество оружий на стрельбище
    srand(time(0)); //Инициализация рандомайзера
    SetConsoleCP(1251); //Поддержка ввода на русском языке
    setlocale(LC_ALL, "Russian"); //Установка русской локали
    int shoot = 0;
    double bullets = 0;
    double time = 0.0;

    //Создаем по объекту на каждое оружие
    Pistol pistol = Pistol();
    SingleShotRifle singleshotrifle = SingleShotRifle();
    SniperRifle sniperrifle = SniperRifle();
    SlideShotgun slideshotgun = SlideShotgun();
    Shotgun shotgun = Shotgun();
    AssaultRifle assaultrifle = AssaultRifle();
    Weapon* all_weapons[N] = { &pistol, &singleshotrifle, &sniperrifle, &slideshotgun, &shotgun, &assaultrifle };
    simulationOfWeaponsTest(N, all_weapons);

};

//Функция проведения испытаний и вывода отчета о нем
void simulationOfWeaponsTest(const int N, Weapon* all_weapons[]) {
    /*
    *@param statistic[][][] - трехмерный массив с информацией о каждом испытании всех оружий
    *@param shoot - количество попаданий с отстрела магазина
    *@param bullets - количество попаданий по одной мишене
    *@param time - время, затраченное на стрельбу по одной мишене
    */
    double statistic[6][5][5]{};
    int shoot = 0;
    double bullets = 0;
    double time = 0.0;
    for (int i = 0; i < N; i++) {

        //Получаю статистику об оружии
        all_weapons[i]->printInfo();
        cout << "--------------------------------" << endl;
        cout << "Результат на стрельбище:\n";

        //Создаю "дорожку" под определенное оружие и со своими мишенями
        ShootingRange sr = ShootingRange(all_weapons[i]);
        for (int j = 0; j < sr.N; j++) {

            //Вывожу информацию о дистанции до мишени
            sr.my_targets[j].printDistance();

            //Генерирую шанс попадания по цели
            sr.my_targets[j].chanceCalc(all_weapons[i]->sighting_range);

            //Количество патронов по условию
            int ammo = 100;

            //Пока они не закончатся, оружие будет стрелять
            while (ammo > 0) {

                //Если в магазине есть патроны
                if (all_weapons[i]->count_of_rounds_in_magazine > 0) {
                    shoot = all_weapons[i]->shooting(sr.my_targets[j], sr.my_targets[j].distance, sr.my_targets[j].chance); //Считаем попадания
                    time += all_weapons[i]->time_for_shoot; //Считаем время
                    bullets += shoot; //Итоговое количество попаданий
                }

                //Иначе нужно перезарядиться
                else {

                    //Убираем патроны из общего количества в магазин
                    ammo -= all_weapons[i]->magazine_capacity;

                    //Если патронов есть, то перезаряжаем
                    if (ammo > 0) {
                        time += all_weapons[i]->reloadMagazine(ammo);
                    }
                }
            }

            //Полная перезарядка оружия перед следующей мишенью
            all_weapons[i]->reloadMagazine(all_weapons[i]->magazine_capacity);


            cout << "\nКоличество попаданий: " << bullets << endl;
            //cout << "\nСекунд, затраченное на стрельбу - " << time << endl;
            //cout << "\nСредняя точность стрельбы - " << (double)(bullets/100) << endl;

            //Внос информации в массив со статистикой 
            statistic[i][j][0] = sr.my_targets[j].distance; //Дистанция до мишени
            statistic[i][j][1] = bullets; //Количество попаданий
            statistic[i][j][2] = time; //Затраченное время
            statistic[i][j][3] = (double)(bullets / 100); //Средний шанс попадания
            statistic[i][j][4] = (double)(time / 60); //Скорострельность в минуту
            bullets = 0;
            time = 0;
        }

        cout << "--------------------------------" << endl;

        //Удаляем мишени
        delete[] sr.my_targets;
    }

    //Вывод статистики
    cout << "\n--------------------------------------------" << endl;
    cout << "|                   Отчет                  |" << endl;
    cout << "--------------------------------------------\n" << endl;
    for (int i = 0; i < N; i++) {
        cout << "\n------------------------------------------\n";
        all_weapons[i]->printInfo();
        for (int j = 0; j < 4; j++) {
            cout << "|" << endl;
            cout << "|Дистанция до мишени: " << statistic[i][j][0] << endl;
            cout << "|Количество попаданий: " << statistic[i][j][1] << endl;
            cout << "|Секунд, затраченное на стрельбу - " << statistic[i][j][2] << endl;
            cout << "|Средняя точность стрельбы - " << statistic[i][j][3] << endl;
            cout << "|Скорострельность в минуту - " << statistic[i][j][4] << endl;
            cout << "|" << endl;
        }
        cout << "------------------------------------------\n";
    }
}
