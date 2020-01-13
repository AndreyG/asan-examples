#include <iostream>
#include <map>
#include <string>

using String      = std::string;
using StringView  = std::string_view;

struct Person
{
    StringView    name()    const { return name_; }
    int           age()     const { return age_; }

    Person(String name, int age)
        : name_(std::move(name))
        , age_(age)
    {}

private:
    String name_;
    int age_;
};

using Dict = std::map<StringView, Person>;  

void insert1(Dict & dict, String name, int age)
{
    Person person(std::move(name), age);
    auto key = person.name();
    dict.emplace(key, std::move(person));
}

void insert2(Dict & dict, String name, int age)
{
    using std::forward_as_tuple;
    StringView key = name;
    dict.emplace(std::piecewise_construct, forward_as_tuple(key), forward_as_tuple(std::move(name), age));
}

int main() {
    Dict dict;
    insert2(dict, "Andrey", 31);
    std::cout << dict.at("Andrey").age() << std::endl;
}
