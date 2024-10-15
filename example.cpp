#include "vector.hpp"

#include <initializer_list>
#include <iostream>

int main() {
    Vector<int> v = {1, 2, 3};
    std::cout << "Vector of size: " << v.Size() << "  and capacity: " << v.Capacity() << std::endl;
    for (auto& e : v) {
        std::cout << e << " ";
    }
    std::cout << std::endl;

    v.Erase(v.begin() + 1);
    std::cout << "Vector of size: " << v.Size() << "  and capacity: " << v.Capacity() << std::endl;
    for (auto& e : v) {
        std::cout << e << " ";
    }
    std::cout << std::endl;

    auto copy = v;
    std::cout << std::boolalpha << (copy == v) << std::endl;

    copy.Reserve(100);
    std::cout << "Copy size: " << copy.Size() << "    and capacity: " << copy.Capacity()
              << std::endl;

    for (size_t i = 0; i < 100; ++i) {
        copy.PushBack(i);
    }
    std::cout << "Copy size: " << copy.Size() << "  and capacity: " << copy.Capacity() << std::endl;

    copy.Clear();
    std::cout << "Copy size: " << copy.Size() << "    and capacity: " << copy.Capacity()
              << std::endl;
    return 0;
}