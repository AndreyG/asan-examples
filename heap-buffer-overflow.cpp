#include <iostream>
#include <string>
#include <vector>

int main(int argc, char* argv[]) {
    const auto index = std::stoi(argv[1]); 
    std::vector<int> vec(100);
    std::cout << vec[index] << std::endl;
}
