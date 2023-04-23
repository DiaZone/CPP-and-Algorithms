#include "sha256.h"
#include <random>
#include <math.h>
#include <vector>
#include <fstream>
#include <chrono>
#include <algorithm>
#include <iostream>
using namespace std;

//Первые 32 бита дробных частей квадратных корней первых восьми простых чисел[от 2 до 19]:
void sha256_init(struct sha256_buff* buff) {
    buff->h[0] = 0x6a09e667;
    buff->h[1] = 0xbb67ae85;
    buff->h[2] = 0x3c6ef372;
    buff->h[3] = 0xa54ff53a;
    buff->h[4] = 0x510e527f;
    buff->h[5] = 0x9b05688c;
    buff->h[6] = 0x1f83d9ab;
    buff->h[7] = 0x5be0cd19;
    buff->data_size = 0;
    buff->chunk_size = 0;
}

//Первые 32 бита дробных частей кубических корней первых 64 простых чисел [от 2 до 311]:
static const uint32_t k[64] = {
    0x428a2f98, 0x71374491, 0xb5c0fbcf, 0xe9b5dba5, 0x3956c25b, 0x59f111f1, 0x923f82a4, 0xab1c5ed5,
    0xd807aa98, 0x12835b01, 0x243185be, 0x550c7dc3, 0x72be5d74, 0x80deb1fe, 0x9bdc06a7, 0xc19bf174,
    0xe49b69c1, 0xefbe4786, 0x0fc19dc6, 0x240ca1cc, 0x2de92c6f, 0x4a7484aa, 0x5cb0a9dc, 0x76f988da,
    0x983e5152, 0xa831c66d, 0xb00327c8, 0xbf597fc7, 0xc6e00bf3, 0xd5a79147, 0x06ca6351, 0x14292967,
    0x27b70a85, 0x2e1b2138, 0x4d2c6dfc, 0x53380d13, 0x650a7354, 0x766a0abb, 0x81c2c92e, 0x92722c85,
    0xa2bfe8a1, 0xa81a664b, 0xc24b8b70, 0xc76c51a3, 0xd192e819, 0xd6990624, 0xf40e3585, 0x106aa070,
    0x19a4c116, 0x1e376c08, 0x2748774c, 0x34b0bcb5, 0x391c0cb3, 0x4ed8aa4a, 0x5b9cca4f, 0x682e6ff3,
    0x748f82ee, 0x78a5636f, 0x84c87814, 0x8cc70208, 0x90befffa, 0xa4506ceb, 0xbef9a3f7, 0xc67178f2
};

#define rotate_r(val, bits) (val >> bits | val << (32 - bits))

//Сообщение обрабатывается последовательными порциями по 512 бит:
static void sha256_calc_chunk(struct sha256_buff* buff, const uint8_t* chunk) {
    uint32_t w[64];
    uint32_t tv[8];
    uint32_t i;

    //Разбить кусок на 16 слов длиной 32 бита:
    for (i = 0; i < 16; ++i) {
        w[i] = (uint32_t)chunk[0] << 24 | (uint32_t)chunk[1] << 16 | (uint32_t)chunk[2] << 8 | (uint32_t)chunk[3];
        chunk += 4;
    }

    //Сгенерировать дополнительные 48 слов:
    for (i = 16; i < 64; ++i) {
        uint32_t s0 = rotate_r(w[i - 15], 7) ^ rotate_r(w[i - 15], 18) ^ (w[i - 15] >> 3);
        uint32_t s1 = rotate_r(w[i - 2], 17) ^ rotate_r(w[i - 2], 19) ^ (w[i - 2] >> 10);
        w[i] = w[i - 16] + s0 + w[i - 7] + s1;
    }
    //Инициализация вспомогательных переменных:
    for (i = 0; i < 8; ++i)
        tv[i] = buff->h[i];

    //Основной цикл:
    for (i = 0; i < 64; ++i) {
        uint32_t S1 = rotate_r(tv[4], 6) ^ rotate_r(tv[4], 11) ^ rotate_r(tv[4], 25);
        uint32_t ch = (tv[4] & tv[5]) ^ (~tv[4] & tv[6]);
        uint32_t temp1 = tv[7] + S1 + ch + k[i] + w[i];
        uint32_t S0 = rotate_r(tv[0], 2) ^ rotate_r(tv[0], 13) ^ rotate_r(tv[0], 22);
        uint32_t maj = (tv[0] & tv[1]) ^ (tv[0] & tv[2]) ^ (tv[1] & tv[2]);
        uint32_t temp2 = S0 + maj;

        tv[7] = tv[6];
        tv[6] = tv[5];
        tv[5] = tv[4];
        tv[4] = tv[3] + temp1;
        tv[3] = tv[2];
        tv[2] = tv[1];
        tv[1] = tv[0];
        tv[0] = temp1 + temp2;
    }
    
    //Добавить полученные значения к ранее вычисленному результату:
    for (i = 0; i < 8; ++i)
        buff->h[i] += tv[i];
}

