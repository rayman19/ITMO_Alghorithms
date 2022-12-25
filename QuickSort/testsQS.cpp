#include <iostream>
#include <gtest/gtest.h>
#include <algorithm>
#include "QuickSort.h"

class QuickSortTestsF : public testing::Test {
public:
	template<typename T>
	void TestSort(T a, T b, int N)
	{
		std::sort(a, a + N, [](int left, int right) { return left < right; });
		sort(b, b + N - 1, [](int left, int right) { return left < right; });

		for (int i = 0; i < N; i++) 
		{
			EXPECT_EQ(a[i], b[i]);
		}
	}

	template<typename T>
	void TestReverseSort(T a, T b, int N)
	{
		std::sort(a, a + N, [](int left, int right) { return left > right; });
		sort(b, b + N - 1, [](int left, int right) { return left > right; });

		for (int i = 0; i < N; i++)
		{
			EXPECT_EQ(a[i], b[i]);
		}
	}
};

TEST_F(QuickSortTestsF, Sort) {
	int N = 10;
	int* aIntRevSort = new int[N];
	int* bIntRevSort = new int[N];
	char* aCharRevSort = new char[N];
	char* bCharRevSort = new char[N];

	int* aIntSort = new int[N];
	int* bIntSort = new int[N];
	char* aCharSort = new char[N];
	char* bCharSort = new char[N];

	for (int i = 0; i < N; i++) 
	{
		aIntRevSort[i] = N - i;
		bIntRevSort[i] = N - i;
		aCharRevSort[i] = char(N - i);
		bCharRevSort[i] = char(N - i);
		aIntSort[i] = i;
		bIntSort[i] = i;
		aCharSort[i] = char(i);
		bCharSort[i] = char(i);
	}

	TestSort(aIntRevSort, bIntRevSort, N);
	TestSort(aCharRevSort, bCharRevSort, N);
	TestSort(aIntSort, bIntSort, N);
	TestSort(aCharSort, bCharSort, N);
}

TEST_F(QuickSortTestsF, ReverseSort) {
	int N = 10;
	int* aIntSort = new int[N];
	int* bIntSort = new int[N];
	char* aCharSort = new char[N];
	char* bCharSort = new char[N];
	int* aIntRevSort = new int[N];
	int* bIntRevSort = new int[N];
	char* aCharRevSort = new char[N];
	char* bCharRevSort = new char[N];

	for (int i = 0; i < N; i++) 
	{
		aIntRevSort[i] = N - i;
		bIntRevSort[i] = N - i;
		aCharRevSort[i] = char(N - i);
		bCharRevSort[i] = char(N - i);
		aIntSort[i] = i;
		bIntSort[i] = i;
		aCharSort[i] = char(i);
		bCharSort[i] = char(i);
	}

	TestReverseSort(aIntRevSort, bIntRevSort, N);
	TestReverseSort(aCharRevSort, bCharRevSort, N);
	TestReverseSort(aIntSort, bIntSort, N);
	TestReverseSort(aCharSort, bCharSort, N);
}

TEST_F(QuickSortTestsF, SortRand) {
	int N = 10;
	int* aInt = new int[N];
	int* bInt = new int[N];
	char* aChar = new char[N];
	char* bChar = new char[N];

	for (int i = 0; i < N; i++) 
	{
		aInt[i] = rand();
		bInt[i] = aInt[i];
		aChar[i] = char(rand());
		bChar[i] = aChar[i];
	}

	TestSort(aInt, bInt, N);
	TestSort(aChar, bChar, N);
}

TEST_F(QuickSortTestsF, ReverseSortRand) {
	int N = 10;
	int* aInt = new int[N];
	int* bInt = new int[N];
	char* aChar = new char[N];
	char* bChar = new char[N];

	for (int i = 0; i < N; i++) 
	{
		aInt[i] = rand();
		bInt[i] = aInt[i];
		aChar[i] = char(rand());
		bChar[i] = aChar[i];
	}

	TestReverseSort(aInt, bInt, N);
	TestReverseSort(aChar, bChar, N);
}