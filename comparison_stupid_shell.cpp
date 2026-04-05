#include <iostream>
#include <vector>
#include <fstream>
#include <chrono>
#include <clocale>

using namespace std;

void stupidSort(vector<int>& arr) {
    int n = arr.size();
    for (int i = 0; i < n - 1; ++i) {
        if (arr[i] > arr[i + 1]) {
            swap(arr[i], arr[i + 1]);
            i = -1;
        }
    }
}

void shellSort(vector<int>& arr) {
    int n = arr.size();
    for (int gap = n / 2; gap > 0; gap /= 2) {
        for (int i = gap; i < n; ++i) {
            int temp = arr[i];
            int j;
            for (j = i; j >= gap && arr[j - gap] > temp; j -= gap) {
                arr[j] = arr[j - gap];
            }
            arr[j] = temp;
        }
    }
}

void printArray(const vector<int>& arr) {
    for (int num : arr) {
        cout << num << " ";
    }
    cout << endl;
}

int main() {
    setlocale(LC_ALL, "ru_RU");

    string fileName = "unba5.txt";

    ifstream inputFile(fileName);
    if (!inputFile) {
        cerr << "Ошибка при открытии файла!" << endl;
        return 1;
    }

    int count;
    inputFile >> count;
    vector<int> arr(count);

    for (int i = 0; i < count; ++i) {
        inputFile >> arr[i];
    }
    inputFile.close();

    vector<int> arr1 = arr;
    vector<int> arr2 = arr;

    cout << "Исходный массив: ";
    printArray(arr);

    auto startStupid = chrono::high_resolution_clock::now();
    stupidSort(arr1);
    auto endStupid = chrono::high_resolution_clock::now();
    chrono::duration<double, std::micro> durationStupid = endStupid - startStupid;

    cout << "Отсортированный массив с помощью глупой сортировки: ";
    printArray(arr1);
    cout << "Время, затраченное на глупую сортировку: " << durationStupid.count() << " мс" << endl;

    auto startShell = chrono::high_resolution_clock::now();
    shellSort(arr2);
    auto endShell = chrono::high_resolution_clock::now();
    chrono::duration<double, std::micro> durationShell = endShell - startShell;

    cout << "Отсортированный массив с помощью сортировки Шелла: ";
    printArray(arr2);
    cout << "Время, затраченное на сортировку Шелла: " << durationShell.count() << " мс" << endl;

    return 0;
}

