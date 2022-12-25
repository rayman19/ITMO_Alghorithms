#include <iostream>
#include "Array.h"
#include <gtest/gtest.h>


int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();

    std::string strings[3] = { "Hello ", "world", " !" };
    Array<std::string> a(10);
    
    for (int i = 0; i < 3; i++) {
        a.insert(strings[i]);
    }

    Array<std::string> b;
    b = a;

    std::cout << 3 << ' ' << a.size() << std::endl;
    std::cout << 3 << ' ' << b.size() << std::endl;
    std::cout << 10 << ' ' << b.capacity() << std::endl;

    for (int i = 0; i < a.size(); i++) {
        std::cout << strings[i] << ' ' << a[i] << std::endl;
    }

    for (int i = 0; i < b.size(); i++) {
        std::cout << strings[i] << ' ' << b[i] << std::endl;
    }

    return 0;

    int N = 10;
    Array<int> arr(N);

    for (int i = 0; i < N; i++) {
        arr.insert(i + 1);
    }

    for (int i = 0; i < arr.size(); i++) {
        arr[i] *= 2;
    }

    for (int i = 0; i < arr.size(); i++) {
        std::cout << arr[i] << ' ';
    }
    std::cout << std::endl;

    Array<int> arr2 = arr;
    Array<int> arr3 = arr2;

    std::cout << arr.capacity();
    arr.insert(100);
    arr.insert(200);
    arr.insert(300);
    std::cout << '\n' << arr.capacity() << '\n';

    arr3 = arr;

    for (int i = 0; i < arr.size(); i++) {
        std::cout << arr[i] << ' ';
    }

    std::cout << std::endl;
    
    for (auto it = arr.iterator(); it.hasNext(); it.next())
        std::cout << it.get() << ' ';
       
    std::cout << std::endl;

    for (auto it = arr.reverseIterator(); it.hasNext(); it.next())
        std::cout << it.get() << ' ';

    std::cout << std::endl;

    for (auto it = arr.iterator(); it.hasNext(); it.next())
        std::cout << it.get() << ' ';

    std::cout << std::endl;

    for (auto it = arr2.iterator(); it.hasNext(); it.next())
        std::cout << it.get() << ' ';

    std::cout << std::endl;

    for (auto it = arr3.iterator(); it.hasNext(); it.next())
        std::cout << it.get() << ' ';

    std::cout << std::endl;

    std::cout << arr.capacity() << ' ' << arr.size() << '\n';
    std::cout << arr2.capacity() << ' ' << arr2.size() << '\n';
    std::cout << arr3.capacity() << ' ' << arr3.size() << '\n';

    Array<int> arr4;
    Array<int> arr5 = std::move(arr2);
    arr4 = std::move(arr5);

    return 0;
}