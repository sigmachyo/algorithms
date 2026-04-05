#include <iostream>
#include <fstream>
#include <vector>
#include <set>
#include <map>
#include <chrono>
#include <locale>
#include <clocale>
#include <algorithm>

using namespace std;

struct TreeNode {
    int value;
    TreeNode* left;
    TreeNode* right;
    TreeNode(int val) : value(val), left(nullptr), right(nullptr) {}
};

class BinaryTree {
public:
    BinaryTree() : root(nullptr) {}

    void insert(int value) { root = insertRec(root, value); }
    void removeLessThan(int threshold) { root = removeLessThanRec(root, threshold); }
    void printInOrder() { inOrderRec(root); cout << endl; }

private:
    TreeNode* root;

    TreeNode* insertRec(TreeNode* node, int value) {
        if (!node) return new TreeNode(value);
        if (value < node->value)
            node->left = insertRec(node->left, value);
        else
            node->right = insertRec(node->right, value);
        return node;
    }

    TreeNode* removeLessThanRec(TreeNode* node, int threshold) {
        if (!node) return nullptr;

        node->left = removeLessThanRec(node->left, threshold);
        node->right = removeLessThanRec(node->right, threshold);

        if (node->value < threshold) {
            TreeNode* temp = node->right ? node->right : node->left;
            delete node;
            return temp;
        }
        return node;
    }

    void inOrderRec(TreeNode* node) {
        if (node) {
            inOrderRec(node->left);
            cout << node->value << " ";
            inOrderRec(node->right);
        }
    }
};

class RedBlackTree {
public:
    void insert(int value) { rbt.insert(value); }
    void removeLessThan(int threshold) {
        for (auto it = rbt.begin(); it != rbt.end();) {
            if (*it < threshold) it = rbt.erase(it);
            else ++it;
        }
    }
    void printInOrder() {
        for (int val : rbt) cout << val << " ";
        cout << endl;
    }

private:
    set<int> rbt;
};

class AVLTree {
public:
    void insert(int value) { avl[value] = value; }
    void removeLessThan(int threshold) {
        for (auto it = avl.begin(); it != avl.end();) {
            if (it->first < threshold) it = avl.erase(it);
            else ++it;
        }
    }
    void printInOrder() {
        for (const auto& pair : avl) cout << pair.first << " ";
        cout << endl;
    }

private:
    map<int, int> avl;
};

vector<int> readDataFromFile(const string& filename) {
    vector<int> data;
    ifstream file(filename);
    int number;
    while (file >> number) data.push_back(number);
    return data;
}

int findMax(const vector<int>& data) {
    return *max_element(data.begin(), data.end());
}

int main() {
    setlocale(LC_ALL, "Russian");

    string filename;
    cout << "Введите имя файла с данными: ";
    cin >> filename;

    vector<int> data = readDataFromFile(filename);
    if (data.empty()) {
        cout << "Файл пуст или не может быть открыт." << endl;
        return 1;
    }

    int maxValue = findMax(data);
    int threshold = maxValue / 2;

    BinaryTree binaryTree;
    RedBlackTree rbTree;
    AVLTree avlTree;

    for (int value : data) {
        binaryTree.insert(value);
        rbTree.insert(value);
        avlTree.insert(value);
    }

    cout << "\nБинарное дерево (обход в порядке возрастания): ";
    binaryTree.printInOrder();

    cout << "\nКрасно-черное дерево (обход в порядке возрастания): ";
    rbTree.printInOrder();

    cout << "\nAVL-дерево (обход в порядке возрастания): ";
    avlTree.printInOrder();

    auto start = chrono::high_resolution_clock::now();
    binaryTree.removeLessThan(threshold);
    auto end = chrono::high_resolution_clock::now();
    auto durationBinary = chrono::duration_cast<chrono::microseconds>(end - start).count();

    start = chrono::high_resolution_clock::now();
    rbTree.removeLessThan(threshold);
    end = chrono::high_resolution_clock::now();
    auto durationRB = chrono::duration_cast<chrono::microseconds>(end - start).count();

    start = chrono::high_resolution_clock::now();
    avlTree.removeLessThan(threshold);
    end = chrono::high_resolution_clock::now();
    auto durationAVL = chrono::duration_cast<chrono::microseconds>(end - start).count();

    cout << "\nПосле удаления узлов меньше половины максимального значения (" << threshold << "):" << endl;

    cout << "\nБинарное дерево (обход в порядке возрастания): ";
    binaryTree.printInOrder();
    cout << "Время, затраченное на удаление узлов из бинарного дерева: " << durationBinary << " микросекунд" << endl;

    cout << "\nКрасно-черное дерево (обход в порядке возрастания): ";
    rbTree.printInOrder();
    cout << "Время, затраченное на удаление узлов из красно-черного дерева: " << durationRB << " микросекунд" << endl;

    cout << "\nAVL-дерево (обход в порядке возрастания): ";
    avlTree.printInOrder();
    cout << "Время, затраченное на удаление узлов из AVL-дерева: " << durationAVL << " микросекунд" << endl;

    return 0;
}