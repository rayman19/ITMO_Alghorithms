#include <iostream>
#include <chrono>
#include <fstream>
#include <gtest/gtest.h>
#include "QuickSort.h"


int main(int argc, char** argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();

    std::ofstream data;
    data.open("qsort_data.txt");
    //data.open("isort_data.txt");
    //data.open("iqsort_data.txt");

    for (int k = 2; k <= 100; k++) {
        long sum = 0;
        for (int j = 0; j <= 100; j++) {
            // инициализация данных
            int N = k;
            int* arr = new int[N];

            for (int i = 0; i < N; i++) {
                arr[i] = N - i;
                //std::cout << arr[i] << ' ';
            }
            //std::cout << std::endl;

            // старт
            auto start_time = std::chrono::steady_clock::now();

            //сортировка
            //insertSort(arr, N, [](int left, int right) { return left < right; });
            //sort(arr, arr + N - 1, [](int left, int right) { return left < right; });
            sortWithoutInsert(arr, arr + N - 1, [](int left, int right) { return left < right; });

            // финиш
            auto end_time = std::chrono::steady_clock::now();
            auto elapsed_ns = std::chrono::duration_cast<std::chrono::nanoseconds>(end_time - start_time);
            //std::cout << elapsed_ns.count() << " ns\n";

            sum += elapsed_ns.count();

            // сортированный массив
            /*for (int i = 0; i < N; i++) {
                std::cout << arr[i] << ' ';
            }
            std::cout << std::endl;*/

            delete[] arr;
        }
        data << k << ' ' << sum / 10 << std::endl;
    }

    data.close();
}