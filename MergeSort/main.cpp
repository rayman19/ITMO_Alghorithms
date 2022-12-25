#include <iostream>

using namespace std;

template<typename T>
void Merge(T* a, int f, int m, int l) {
    T* temp = new T[l];
    int i = f;
    int j = m + 1;
    for (int k = f; k <= l; k++) {
        if ((j > l) || ((a[i] < a[j]) && (i <= m))) {
            temp[k] = a[i];
            i++;
        }
        else {
            temp[k] = a[j];
            j++;
        }
    }

    for (int k = f; k <= l; k++) {
        a[k] = temp[k];
    }
}

template<typename T>
void SortMerge(T* a, int f, int l) {
    if (f >= l) {
        return;
    }

    int m = (l + f) / 2;
    SortMerge(a, f, m);
    SortMerge(a, m + 1, l);
    Merge(a, f, m, l);
}

int main()
{
    int N = 1000;
    int* a = new int[N];
    for (int i = 0; i < N; i++) {
        a[i] = rand() % 11;
    }

    for (int i = 0; i < N; i++) {
        cout << a[i] << ' ';
    }

    SortMerge<int>(a, 0, N - 1);

    cout << endl;

    for (int i = 0; i < N; i++) {
        cout << a[i] << ' ';
    }

    return 0;
}