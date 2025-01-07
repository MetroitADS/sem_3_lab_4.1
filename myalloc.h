#ifndef MYALLOC_H
#define MYALLOC_H

#include <initializer_list>
#include <stdexcept> 
#include <algorithm>
#include <map>
#include <memory>
#include <iostream>


template <class T>
struct simple_allocator {

    using value_type = T;
    simple_allocator () noexcept {} //не вызывает исключений
    template <class U> simple_allocator ( simple_allocator<U>&) noexcept {} //конструктор копирования который не вызывает исключений      
    T* allocate (std::size_t n)
    {
        return static_cast<T*>(::operator new(n*sizeof(T)));
    }
    void deallocate (T* p, std::size_t n)
    {
        ::operator delete(p);
    }    
};

template <class T, class U>
constexpr bool operator== (const simple_allocator<T>& a1, const
simple_allocator<U>& a2) noexcept
{
    return true;
}
template <class T, class U>
constexpr bool operator!= (const simple_allocator<T>& a1, const
simple_allocator<U>& a2) noexcept
{
    return false;
}




template <typename Type, class Allocator = simple_allocator<Type>>
class my_array {
private:
    Type* arr;
    std::size_t capacity; //"вместимость" данного конкретного массива
    std::size_t currentSize; //текущее количество элементов данного массива
    Allocator alloc; //Аллокатор

public:
    //Конструктор по умолчанию
    my_array() : capacity(10), currentSize(0) {
        arr = alloc.allocate(capacity);
    }

    //Конструктор с пользовательским аллокатором
    my_array(const Allocator& allocator) : alloc(allocator), capacity(10), currentSize(0) {
        arr = alloc.allocate(capacity);
    }
    //Деструктор
    ~my_array() {
        for (std::size_t i = 0; i < currentSize; ++i) {
            std::allocator_traits<Allocator>::destroy(alloc, arr + i);
        }
        alloc.deallocate(arr, capacity);
    }

    //Метод resize с использованием аллокатора
    void resize(std::size_t newCapacity) {
        Type* newArr = alloc.allocate(newCapacity);
        for (std::size_t i = 0; i < currentSize; ++i) {
            std::allocator_traits<Allocator>::construct(alloc, newArr + i, std::move(arr[i])); 
            std::allocator_traits<Allocator>::destroy(alloc, arr + i);
        }
        alloc.deallocate(arr, capacity);
        arr = newArr;
        capacity = newCapacity;
    }

    //Метод push_back с использованием аллокатора
    void push_back(const Type& value) {
        if (currentSize >= capacity) {
            resize(capacity * 2);
        }
        std::allocator_traits<Allocator>::construct(alloc, arr + currentSize, value);
        ++currentSize;
    }

    //Метод insert с использованием аллокатора
    void insert(std::size_t index, const Type& value) {
        if (index > currentSize) {
            throw std::out_of_range("Index out of Range");
        }
        if (currentSize >= capacity) {
            resize(capacity * 2);
        }
        for (std::size_t i = currentSize; i > index; --i) {
            std::allocator_traits<Allocator>::construct(alloc, arr + i, std::move(arr[i - 1]));
            std::allocator_traits<Allocator>::destroy(alloc, arr + i - 1); 
        }
        std::allocator_traits<Allocator>::construct(alloc, arr + index, value);
        ++currentSize;
    }

    //Метод erase с использованием аллокатора
    void erase(std::size_t index) {
        if (index >= currentSize) {
            throw std::out_of_range("Index out of range");
        }
        for (std::size_t i = index; i < currentSize - 1; ++i) {
            std::allocator_traits<Allocator>::construct(alloc, arr + i, std::move(arr[i + 1]));
            std::allocator_traits<Allocator>::destroy(alloc, arr + i + 1);
        }
        std::allocator_traits<Allocator>::destroy(alloc, arr + currentSize - 1); 
        --currentSize;
    }

    std::size_t size() {
        return currentSize;
    }

    //оператор индексации
    Type& operator[](std::size_t index) {
        if (index >= currentSize) {
            throw std::out_of_range("Index out of range");
        }
        return arr[index];
    }

    void print() {
        for (std::size_t i = 0; i < currentSize; ++i) {
            std::cout << arr[i];
            if (i < currentSize - 1) {
                std::cout << ", ";
            }
        }
        std::cout << std::endl;
    }

    //итератор
    struct Iterator {
        Type* ptr;

        Iterator(Type* ptr) : ptr(ptr) {}

        //оператор разыменования
        Type& operator*() {
            if (ptr == nullptr) {
                throw std::out_of_range("Index out of range"); 
            }
            return *ptr;
        }

        Type& get() {
            if (ptr == nullptr) {
                throw std::out_of_range("Index out of range");
            }
            return *ptr;
        }
        bool operator!=(const Iterator& other) {
            return ptr!= other.ptr;
        }

        //следующий элемент
        Iterator& operator++() {
            ++ptr;
            return *this;
        }
    };

    //начало контейнера
    Iterator begin() {
        return Iterator(arr);
    }

    //конец контейнера
    Iterator end() {
        return Iterator(arr + currentSize);
    }
};


#endif