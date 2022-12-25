#include <utility>

template<typename T>
class Array final 
{
public:
    class Iterator 
    {
    public:
        Iterator(T* first, int size, bool isReverse) 
        {
            first_ = first;
            last_ = first + size - 1;
            cur_ = (isReverse) ? last_ : first_;
            isReverse_ = isReverse;
        }

        const T& get() const 
        {
            return *(cur_);
        }

        void set(T& value) 
        {
            *cur_ = value;
        }

        void next() 
        {
            cur_ = (isReverse_) ? (cur_- 1) : (cur_ + 1);
        }

        bool hasNext() const 
        {
            if (isReverse_) 
            {
                if (cur_ == first_ - 1) 
                {
                    return false;
                }
            }
            else 
            {
                if (cur_ == last_ + 1) 
                {
                    return false;
                }
            }
            return true;
        }
    private:
        T* cur_;
        T* first_;
        T* last_;
        bool isReverse_;
    };

    class ConstIterator 
    {
    public:
        ConstIterator(T* first, int size, bool isReverse) 
        {
            first_ = first;
            last_ = first + size - 1;
            cur_ = (isReverse) ? last_ : first_;
            isReverse_ = isReverse;
        }

        const T& get() const 
        {
            return *(cur_);
        }

        void next() {
            cur_ = (isReverse_) ? (cur_ - 1) : (cur_ + 1);
        }

        bool hasNext() const 
        {
            if (isReverse_) {
                if (cur_ == first_ - 1) 
                {
                    return false;
                }
            }
            else 
            {
                if (cur_ == last_ + 1) 
                {
                    return false;
                }
            }
            return true;
        }
    private:
        T* cur_;
        T* first_;
        T* last_;
        bool isReverse_;
    };

    Array() : 
        data((T*)malloc(sizeof(T) * 8)),
        cap(8),
        sizer(0)
    {
#ifdef _DEBUG
        std::cout << "Constructor Default\n";
#endif // DEBUG
    }

    Array(int capacity)
        : data((T*)malloc(sizeof(T) * capacity)),
        cap(capacity),
        sizer(0)
    {
#ifdef _DEBUG
        std::cout << "Constructor with Cap\n";
#endif // DEBUG
    }

    Array(const Array<T>& other)
        : data((T*)malloc(sizeof(T) * other.capacity())),
        cap(other.capacity()),
        sizer(other.size())
    {
        int i = 0;
        for (auto it = other.iterator(); it.hasNext(); it.next()) 
        {
            new (data + i) T(it.get());
            i++;
        }

#ifdef _DEBUG
        std::cout << "Constructor Copy\n";
#endif // DEBUG
    }

    Array(Array<T>&& other)
        : data(other.data),
        cap(other.capacity()),
        sizer(other.size())
    {
        other.data = nullptr;
        other.cap = 0;
        other.sizer = 0;

#ifdef _DEBUG
        std::cout << "Constructor Move\n";
#endif // DEBUG
    }

    void operator=(const Array<T>& other)
    {
        if (this == &other) { return; }

        for (int i = 0; i < size(); i++)
        {
            data[i].~T();
        }
        free(data);

        data = (T*)malloc(sizeof(T) * other.capacity());
        cap = other.capacity();
        sizer = other.size();

        int i = 0;
        for (auto it = other.iterator(); it.hasNext(); it.next())
        {
            new (data + i) T(it.get());
            i++;
        }

#ifdef _DEBUG
        std::cout << "Operator Copy\n";
#endif // _DEBUG
    }

    void operator=(Array<T>&& other)
    {
        if (this == &other) { return; }

        for (int i = 0; i < size(); i++) 
        {
            data[i].~T();
        }
        free(data);

        data = other.data;
        cap = other.capacity();
        sizer = other.size();

        other.data = nullptr;
        other.cap = 0;
        other.sizer = 0;

#ifdef _DEBUG
        std::cout << "Operator Move\n";
#endif // _DEBUG
    }

    ~Array() {
        for (int i = 0; i < size(); i++)
        {
            data[i].~T();
        }
        
        free(data);

#ifdef _DEBUG
        std::cout << "Destructor\n";
#endif // _DEBUG
    }

    int insert(const T& value) {
        if (size() >= capacity()) {
            expand();
        }

        new (data + size()) T(value);
        sizer++;
        return size() - 1;
    }

    int insert(int index, const T& value) {
        if (index < 0) return -1;

        if (size() >= capacity()) 
        {
            expand();
        }

        if (index >= size()) 
        {
            new (data + size()) T(value);
        }
        else {
            for (int i = size(); i > index; i--) 
            {
                new (data + i) T(std::move(data[i - 1]));
                data[i].~T();
            }
            new (data + index) T(value);
        }

        sizer++;
        return index;
    }

    void remove(int index) 
    {
        if (index + 1 > sizer || index < 0) return;

        for (int i = index; i < sizer - 1; i++) 
        {
            new (data + i) T(std::move(data[i + 1]));
            array_[i + 1].~T();
        }

        sizer--;
        data[size()].~T();
    }

    void expand() {
        cap *= 2;
        T* tempData = (T*)malloc(sizeof(T) * cap);

        for (int i = 0; i < sizer; i++) {
            new (tempData + i) T(std::move(data[i]));
            data[i].~T();
        }

        free(data);
        data = tempData;
    }

    const int size() const { return sizer; }
    int size() { return sizer; }
    const int capacity() const { return cap; }
    int capacity() { return cap; }

    T& operator[](int index) {
        //if (index > size() - 1 || index < 0) {
        //    return *data;
        //}
        return *(data + index);
    }

    const T& operator[](int index) const {
        //if (index > size() - 1 || index < 0) {
        //    return *data;
        //}
        return *(data + index);
    }

    Iterator iterator() {
        return Iterator(data, size(), false);
    }
    ConstIterator iterator() const {
        return ConstIterator(data, size(), false);
    }

    Iterator reverseIterator() {
        return Iterator(data, size(), true);
    }

    ConstIterator reverseIterator() const {
        return ConstIterator(data, size(), true);
    }

private:
    int cap;
    int sizer;
    T* data;
};