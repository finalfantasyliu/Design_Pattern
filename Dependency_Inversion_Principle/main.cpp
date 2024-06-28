#include <iostream>
#include <cstdio>
#include <string>
#include <vector>
#include <fstream>
#include <tuple>
#include <stdio.h>

using namespace std;
/* A. High level modules should not depend on low-level modules.
 *    Both should depend on abstraction
 * B. Abstractions should not depend on details
 * */
enum class Relationship {
    parent,
    child,
    sibling
};

struct Person {
    string name;
};

struct RelationshipBrowser {
    virtual vector<Person> find_all_children_of(const string &name) = 0;
};

struct Relationships : RelationshipBrowser { //low-level modules 假relations為private，那外部函數就無法取得，因此導致high-level modules會失效
    vector<tuple<Person, Relationship, Person>> relations;

    void add_parent_and_child(const Person &parent, const Person &child) {
        relations.push_back({parent, Relationship::parent, child});
        relations.push_back({child, Relationship::child, parent});
    }

    vector<Person> find_all_children_of(const string &name) override {
        vector<Person> result;
        for (auto &&[first, rel, second]: relations) {
            if (first.name == name && rel == Relationship::parent) {
                result.push_back(second);
            }
        }
        return result;
    }

};

struct Research { //high-level modules
    /* Research(Relationships &relationships) {
         auto &relations = relationships.relations;
         for (auto &&[first, rel, second]: relations) {
             if (first.name == "John" && rel == Relationship::parent) {
                 cout << "John has child called " << second.name << endl;
             }
         }
     }*/
    Research(RelationshipBrowser &browser) {
        for (auto &child: browser.find_all_children_of("john")) {
            cout << "John has a child: " << child.name << endl;
        }

    }
};

int main() {
    Person parent{"John"};
    Person child1{"Chris"}, child2{"Matt"};
    Relationships relationships;
    relationships.add_parent_and_child(parent, child1);
    relationships.add_parent_and_child(parent, child2);
    Research _(relationships);
    return 0;
}
