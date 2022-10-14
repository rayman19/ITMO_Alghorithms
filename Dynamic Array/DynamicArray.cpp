#include <iostream>

template<typename T>
class Array final {
public:
    Array() {
        data = new T[8];
        cap = 8;
        sizer = 0;
    }

    Array(int capacity) {
        data = new T[capacity];
        cap = capacity;
        sizer = 0;
    }

    ~Array() {
        delete [] data;
    }
    
    int insert(const T & value) {
        data[sizer] = value;
        sizer++;
        return sizer - 1;
    }

    int insert(int index, const T & value) {
        if (index < 0) return -1;

        if (index >= sizer) {
            data[sizer] = value;
        }
        else {
            for (int i = sizer; i > index; i--) {
                data[i] = data[i - 1];
            }
            data[index] = value;

        }
        
        sizer++;
        return index;
    }

    void remove(int index) {
        if (index + 1 > sizer || index < 0) return;

        if (index + 1 == sizer) {
            data[index] = 0;
        }
        else {
            for (int i = index; i < sizer - 1; i++) {
                data[i] = data[i + 1];
            }
            data[sizer - 1] = 0;

        }
        sizer--;
    }

    void expand() {
        cap *= 2;
        T* tempData = new T[cap];

        for (int i = 0; i < sizer; i++) {
            tempData[i] = data[i];
        }

        delete [] data;
        data = tempData;
    }

    int size() { return sizer; }

    T & operator[](int index) {
        return data[index];
    }

private:
    int cap;
    int sizer;
    T * data;
};

int main()
{
    Array<int> arr(20);

    for (int i = 0; i < 10; i++) {
        arr.insert(i + 1);
    }

    for (int i = 0; i < arr.size(); i++) {
        arr[i] *= 2;
    }

    for (int i = 0; i < arr.size(); i++) {
        std::cout << arr[i] << ' ';
    }
    std::cout << std::endl;



    for (int i = 0; i < arr.size(); i++) {
        std::cout << arr[i] << ' ';
    }
}