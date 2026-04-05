#include <iostream>
#include <vector>
#include <algorithm>
#include <fstream>
#include <chrono>
#include <clocale>
#include <string>
#include <sstream>

using namespace std;

const int S[64] = {
    7, 12, 17, 22, 7, 12, 17, 22, 7, 12, 17, 22, 7, 12, 17, 22,
    5, 9, 14, 20, 5, 9, 14, 20, 5, 9, 14, 20, 5, 9, 14, 20,
    4, 11, 16, 23, 4, 11, 16, 23, 4, 11, 16, 23, 4, 11, 16, 23,
    6, 10, 15, 21, 6, 10, 15, 21, 6, 10, 15, 21, 6, 10, 15, 21
};

const uint32_t T[64] = {
    0xd76aa478, 0xe8c7b756, 0x242070db, 0xc1bdceee, 0xf57c0faf, 0x4787c62a,
    0xa8304613, 0xfd469501, 0x698098d8, 0x8b44f7af, 0x4f4bb3f1, 0x6e3f4dd9,
    0x1fa27cf8, 0xc4ac5665, 0x2f9a3e0f, 0x78b1f72d, 0x4e4baf3e, 0x9d49ab7b,
    0x1a49c79a, 0xa7c51f00, 0x9bdf06b1, 0x5538ad1f, 0xa1b52a9a, 0x49db7742,
    0xa1e63f72, 0x8be4a1ec, 0xa2015573, 0x849a6e8e, 0x6ee5a1ea, 0x95dbb8bf,
    0x8f481bf6, 0x42b3d7f2, 0x3816573e, 0x6f5c6579, 0xe08a6d1c, 0x44c7e1b6,
    0xf27c882d, 0x2f7b56b5, 0x7793c703, 0x1b7f3e72, 0x287d0732, 0x3bbd072b,
    0x4fe8a767, 0xd7b0d9c7, 0xd60df771, 0x2b6160bc, 0x5474958d, 0xa82604d9,
    0x5718d1f7, 0x49368d6b, 0xa26c3133, 0x245105fa, 0x9e59e2c0, 0x3527064d,
    0x38b0f672, 0x1aab0e5b, 0x0c89b48d, 0x8a1a498f, 0x8f319800, 0x785b5e9f
};

class HashTable {
private:
    vector<pair<int, int>> table;
    int capacity;
    int size;
    const int DELETED = -2;

    string md5(int key) {
        uint32_t A = 0x67452301;
        uint32_t B = 0xEFCDAB89;
        uint32_t C = 0x98BADCFE;
        uint32_t D = 0x10325476;

        stringstream ss;
        ss << key;
        string input = ss.str();

        input += char(0x80);
        while ((input.size() * 8) % 512 != 448) {
            input += char(0x00);
        }

        uint64_t length = input.size() * 8;
        for (int i = 0; i < 8; i++) {
            input += char((length >> (i * 8)) & 0xFF);
        }

        for (size_t i = 0; i < input.size(); i += 64) {
            uint32_t X[16];
            for (int j = 0; j < 16; j++) {
                X[j] = (input[i + j * 4 + 0] & 0xFF) |
                       (input[i + j * 4 + 1] << 8) |
                       (input[i + j * 4 + 2] << 16) |
                       (input[i + j * 4 + 3] << 24);
            }

            uint32_t A_temp = A, B_temp = B, C_temp = C, D_temp = D;

            for (int j = 0; j < 64; j++) {
                uint32_t F, g;
                if (j < 16) {
                    F = (B_temp & C_temp) | (~B_temp & D_temp);
                    g = j;
                } else if (j < 32) {
                    F = (D_temp & B_temp) | (~D_temp & C_temp);
                    g = (5 * j + 1) % 16;
                } else if (j < 48) {
                    F = B_temp ^ C_temp ^ D_temp;
                    g = (3 * j + 5) % 16;
                } else {
                    F = C_temp ^ (B_temp | ~D_temp);
                    g = (7 * j) % 16;
                }

                uint32_t temp = D_temp;
                D_temp = C_temp;
                C_temp = B_temp;
                B_temp = B_temp + ((A_temp + F + X[g] + T[j]) << S[j] | (A_temp + F + X[g] + T[j]) >> (32 - S[j]));
                A_temp = temp;
            }

            A += A_temp;
            B += B_temp;
            C += C_temp;
            D += D_temp;
        }

        string result;
        result += (A & 0xFF);
        result += ((A >> 8) & 0xFF);
        result += ((A >> 16) & 0xFF);
        result += ((A >> 24) & 0xFF);
        result += (B & 0xFF);
        result += ((B >> 8) & 0xFF);
        result += ((B >> 16) & 0xFF);
        result += ((B >> 24) & 0xFF);
        result += (C & 0xFF);
        result += ((C >> 8) & 0xFF);
        result += ((C >> 16) & 0xFF);
        result += ((C >> 24) & 0xFF);
        result += (D & 0xFF);
        result += ((D >> 8) & 0xFF);
        result += ((D >> 16) & 0xFF);
        result += ((D >> 24) & 0xFF);

        return result;
    }

