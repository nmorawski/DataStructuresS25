// A simple "caller ID" program

#include <iostream>
#include <vector>
#include <string>
#include <map>
using namespace std;

// add a number, name pair to the phonebook
#define VECTOR_

#ifdef VECTOR
void add(vector<string> &phonebook, int number, string const& name) {
  phonebook[number] = name;
}
#endif
void add(map<int, string> &phonebook, int number, string const& name) {//O(logn)
  phonebook[number] = name;
}

// given a phone number, determine who is calling
/*void identify(const vector<string> & phonebook, int number) {
  if (phonebook[number] == "UNASSIGNED") 
    cout << "unknown caller!" << endl;
  else 
    cout << phonebook[number] << " is calling!" << endl;
}*/
void identify(const map<int,string> &phonebook, int number) {//O(logn)
  map<int,string>::const_iterator itr = phonebook.find(number);
  if (itr == phonebook.end()) 
    cout << "unknown caller!" << endl;
  else 
    cout << itr->second << " is calling!" << endl;
}

int main() {
  // create the phonebook; initially all numbers are unassigned
  //vector<string> phonebook(10000, "UNASSIGNED");
  map<int,string> phonebook;

  // add several names to the phonebook
  add(phonebook, 1111, "fred");
  add(phonebook, 2222, "sally");
  add(phonebook, 3333, "george");

  // test the phonebook
  identify(phonebook, 2222);
  identify(phonebook, 4444);
  identify(phonebook, 4444);
}