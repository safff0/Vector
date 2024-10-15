# Vector

A simple implementation of std::vector.

## example.cpp
```C++
#include "vector.hpp"

#include <initializer_list>
#include <iostream>

int main() {
    // Vector Initialization
    Vector<int> v = {1, 2, 3};
    std::cout << "Vector of size: " << v.Size() << "  and capacity: " << v.Capacity() << std::endl;
    for (auto& e : v) {
        std::cout << e << " ";
    }
    std::cout << std::endl;

    // Erase method
    v.Erase(v.begin() + 1);
    std::cout << "Vector of size: " << v.Size() << "  and capacity: " << v.Capacity() << std::endl;
    for (auto& e : v) {
        std::cout << e << " ";
    }
    std::cout << std::endl;

    // operator==
    auto copy = v;
    std::cout << std::boolalpha << (copy == v) << std::endl;

    // Reserve method
    copy.Reserve(100);
    std::cout << "Copy size: " << copy.Size() << "    and capacity: " << copy.Capacity()
              << std::endl;

    // Capacity doubles during reallocation
    for (size_t i = 0; i < 100; ++i) {
        copy.PushBack(i);
    }
    std::cout << "Copy size: " << copy.Size() << "  and capacity: " << copy.Capacity() << std::endl;

    // Erasing elements doesn't drop capacity
    copy.Clear();
    std::cout << "Copy size: " << copy.Size() << "    and capacity: " << copy.Capacity()
              << std::endl;

    return 0;
}
```

## Output
```Bash
Vector of size: 3  and capacity: 3
1 2 3 
Vector of size: 2  and capacity: 3
1 3 
true
Copy size: 2    and capacity: 100
Copy size: 102  and capacity: 200
Copy size: 0    and capacity: 200
```