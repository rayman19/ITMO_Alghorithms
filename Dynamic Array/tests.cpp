#include <iostream>
#include "Array.h"
#include <gtest/gtest.h>


class ArrayTestsF : public testing::Test {
public:
    template<typename T>
    void TestConstructor() 
    {
        Array<T> a;
        EXPECT_EQ(0, a.size());
        EXPECT_EQ(8, a.capacity());
    }

    template<typename T>
    void TestConstructorCapacity() {
        int capacity = 10;
        Array<T> a(capacity);
        EXPECT_EQ(0, a.size());
        EXPECT_EQ(capacity, a.capacity());
    }

    template<typename T>
    void TestCopyConstructor_Empty() {
        int capacity = 10;
        Array<T> a(capacity);
        Array<T> b = a;

        EXPECT_EQ(0, a.size());
        EXPECT_EQ(0, b.size());
        EXPECT_EQ(capacity, b.capacity());
    }

    template<typename T>
    void TestCopyConstructor_App(T* items, int capacity, int size) {
        Array<T> a(capacity);
        
        for (int i = 0; i < size; i++) {
            a.insert(items[i]);
        }
        
        Array<T> b = a;

        EXPECT_EQ(size, a.size());
        EXPECT_EQ(size, b.size());
        EXPECT_EQ(capacity, b.capacity());

        for (int i = 0; i < a.size(); i++) {
            EXPECT_EQ(items[i], a[i]);
        }

        for (int i = 0; i < b.size(); i++) {
            EXPECT_EQ(items[i], b[i]);
        }
    }

    template<typename T>
    void TestMoveConstructor_Filled(T* items, int capacity, int size) {
        Array<T> a(capacity);
        
        for (int i = 0; i < size; i++) {
            a.insert(items[i]);
        }
        
        Array<T> b = std::move(a);

        EXPECT_EQ(size, b.size());
        EXPECT_EQ(capacity, b.capacity());

        for (int i = 0; i < b.size(); i++) {
            EXPECT_EQ(items[i], b[i]);
        }
    }

    template<typename T>
    void TestCopyAssigment_Filled(T* items, int capacity, int size) {
        Array<T> a(capacity);
        
        for (int i = 0; i < size; i++) {
            a.insert(items[i]);
        }

        Array<T> b;
        b = a;

        EXPECT_EQ(size, a.size());
        EXPECT_EQ(size, b.size());
        EXPECT_EQ(capacity, b.capacity());

        for (int i = 0; i < a.size(); i++) {
            EXPECT_EQ(items[i], a[i]);
        }

        for (int i = 0; i < b.size(); i++) {
            EXPECT_EQ(items[i], b[i]);
        }
    }

    template<typename T>
    void TestMoveAssigment_Filled(T* items, int capacity, int size) {
        Array<T> a(capacity);
        
        for (int i = 0; i < size; i++) {
            a.insert(items[i]);
        }

        Array<T> b;
        b = std::move(a);

        EXPECT_EQ(size, b.size());
        EXPECT_EQ(capacity, b.capacity());

        for (int i = 0; i < b.size(); i++) {
            EXPECT_EQ(items[i], b[i]);
        }
    }

    template<typename T>
    void TestInsert(T* items, int size, int index, T value) {
        Array<T> a(size);
        
        for (int i = 0; i < size; i++) {
            a.insert(items[i]);
        }
        
        EXPECT_EQ(size, a.size());
        EXPECT_EQ(size, a.capacity());

        for (int i = 0; i < a.size(); i++) {
            EXPECT_EQ(items[i], a[i]);
        }

        a.insert(index, value);

        for (int i = 0; i < index; i++) {
            EXPECT_EQ(items[i], a[i]);
        }

        EXPECT_EQ(value, a[index]);

        for (int i = index; i < a.size() - 1; i++) {
            EXPECT_EQ(items[i], a[i + 1]);
        }
    }

    template<typename T>
    void TestRemove(T* items, int size, int removeIndex) {
        Array<T> a(size);

        for (int i = 0; i < size; i++) {
            a.insert(items[i]);
        }

        a.remove(removeIndex);

        for (int i = 0; i < size - 1; i++) {
            if (i < removeIndex) {
                EXPECT_EQ(items[i], a[i]);
            }
            else if (i > removeIndex) {
                EXPECT_EQ(items[i], a[i - 1]);
            }
        }
    }

    template<typename T>
    void TestCapacityExpand(T* items, int size) {
        Array<T> a(size);
        
        for (int i = 0; i < size; i++) {
            a.insert(items[i]);
        }

        EXPECT_EQ(size, a.capacity());
        a.insert(items[0]);
        EXPECT_EQ(size * 2, a.capacity());

        for (int i = 1; i < size; i++) {
            a.insert(items[i]);
        }

        EXPECT_EQ(size * 2, a.capacity());
        a.insert(items[0]);
        EXPECT_EQ(size * 4, a.capacity());
    }

    template<typename T>
    void TestIterator(T* items, int size) {
        Array<T> a(size);
        
        for (int i = 0; i < size; i++) {
            a.insert(items[i]);
        }

        auto iter = a.iterator();

        for (int i = 0; i < size; i++) {
            EXPECT_EQ(iter.get(), a[i]);
            iter.next();
            EXPECT_EQ(iter.hasNext(), i < size - 1);
        }
    }

