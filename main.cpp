#include "myalloc.h"

int factorial (int value)
{
    if (value == 0) return 1; 
    else return value*factorial(value - 1);
}

int main() {
    std::map<int, int> map1;
    for (int i = 0; i < 10; i++)
    {
        map1[i] = factorial(i);
    }

    std::map<int, int, std::less<int>, simple_allocator<std::pair<const int, int> > > map2;
    for (int i = 0; i < 10; i++)
    {
        map2[i] = factorial(i);
    }



    // Вывод на экран всех значений map1
    std::cout << "map1:" << std::endl;
    for (const auto& pair : map1) {
        std::cout << pair.first << " " << pair.second << std::endl;
    }

    // Вывод на экран всех значений map1
    std::cout << "map2:" << std::endl;
    for (const auto& pair : map2) {
        std::cout << pair.first << " " << pair.second << std::endl;
    }

    my_array<int> arr1;
    my_array<int, simple_allocator<int>> my_first_arr;
    for (int i = 0; i < 10; i++)
    {
        arr1.push_back(i);
    }
    std::cout << "arr1:" << std::endl;
    arr1.print();

    for (int i = 0; i < 10; ++i) {
        my_first_arr.push_back(i);
    }
    std::cout << "my_first_arr:" << std::endl;
    my_first_arr.print();
    return 0;
}