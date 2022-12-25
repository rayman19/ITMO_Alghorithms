const int C_SIZE_INSERTION = 16;


template<typename T, typename Comp>
T* partition(T* first, T* last, Comp comp) {
    T pivot = *last;
    T* i = first;
    T* j = first;

    while (i < last) {
        if (comp(*i, pivot)) {
            std::swap(*i, *j);
            j++;
        }
        i++;
    }

    std::swap(*last, *j);
    return j;
}

template<typename T, typename Comp>
void insertSort(T* first, int size, Comp comp) {
    for (auto i = first + 1; i < first + size; i++) {
        auto j = i - 1;
        auto temp = std::move(*i);
        while (j >= first && comp(temp, *j)) {
            //std::swap(*(j), *(j - 1));
            *(j + 1) = std::move(*j);
            j--;
        }
        *(j + 1) = std::move(temp);
    }
}

template<typename T, typename Comp>
void sort(T* first, T* last, Comp comp) {
    while (first < last) {
        if (last - first + 1 < C_SIZE_INSERTION) {
            insertSort(first, last - first + 1, comp);
            break;
        }
        else {
            T* middle = first + (last - first) / 2;

            if (comp(*last, *first))
                std::swap(*last, *first);

            if (comp(*middle, *first))
                std::swap(*middle, *first);

            if (comp(*last, *middle))
                std::swap(*last, *middle);

            std::swap(*middle, *(last));

            T* pivot = partition(first, last, comp);
            if (pivot - first < last - pivot) {
                sort(first, pivot, comp);
                first = pivot + 1;
            }
            else {
                sort(pivot, last, comp);
                last = pivot - 1;
            }
        }
    }
}

template<typename T, typename Comp>
void sortWithoutInsert(T* first, T* last, Comp comp) {
    while (first < last) {

            T* middle = first + (last - first) / 2;

            if (comp(*last, *first))
                std::swap(*last, *first);

            if (comp(*middle, *first))
                std::swap(*middle, *first);

            if (comp(*last, *middle))
                std::swap(*last, *middle);

            std::swap(*middle, *(last));

            T* pivot = partition(first, last, comp);
            if (pivot - first < last - pivot) {
                sort(first, pivot, comp);
                first = pivot + 1;
            }
            else {
                sort(pivot, last, comp);
                last = pivot - 1;
            }
    }
}