    template<typename T>
    void TestReverseIterator(T* items, int size) {
        Array<T> a(size);
        
        for (int i = 0; i < size; i++) {
            a.insert(items[i]);
        }

        auto iter = a.reverseIterator();

        for (int i = size - 1; i >= 0; --i) {
            EXPECT_EQ(iter.get(), a[i]);
            iter.next();
            EXPECT_EQ(iter.hasNext(), i > 0);
        }
    }


};

TEST_F(ArrayTestsF, DefaultConstructor) {
    TestConstructor<int>();
    TestConstructor<std::string>();
}

TEST_F(ArrayTestsF, TestConstructorCapacity) {
    TestConstructorCapacity<int>();
    TestConstructorCapacity<std::string>();
}

TEST_F(ArrayTestsF, TestCopyConstructor_Empty) {
    TestCopyConstructor_Empty<int>();
    TestCopyConstructor_Empty<std::string>();
}

TEST_F(ArrayTestsF, TestCopyConstructor_App) {
    int ints[3] = { 1, 2, 3 };
    TestCopyConstructor_App<int>(ints, 10, 3);

    std::string strings[3] = { "Hello ", "world", " !" };
    TestCopyConstructor_App<std::string>(strings, 10, 3);
}


TEST_F(ArrayTestsF, TestMoveConstructor_Filled) {
    int ints[3] = { 1, 2, 3 };
    TestMoveConstructor_Filled<int>(ints, 10, 3);

    std::string strings[3] = { "Hello ", "world", " !" };
    TestMoveConstructor_Filled<std::string>(strings, 10, 3);
}

TEST_F(ArrayTestsF, TestCopyAssigment_Filled) {
    int ints[3] = { 1, 2, 3 };
    TestCopyAssigment_Filled<int>(ints, 10, 3);

    std::string strings[3] = { "Hello ", "world", " !" };
    TestCopyAssigment_Filled<std::string>(strings, 10, 3);
}

TEST_F(ArrayTestsF, TestMoveAssigment_Filled) {
    int ints[3] = { 1, 2, 3 };
    TestMoveAssigment_Filled<int>(ints, 10, 3);

    std::string strings[3] = { "Hello ", "world", " !" };
    TestMoveAssigment_Filled<std::string>(strings, 10, 3);
}

TEST_F(ArrayTestsF, TestInsertEnd) {
    int ints[3] = { 1, 2, 3 };
    TestInsert<int>(ints, 3, 3, 4);

    std::string strings[3] = { "Hello ", "world", " !" };
    TestInsert<std::string>(strings, 3, 3, "Hello world !");;
}

TEST_F(ArrayTestsF, TestInsertStart) {
    int ints[3] = { 1, 2, 3 };
    TestInsert<int>(ints, 3, 0, 4);


    std::string strings[3] = { "Hello ", "world", " !" };
    TestInsert<std::string>(strings, 3, 0, "Hello world!");;
}

TEST_F(ArrayTestsF, TestRemoveStart) {
    int ints[3] = { 1, 2, 3 };
    TestRemove<int>(ints, 3, 0);

    std::string strings[3] = { "Hello ", "world", " !" };
    TestRemove<std::string>(strings, 3, 0);
}

TEST_F(ArrayTestsF, TestRemoveEnd) {
    int ints[3] = { 1, 2, 3 };
    TestRemove<int>(ints, 3, 2);

    std::string strings[3] = { "Hello ", "world", " !" };
    TestRemove<std::string>(strings, 3, 2);
}


TEST_F(ArrayTestsF, TestRemoveMid) {
    int ints[3] = { 1, 2, 3 };
    TestRemove<int>(ints, 3, 1);

    std::string strings[3] = { "Hello ", "world", " !" };
    TestRemove<std::string>(strings, 3, 1);
}

TEST_F(ArrayTestsF, TestCapacityExpand) {
    int ints[3] = { 1, 2, 3 };
    TestCapacityExpand<int>(ints, 3);

    std::string strings[3] = { "Hello ", "world", " !" };
    TestCapacityExpand<std::string>(strings, 3);
}

TEST_F(ArrayTestsF, TestIterator) {
    int ints[3] = { 1, 2, 3 };
    TestIterator<int>(ints, 3);

    std::string strings[3] = { "Hello ", "world", " !" };
    TestIterator<std::string>(strings, 3);
}

TEST_F(ArrayTestsF, TestReverseIterator) {
    int ints[3] = { 1, 2, 3 };
    TestReverseIterator<int>(ints, 3);

    std::string strings[3] = { "Hello ", "world", " !" };
    TestReverseIterator<std::string>(strings, 3);
}


TEST(ArrayTests, exampleTest) {

    Array<int> a;
    for (int i = 0; i < 10; i++) {
        a.insert(i + 1);
    }
    for (int i = 0; i < a.size(); i++)
        a[i] *= 2;
    for (int i = 0; i < 10; i++) {
        EXPECT_EQ(a[i], (i + 1) * 2);
    }
}

TEST(ArrayTests, exampleStringTest) {

    Array<std::string> a;
    for (int i = 0; i < 10; i++) {
        a.insert(std::to_string(i + 1));
    }

    for (int i = 0; i < 10; i++) {
        EXPECT_EQ(std::to_string(i + 1), a[i]);
    }
}




