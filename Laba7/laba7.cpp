/*Оружие:
1)Оружие содержит магазин на 30 патронов.
2)При стрельбе, оружие забирает патрон из магазина и выстреливает его, вызывая демонстрацию эффекта патрона.На выстрел тратиться время.
3)Оружие не может самостоятельно перезарядить свой магазин.
Магазин:
1)Магазин содержит 30 патронов.
2)Патроны заряжаются в магазин по одному и кладутся внутри него в стопку.
3)Выходят патроны из магазина в порядке обратном порядку их помещения. (Время на зарядку больше чем на изъятие)
4)На помещение одного патрона в магазин тратится определенное время. На изъятие патрона из магазина, так же тратится время.
Патрон:
1)Патроны делятся на 3 типа: дозвуковые, обычные(сверх звуковые), трассирующие.
2)Патроны одного типа имеют одинаковые свойства.
3)Патроны разных типов имеют разных эффект от выстрела

1.Реализовать класс Оружие, Магазин, Патрон.
2.Реализовать метод заполнения магазина патронами из предустановленного набора патронов.
3.Реализовать метод изъятия патрона из магазина.
4.Реализовать метод выстрела патрона из оружия.
5.Реализовать процесс стрельбы из оружия. Процесс циклический до тех пор пока не кончатся все патроны в сгенерированной коробке. При этом:
    В начале стрельбы генерируется коробка патронов содержащая от 300 до 500 разных патронов.
    Перед стрельбой в магазин заряжается 30 патронов из коробки (запрещается прямое взаимодействие с хранилищем патронов в магазине).
    Магазин помещается в оружие.
    Оружие выстреливает патроны изымая их из магазина по одному (запрещается прямое взаимодействие с хранилищем патронов в магазине).
    После окончания патронов, магазин изымается из оружия и производится повторная зарядка.
6.После завершения процесса стрельбы вывести статистику:
    Суммарное количество отстрелянных патронов и заряженных магазинов.
    Суммарное время потраченное на зарядку патронов в магазин.
    Суммарное время стрельбы.
    Количество каждого типа патронов.
*/

#include <iostream>
#include <stack>
#include <iomanip>
#include <Windows.h>
#include <vector>

using namespace std;

class Patron {
public:
    int patron_type; //Тип патрона(0,1,2)
    string patron_type_name; //Тип патрона(название)

    //При создании объекта, рандомно задаем тип патрона
    Patron() {
        patron_type = rand() % 3; // 0 - Дозвуковые, 1 - Обычные, 2 - Трассеры
        if (patron_type == 0) {
            patron_type_name = "Дозвуковые";
        }
        else if (patron_type == 1) {
            patron_type_name = "Обычные";
        }
        else {
            patron_type_name = "Трассеры";
        }
        
    }

    //Функция вывода информации о патронах
    void printType() {
        cout << "Тип патрона: " << patron_type_name << endl;
        if (patron_type == 0) {
            cout << "\nЭффект патрона: " << "дозвуковой" << endl;
            cout << "Звук выстрела: " << "тихий\n" << endl;
        }
        else if (patron_type == 1) {
            cout << "\nЭффект патрона: " << "нет" << endl;
            cout << "Звук выстрела: " << "обычный\n" << endl;
        }
        else {
            cout << "\nЭффект патрона: " << "трассер" << endl;
            cout << "Звук выстрела: " << "обычный\n" << endl;
        }
    }
};

class Magazine {
public:
    stack <Patron> ammo; //Патроны в магазине
    int magazine_capacity = 30; //Емкость магазина
    double time_for_getting_patron = 1.5; //Воемя на доставку патрона
    
    //Функция перезарядки
    double reloading(vector <Patron> patron) {
        /*
        *@param patron - вектор патронов(кол-во патронов которое мы будем заряжать)
        *@param time - время, которое ушло на перезарядку 
        *return - время, которое ушло на перезарядку 
        */
        double time = 0.0;

        //Если патронов нет
        if (ammo.empty()) {
            cout << "Перезарядка" << endl;

            //Проходимся по вектору патронов
            for (int i = 0; i < patron.size(); i++) {

                //И добавляем каждый патрон
                ammo.push(patron[i]);
                time += 1.5; //Время добавления патрона в магазин
            }
        }
        return time;
    }

    //Функция передачи патрона
    Patron getPatron() {
        /*
        *@param patron - вытащенный патрон из стека
        *return - передаем этот патрон в оружие
        */
        Patron patron = ammo.top();

        //Удаляем его из стека
        ammo.pop();
        return patron;
    }

    //Функция определения размера магазина
    int sizeofMagazine() {
        /*
        *return - емкость магазина
        */
        return magazine_capacity;
    }
};

class Weapon {
public:
    Magazine magazine;