    int hashFunction(int key) {
        string hash = md5(key);
        int index = 0;
        for (char c : hash) {
            index = (index + (unsigned char)c) % capacity;
        }
        return index;
    }

public:
    HashTable(int capacity) : capacity(capacity), size(0) {
        table.resize(capacity, make_pair(-1, DELETED));
    }

    void insert(int key, int value) {
        int index = hashFunction(key);
        while (table[index].first != -1 && table[index].first != DELETED) {
            index = (index + 1) % capacity;
        }
        table[index] = make_pair(key, value);
        size++;
    }

    bool search(int key, int& value) {
        int index = hashFunction(key);
        while (table[index].first != -1) {
            if (table[index].first == key) {
                value = table[index].second;
                return true;
            }
            index = (index + 1) % capacity;
        }
        return false;
    }

    void remove(int key) {
        int index = hashFunction(key);
        while (table[index].first != -1) {
            if (table[index].first == key) {
                table[index].first = -1;
                table[index].second = DELETED;
                size--;
                return;
            }
            index = (index + 1) % capacity;
        }
    }

    void display() {
        vector<pair<int, int>> validEntries;

        for (int i = 0; i < capacity; i++) {
            if (table[i].first != -1 && table[i].first != DELETED) {
                validEntries.push_back(table[i]);
            }
        }

        sort(validEntries.begin(), validEntries.end(), [](const pair<int, int>& a, const pair<int, int>& b) {
            return a.first < b.first;
        });

        for (const auto& entry : validEntries) {
            cout << "Key: " << entry.first << ", Value: " << entry.second << endl;
        }
    }

    void insertFromInput() {
        int key, value;
        cout << "Введите ключ и значение: ";
        cin >> key >> value;
        insert(key, value);
    }
};

int main() {
    setlocale(LC_ALL, "ru_RU");

    string filename;
    cout << "Введите имя файла с данными: ";
    cin >> filename;

    ifstream infile(filename);
    if (!infile) {
        cerr << "Не удалось открыть файл!" << endl;
        return 1;
    }

    int size;
    infile >> size;

    HashTable ht(size);

    int value;
    cout << "Чтение данных из файла и вставка в таблицу..." << endl;

    auto start = chrono::high_resolution_clock::now();
    for (int i = 0; i < size; ++i) {
        infile >> value;
        ht.insert(i, value);
    }
    auto end = chrono::high_resolution_clock::now();
    infile.close();

    auto duration = chrono::duration_cast<chrono::microseconds>(end - start).count();
    cout << "Время вставки: " << duration << " микросекунд." << endl;

    cout << "\nВведите индекс для поиска: ";
    int key;
    cin >> key;
    if (ht.search(key, value)) {
        cout << "Ключ (индекс) найден: " << key << " => Значение: " << value << endl;
    } else {
        cout << "Ключ (индекс) не найден: " << key << endl;
    }

    cout << "\nВведите индекс для удаления: ";
    cin >> key;
    ht.remove(key);

    cout << "\nВведите данные для вставки нового элемента:" << endl;
    ht.insertFromInput();

    int show;
    cout << "\nВывести таблицу?(1/0)";
    cin >> show;

    if (show == 1) {
        ht.display();
    }

    return 0;
}