#include <iostream>
#include <vector>
#include <chrono>
#include <unordered_set>
#include <algorithm>

using namespace std;
using namespace std::chrono;

struct Node {
    int data;
    Node* prev;
    Node* next;
    Node(int value) : data(value), prev(nullptr), next(nullptr) {}
};

int findDifference(const vector<int>& arr) {
    if (arr.size() < 2) return 0;

    unordered_set<int> differences;

    for (int i = 1; i < arr.size(); ++i) {
        differences.insert(arr[i] - arr[i - 1]);
    }

    if (differences.size() > 1) {
        int minDiff = *min_element(differences.begin(), differences.end());
        int maxDiff = *max_element(differences.begin(), differences.end());
        return (minDiff + maxDiff) / 2;
    }

    return *differences.begin();
}

int findMissingElement(const vector<int>& arr) {
    int diff = findDifference(arr);

    for (int i = 1; i < arr.size(); ++i) {
        if (arr[i] != arr[i - 1] + diff) {
            return arr[i - 1] + diff;
        }
    }

    if (arr[0] != arr[1] - diff) {
        return arr[0] + diff;
    }

    return arr.back() + diff;
}

void restoreMissingElementInArray(vector<int>& arr, bool isDecreasing) {
    int missingElement = findMissingElement(arr);
    int diff = findDifference(arr);

    for (int i = 1; i < arr.size(); ++i) {
        if (arr[i - 1] + diff != arr[i]) {
            arr.insert(arr.begin() + i, missingElement);
            break;
        }
    }

    if (arr.back() + diff == missingElement) {
        arr.push_back(missingElement);
    }

    cout << "Восстановленная арифметическая прогрессия в массиве: ";
    if (isDecreasing) {
        for (int i = arr.size() - 1; i >= 0; --i) {
            cout << arr[i] << " ";
        }
    } else {
        for (int i = 0; i < arr.size(); ++i) {
            cout << arr[i] << " ";
        }
    }
    cout << endl;
}

void restoreMissingElementInList(Node* head, Node* tail, int missingElement, bool isDecreasing) {
    Node* temp = head;
    int diff = findDifference({head->data, head->next->data});

    while (temp != nullptr && temp->next != nullptr) {
        if (temp->data + diff != temp->next->data) {
            Node* newNode = new Node(missingElement);
            newNode->next = temp->next;
            newNode->prev = temp;
            temp->next->prev = newNode;
            temp->next = newNode;
            break;
        }
        temp = temp->next;
    }

    cout << "Восстановленная арифметическая прогрессия в двусвязном списке: ";
    if (isDecreasing) {
        temp = tail;
        while (temp != nullptr) {
            cout << temp->data << " ";
            temp = temp->prev;
        }
    } else {
        temp = head;
        while (temp != nullptr) {
            cout << temp->data << " ";
            temp = temp->next;
        }
    }
    cout << endl;
}

void inputArray(vector<int>& arr, bool& isDecreasing, int n) {
    cout << "Введите элементы арифметической прогрессии с пропуском: ";
    for (int i = 0; i < n - 1; ++i) {
        cin >> arr[i];
    }

    isDecreasing = arr[0] > arr[1];
    if (isDecreasing) {
        reverse(arr.begin(), arr.end());
    }
}

Node* createDoublyLinkedList(const vector<int>& arr) {
    Node* head = new Node(arr[0]);
    Node* tail = head;
    for (int i = 1; i < arr.size(); ++i) {
        Node* newNode = new Node(arr[i]);
        tail->next = newNode;
        newNode->prev = tail;
        tail = newNode;
    }
    return head;
}

void freeDoublyLinkedList(Node* head) {
    Node* temp;
    while (head != nullptr) {
        temp = head;
        head = head->next;
        delete temp;
    }
}

int main() {
    setlocale(LC_ALL, "ru");
    int n;
    cout << "Введите количество элементов арифметической прогрессии (включая пропущенный элемент): ";
    cin >> n;

    vector<int> arr(n - 1);
    bool isDecreasing;

    inputArray(arr, isDecreasing, n);

    auto start = high_resolution_clock::now();
    restoreMissingElementInArray(arr, isDecreasing);
    auto stop = high_resolution_clock::now();
    auto durationArray = duration_cast<milliseconds>(stop - start);
    cout << "Время выполнения восстановления массива: " << durationArray.count() << " милисекунд" << endl;

    Node* head = createDoublyLinkedList(arr);
    Node* tail = head;
    while (tail->next != nullptr) {
        tail = tail->next;
    }

    int missingElement = findMissingElement(arr);

    start = high_resolution_clock::now();
    restoreMissingElementInList(head, tail, missingElement, isDecreasing);
    stop = high_resolution_clock::now();
    auto durationList = duration_cast<milliseconds>(stop - start);
    cout << "Время выполнения восстановления двусвязного списка: " << durationList.count() << " милисекунд" << endl;

    freeDoublyLinkedList(head);

    return 0;
}