//Обрабатывающий блок данных произвольной длины
void sha256_update(struct sha256_buff* buff, const void* data, size_t size) {
    const uint8_t* ptr = (const uint8_t*)data;
    buff->data_size += size;
    /* Если в буфере остались данные, объедините их для обработки как новый фрагмент */
    if (size + buff->chunk_size >= 64) {
        uint8_t tmp_chunk[64];
        memcpy(tmp_chunk, buff->last_chunk, buff->chunk_size);
        memcpy(tmp_chunk + buff->chunk_size, ptr, 64 - buff->chunk_size);
        ptr += (64 - buff->chunk_size);
        size -= (64 - buff->chunk_size);
        buff->chunk_size = 0;
        sha256_calc_chunk(buff, tmp_chunk);
    }
    /* Проход по фрагментам */
    while (size >= 64) {
        sha256_calc_chunk(buff, ptr);
        ptr += 64;
        size -= 64;
    }

    /* Сохранение оставшихся данных в буфере */
    memcpy(buff->last_chunk + buff->chunk_size, ptr, size);
    buff->chunk_size += size;
}

//Функция получения конечных хеш-значений(дайджеста)
void sha256_finalize(struct sha256_buff* buff) {
    buff->last_chunk[buff->chunk_size] = 0x80;
    buff->chunk_size++;
    memset(buff->last_chunk + buff->chunk_size, 0, 64 - buff->chunk_size);

    /* Если не хватает места для int64, то фрагмент заполняется нулями */
    if (buff->chunk_size > 56) {
        sha256_calc_chunk(buff, buff->last_chunk);
        memset(buff->last_chunk, 0, 64);
    }

    uint64_t size = buff->data_size * 8;
    int i;
    for (i = 8; i > 0; --i) {
        buff->last_chunk[55 + i] = size & 255;
        size >>= 8;
    }

    sha256_calc_chunk(buff, buff->last_chunk);
}

//Считывание дайджеста в 32-байтовый двоичный массив 
void sha256_read(const struct sha256_buff* buff, uint8_t* hash) {
    uint32_t i;
    for (i = 0; i < 8; i++) {
        hash[i * 4] = (buff->h[i] >> 24) & 255;
        hash[i * 4 + 1] = (buff->h[i] >> 16) & 255;
        hash[i * 4 + 2] = (buff->h[i] >> 8) & 255;
        hash[i * 4 + 3] = buff->h[i] & 255;
    }
}

//Функция перевода из бинарного вида в 16-ричный
static void bin_to_hex(const void* data, uint32_t len, char* out) {
    static const char* const lut = "0123456789abcdef";
    uint32_t i;
    for (i = 0; i < len; ++i) {
        uint8_t c = ((const uint8_t*)data)[i];
        out[i * 2] = lut[c >> 4];
        out[i * 2 + 1] = lut[c & 15];
    }
}

//Считывает дайджест в 64-символьную строку как шестнадцатеричную (без нулевого байта)
void sha256_read_hex(const struct sha256_buff* buff, char* hex) {
    uint8_t hash[32];
    sha256_read(buff, hash);
    bin_to_hex(hash, 32, hex);
}

