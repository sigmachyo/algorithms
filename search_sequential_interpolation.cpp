#include <iostream>
#include <vector>
#include <fstream>
#include <chrono>

using namespace std;
using namespace chrono;

void quickSort(vector<int>& arr, int low, int high) {
    if (low < high) {
        int pivot = arr[high];
        int i = low - 1;

        for (int j = low; j <= high - 1; j++) {
            if (arr[j] <= pivot) {
                i++;
                swap(arr[i], arr[j]);
            }
        }
        swap(arr[i + 1], arr[high]);
        int pivotIndex = i + 1;

        quickSort(arr, low, pivotIndex - 1);
        quickSort(arr, pivotIndex + 1, high);
    }
}

int sequentialSearch(const vector<int>& arr, int target) {
    for (int i = 0; i < arr.size(); i++) {
        if (arr[i] == target) {
            return i;
        }
    }
    return -1;
}

int interpolationSearch(const vector<int>& arr, int x) {
    int low = 0, high = arr.size() - 1;

    while (low <= high && x >= arr[low] && x <= arr[high]) {
        int pos = low + ((x - arr[low]) * (high - low) / (arr[high] - arr[low]));

        if (arr[pos] == x) {
            return pos;
        }
        if (arr[pos] < x) {
            low = pos + 1;
        } else {
            high = pos - 1;
        }
    }
    return -1;
}

vector<int> readArrayFromFile(const string& filename) {
    ifstream file(filename);
    vector<int> arr;

    if (!file.is_open()) {
        cerr << "Ошибка: Не удалось открыть файл " << filename << endl;
        return arr;
    }

    int n;
    file >> n;
    arr.resize(n);
    for (int i = 0; i < n; i++) {
        file >> arr[i];
    }

    file.close();
    return arr;
}

int main() {
    setlocale(LC_ALL, "ru");

    string filename;
    cout << "Введите имя файла с данными: ";
    cin >> filename;

    vector<int> arr = readArrayFromFile(filename);
    if (arr.empty()) {
        return 1;
    }

    int target;
    cout << "Введите элемент для поиска: ";
    cin >> target;

    vector<int> arrForSeq = arr;
    vector<int> arrForInterp = arr;

    quickSort(arrForInterp, 0, arrForInterp.size() - 1);

    auto start = high_resolution_clock::now();
    int seqResult = sequentialSearch(arrForSeq, target);
    auto stop = high_resolution_clock::now();
    auto durationSeq = duration_cast<nanoseconds>(stop - start);
    cout << "Последовательный поиск: ";
    if (seqResult != -1) {
        cout << "Элемент найден на позиции " << seqResult;
    } else {
        cout << "Элемент не найден";
    }
    cout << "; Время выполнения: " << durationSeq.count() << " наносекунд" << endl;

    start = high_resolution_clock::now();
    int interpResult = interpolationSearch(arrForInterp, target);
    stop = high_resolution_clock::now();
    auto durationInterp = duration_cast<nanoseconds>(stop - start);
    cout << "Интерполяционный поиск: ";
    if (interpResult != -1) {
        cout << "Элемент найден на позиции " << interpResult;
    } else {
        cout << "Элемент не найден";
    }
    cout << "; Время выполнения: " << durationInterp.count() << " наносекунд" << endl;

    return 0;
}