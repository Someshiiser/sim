#include<bits/stdc++.h>
#include <iostream>
#include <string>

using namespace std;   // now you can write 'string' and 'cout' directly

class Person {
private:
    string name;
    int age;

public:
    Person() : name(""), age(0) {}
    Person(const string& n, int a) : name(n), age(a) {}

    string getName() const { return name; }
    void   setName(const string& n) { name = n; }

    int    getAge() const { return age; }
    void   setAge(int a) { if (a >= 0) age = a; }

    void introduce() const {
        cout << "Hi, I am " << name
             << " and I am " << age << " years old.\n";
    }
};

class lodu{
    private:
        int physics;
        int math;
    public:
        number(): physics(),math(){};
        number(const )
}



int main() {
    Person alice;              
    Person bob("Bob", 30);     

    alice.setName("Alice");
    alice.setAge(25);

    cout << alice.getName() << endl;  // prints: Alice
    cout << bob.getAge()    << endl;  // prints: 30

    alice.introduce();
    bob.introduce();

    return 0;
}