    //Функция стрельбы
    double shooting() {
        /*
        *@param time - время, которое ушло на стрельбу
        *return - время, которое ушло на стрельбу
        */
        double time = 0.0;
        cout << "---Стрельба---" << endl;

        //Пока есть патроны в магазине, будем стрелять
        while (!magazine.ammo.empty()) {
            time += magazine.time_for_getting_patron; //Время на передачу патрона
            Patron patron = magazine.getPatron(); //Достаем патрон
            time += 2.0; //Время на стрельбу
            patron.printType(); //Выводим информацию о патроне
        }
        cout << "--------------\n" << endl;

        return time;
    }
};


void simulation(Weapon weapon, int count); //Функция симуляции стрельб
void printStatistic(double* stats); //Функция вывода статистики

int main()
{

    srand(time(0)); //Инициализация рандомайзера
    SetConsoleCP(1251); //Поддержка ввода на русском языке
    setlocale(LC_ALL, "Russian"); //Установка русской локали
    Weapon weapon;
    int count = 310;
    simulation(weapon, count);

}

void simulation(Weapon weapon, int count) {
    /*
    *@param weapon - оружие, которым стреляем
    *@param count - количество патронов на складе
    *@param ammobox - склад патронов
    *@param releazed_ammo - количество отстреленных патронов
    *@param time_for_shooting - время, которое потратилось на стрельбу
    *@param time_for_reloading - время, которое потратилось на перезарядку
    *@param count_of_magazines - количество перезаряженных магазинов
    *@param count_of_types_of_ammo - счетчик-массив количества типов патронов
    *@param statistic - массив со всей статистикой симуляции
    */
    Patron* ammobox = new Patron[count];
    int releazed_ammo = 0;
    int ammo_to_reload = 0;
    double time_for_shooting = 0.0;
    double time_for_reloading = 0.0;
    int count_of_magazines = 0;
    int count_of_types_of_ammo[3]{ 0,0,0 };
    double statistic[7]{};

    //Пока остались патроны на складе
    while (releazed_ammo <= count) {

        //Если в магазине есть патроны, то стреляем
        if (!weapon.magazine.ammo.empty()) {
            cout << "Магазин вставлен\n" << endl;
            time_for_shooting += weapon.shooting();
        }
        else {

            //Если закончились патроны в коробке, то перезаряжать больше нечем и мы выходим из цикла.
            if (releazed_ammo >= count) {
                break;
            }

            cout << "Требуется перезарядка\n" << endl;

            //Хранилище патронов, которое мы засунем в магазин
            vector <Patron> patronsToMagazine;

            //Если патронов осталось больше, чем помещается в магазин, будем помещать их столько, сколько помещается в магазин
            if (count - releazed_ammo >= weapon.magazine.sizeofMagazine()) {
                ammo_to_reload = weapon.magazine.sizeofMagazine();
            }

            //Иначе будем помещать столько сколько осталось
            else {
                ammo_to_reload = count - releazed_ammo;
            }

            //Проходим по всем патронам
            for (int i = 0; i < ammo_to_reload; i++) {

                //Если дозвуковой патрон
                if (ammobox[releazed_ammo].patron_type == 0) {
                    count_of_types_of_ammo[0] += 1;
                }
                //Если обычный патрон
                else if (ammobox[releazed_ammo].patron_type == 1) {
                    count_of_types_of_ammo[1] += 1;
                }
                //Если трассер
                else {
                    count_of_types_of_ammo[2] += 1;
                }

                //Добавляем их в хранилище патронов для перезарядки
                patronsToMagazine.push_back(ammobox[releazed_ammo]);

                //Патрон был использован
                releazed_ammo++;

            }

            //Перезаряжаем оружие
            time_for_reloading += weapon.magazine.reloading(patronsToMagazine);

            count_of_magazines++; //+1 заряженный магазин

            //Очищаем хранилище для следующей перезарядки
            patronsToMagazine.clear();
        }
    }
    statistic[0] = releazed_ammo;
    statistic[1] = count_of_magazines;
    statistic[2] = time_for_reloading;
    statistic[3] = time_for_shooting;
    statistic[4] = count_of_types_of_ammo[0];
    statistic[5] = count_of_types_of_ammo[1];
    statistic[6] = count_of_types_of_ammo[2];
    printStatistic(statistic);

}

//Функция вывода статистики
void printStatistic(double* stats) {
    /*
    *@param stats - массив со статистикой
    */
    cout << "-------------------------Статистика--------------------------\n" << endl;
    cout << " Количество использованных патронов - " << stats[0] << endl;
    cout << " Количество заряженных магазинов - " << stats[1] << endl;
    cout << " Время потраченное на зарядку патронов в магазин - " << stats[2] << " секунд" << endl;
    cout << " Время стрельбы - " << stats[3] << " секунд" << endl;
    cout << " Количество каждого типа патронов: " <<
        "\n     Дозвуковые - " << stats[4] <<
        "\n     Обычные - " << stats[5] <<
        "\n     Трассеры - " << stats[6] << endl;
    cout << "\n-------------------------------------------------------------" << endl;
}
