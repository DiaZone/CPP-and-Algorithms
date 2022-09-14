#include <iostream>
#define N 1000 ///Количество символов в строке

using namespace std;

string startLineGeneration();   ///Функция заполнения строки рандомными символами
void lineSelection(string line);    ///Функция подбора строки

int main()
{
    string main_line = startLineGeneration();
    lineSelection(main_line);
}

string startLineGeneration() {
    string line = "";
    srand(time(0));

    ///Заполняем строку рандомным символом от A-Z

    for (int i = 0; i < N; i++) {
        line += char(rand() % 26 + 0x61);
    }
    cout << "Start line:\n" << line << endl;
    return line;
 }

void lineSelection(string line) {
    /*
    * @param line - входная строка с рандомными символами
    * @param duplicate_line - строка, которая получится после подбора
    * @param iter - счетчик итераций
    * @param max - максимальное количество попыток найти символ
    * @param count - буферный счетчик поиска максимального количество символа
    */
    string duplicate_line = "";
    int iter = 0;
    int max = 0;
    int count = 0;

    ///Начинаем посимвольный перебор строки

    for (int i = 0; i < line.length(); i++) {

        ///Проходимся по алфавиту

        for (char sym = 'a'; sym <= 'z'; sym++) {
            count++;
            iter++;

            ///Если символ находится правильно, то мы его записываем в строку и запоминаем сколько попыток потребовалось

            if (sym == line[i]) {
                duplicate_line += sym;
                if (max < count) {
                    max = count;
                }
                count = 0;
                break;
            }
        }
    }
    cout << "\nDuplicate line:\n" << duplicate_line << endl;
    cout << "\nCount of iterations: " << iter << endl;
    cout << "\nMaximum count of iteration of finding symbol: " << max << endl;
 }