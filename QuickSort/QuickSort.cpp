#include <iostream>


template<typename T>
void swap(T &a, T &b) {
    T temp = a;
    a = b;
    b = temp;
}

template<typename T>
bool comp(const T& a, const T& b) {
    if (a < b) {
        return true;
    }
    return false;
}

template<typename T>
void sort(T *first, T *last) {
    if (first >= last) {
        return;
    }
    
    T *left = first;
    T *right = last;
    T *mid = (last + first) / 2;

    while (left < right) {
        while (*left < *mid) {
            left++;
        }
        while (*mid > *right) {
            right--;
        }
        swap(*left, *right);
    }

    sort(first, mid - 1);
    sort(mid + 1, last);
}

int main()
{
    const int N = 10;
    int arr[N];
    
    for (int i = 0; i < N; i++) {
        arr[i] = i;
        std::cout << arr[i] << ' ';
    }
    std::cout << std::endl;

    sort(arr, arr + N - 1);

    for (int i = 0; i < N; i++) {
        std::cout << arr[i] << ' ';
    }
}