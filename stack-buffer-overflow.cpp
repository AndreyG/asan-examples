#include <iostream>
#include <string>

int main(int argc, char* argv[]) {
    const auto index = std::stoi(argv[1]); 
    int arr[100] = { 1 };
    std::cout << arr[index] << std::endl;
}