//Хэширует один непрерывный блок данных и считывает дайджест в 32-байтовый двоичный массив 
void sha256_easy_hash(const void* data, size_t size, uint8_t* hash) {
    struct sha256_buff buff;
    sha256_init(&buff);
    sha256_update(&buff, data, size);
    sha256_finalize(&buff);
    sha256_read(&buff, hash);
}

//Хэширует один непрерывный блок данных и считывает дайджест в 64-символьную строку (без нулевого байта
void sha256_easy_hash_hex(const void* data, size_t size, char* hex) {
    uint8_t hash[32];
    sha256_easy_hash(data, size, hash);
    bin_to_hex(hash, 32, hex);
}


//Генерация случайной строки
string generationString(int len) {
    string str = "";
    for (int i = 0; i < len; i++) {
        str += (char)(97 + rand() % 26);
    }
    return str;
}

//Замена нескольких элементов в строке
string changeString(string str, int count) {
    int* arr = new int[count];
    for (int i = 0; i < count; i++) {
        arr[i] = rand() % str.length();
        if (find(arr, arr + i, arr[i]) != arr + i) {
            i--;
        }
    }
    for (int i = 0; i < count; i++) {
        str[arr[i]] = (char)(((toascii(str[arr[i]]) - 'a' + rand() % 25) % 26) + 'a');
    }
    delete[] arr;
    return str;
}

int main() {
    srand(time(0));

    ofstream out;
    out.open("D:\\Games\\Laba9\\output9.txt");
    string str1, str2;
    string hash1, hash2;
    int str_len = 128; //Длина строки
    int N;
    for (int h = 0; h < 5; h++) {
        N = pow(2, h);
        for (int k = 0; k < 1000; k++) {
            cout << "\nk = " << k << " N = " << N << endl;
            str1 = generationString(str_len); //Генерируем строку
            str2 = str1;

            str2 = changeString(str2, N); //Смена нескольких символов

            hash1 = SHA256::hashString(str1);
            hash2 = SHA256::hashString(str2);

            int count = 0, maxcount = 0;

            //Поиск одинаковой последовательности символов
            for (int i = 0; i < hash1.length(); i++) {
                for (int j = 0; j < hash2.length(); j++) {
                    count = 0;
                    //Пока символы совпадают
                    while (hash1[i] == hash2[j] && i < hash1.length() && j < hash2.length()) {
                        count++;
                        i++;
                        j++;
                    }
                    if (count > maxcount) //Проверка на максимальную длину
                        maxcount = count;
                }
            }
            out << maxcount << endl;
        }
    }
   
    out << "\n------------------------\n";


    str_len = 256; //Длина строки
    string str;
    vector<string> vec_strs;
    int count = 0;

    for (int h = 2; h < 7; h++) {
        N = pow(10, h);
        for (int k = 0; k < N; k++) {
            cout << "\nk = " << k << " N = " << N << endl;
            str = generationString(str_len); //Генерируем строку
            hash1 = SHA256::hashString(str);
            vec_strs.push_back(hash1); //Добавляем хэш в вектор
        }
        sort(vec_strs.begin(), vec_strs.end()); //Сортируем по возрастанию вектор
        int count = 0;
        for (int i = 0; i < N - 1; i++) {
            if (vec_strs[i] == vec_strs[i + 1]) //Сравниваем соседние элементы
                count++; //Коллизия
        }
        out << count << endl;
    }

    out << "\n------------------------\n";

    int sizes[8] = { 64, 128, 256, 512, 1024, 2048, 4096, 8192 };
    chrono::high_resolution_clock::time_point begin;
    chrono::high_resolution_clock::time_point end;
    chrono::duration<double, milli> milli_diff;
    for (int c = 0; c < size(sizes); c++) {
        double all_time = 0;

        for (int i = 0; i < 1000; i++) {
            str = generationString(sizes[c]);
            cout << "\ni = " << i << endl;
            begin = chrono::high_resolution_clock::now(); //Стартовое время
            hash1 = SHA256::hashString(str);
            end = chrono::high_resolution_clock::now(); //Конечное время
            all_time += (end - begin).count();
        }
        out << sizes[c] << " " << all_time / 1000 << endl;
    }

    out << "\n------------------------\n";

    out.close();
    return 0;
}
