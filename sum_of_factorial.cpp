#include <iostream>
#include <chrono>
#include <iomanip>

using namespace std;
using namespace std::chrono;

long long iterative_sum_factorials(long long n) {
    long long result = 1, sum = 0;
    long long k = n;
    while (k > 0) {
        for (int i = 1; i <= k; i++) {
            result = result * i;
        }
        k--;
        sum += result;
        result = 1;
    }
    return sum;
}

long long factorial(long long n) {
    if (n == 0 || n == 1) {
        return 1;
    } else {
        return n * factorial(n - 1);
    }
}

long long recursive_sum_factorials(long long n) {
    if (n == 0) {
        return 0;
    } else {
        return factorial(n) + recursive_sum_factorials(n - 1);
    }
}

int main() {
    setlocale(LC_ALL, "ru");
    long long n;
    
    cin >> n;

    auto start_iterative = high_resolution_clock::now();
    long long iterative_result = iterative_sum_factorials(n);
    auto end_iterative = high_resolution_clock::now();

    cout << "Итеративный результат: " << iterative_result << endl;

    auto duration_iterative_nanoseconds = duration_cast<nanoseconds>(end_iterative - start_iterative).count();
    cout << "Время выполнения итеративного алгоритма: " << duration_iterative_nanoseconds << " наносекунд" << endl;

    auto start_recursive = high_resolution_clock::now();
    long long recursive_result = recursive_sum_factorials(n);
    auto end_recursive = high_resolution_clock::now();

    cout << "Рекурсивный результат: " << recursive_result << endl;

    auto duration_recursive_nanoseconds = duration_cast<nanoseconds>(end_recursive - start_recursive).count();
    cout << "Время выполнения рекурсивного алгоритма: " << duration_recursive_nanoseconds << " наносекунд" << endl;

    return 0;
}