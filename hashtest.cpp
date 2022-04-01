
// CPP program to demonstrate working of unordered_map
// for user defined data types.
#include <unordered_map>
#include <iostream>
using namespace std;
  
// Objects of this class are used as key in hash 
// table. This class must implement operator ==()
// to handle collisions.
class Person {
public:
    string first, last;  // First and last names
  
    Person(string f, string l)
    {
        first = f;
        last = l;
    }
  
    // Match both first and last names in case
    // of collisions.
    bool operator==(const Person& p) const
    {
        return first == p.first && last == p.last;
    }
};
  
class MyHashFunction {
public:
  
    // Use sum of lengths of first and last names
    // as hash function.
    size_t operator()(const Person& p) const
    {
        return p.first.length() + p.last.length();
    }
};
  
// Driver code
int main()
{
    unordered_map<Person, int, MyHashFunction> um;
    Person p1("kartik", "kapoor");
    Person p2("Ram", "Singh");
    Person p3("Laxman", "Prasad");
  
    um[p1] = 100;
    um[p2] = 200;
    um[p3] = 100;

    um.find(p1);
    for (auto e : um) {
        cout << "[" << e.first.first << ", " 
             << e.first.last
             << "] = > " << e.second << '\n';
    }
  
    return 0;
}