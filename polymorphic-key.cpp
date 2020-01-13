#include <map>
#include <string>
#include <iostream>

using String      = std::string;
using StringView  = std::string_view;

struct Interface {
   virtual StringView name() const = 0;
   virtual ~Interface() = default;
};

Interface * make_interface(String name) {
   struct Impl : Interface {
      StringView name() const override { return name_; }
      Impl(String && name) : name_(std::move(name)) {}
   private:
      String name_;
   };
   return new Impl(std::move(name));
}

struct Key
{
   explicit Key(Interface * i) : i_(i) {}
   ~Key() { delete i_; }

   Interface const & get_interface() const { return *i_; }
private:
   Interface * i_;
};

bool operator < (Key const & a, Key const & b) {
   return a.get_interface().name() < b.get_interface().name();
}

int main()
{
   using std::piecewise_construct, std::forward_as_tuple;
   std::map<Key, int> map;
   map.emplace(piecewise_construct, forward_as_tuple(make_interface("one")), forward_as_tuple(1));
   map.emplace(piecewise_construct, forward_as_tuple(make_interface("two")), forward_as_tuple(2));
   map.emplace(std::piecewise_construct, std::forward_as_tuple(make_interface("three")), std::forward_as_tuple(3));

   int sum = 0;

   for (std::pair<Key, int> const & kv : map)
      sum += kv.second;

   std::cout << sum << std::endl;
}
