#include <iostream>

std::string** foo() {
    std::string **temp = new std::string*[2];
    temp[0] = new std::string("ok");
    temp[1] = nullptr;

    return temp;
}

int main() {
    std::string** ret = foo();

    if (ret[1] == nullptr) {
        std::cout << "second is null";
    } else {
        std::cout << *ret[0] << "\n" << *ret[1] << "\n";
    }
    
    return 0;
}