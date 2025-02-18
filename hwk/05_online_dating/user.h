/*template <class T> class Node {
public:
	Node() : next_(NULL), prev_(NULL) {}
	Node(const T& v) : value_(v), next_(NULL), prev_(NULL) {}
	T value_;
	Node<T>* next_;
	Node<T>* prev_;
};*/
#include <string>
#include <vector>
#include "location.h"
class User {
public:
	User();
    User(std::string name, int age, std::string gender, std::string phone_number, 
        std::string profession, std::string school, Location location, bool isPremium,
        int minAge_pref, int maxAge_pref, int max_dist, std::string gender_pref,
        std::vector<std::string> likes);
	User* next_;

	std::string name_;
	int age_;
	std::string gender_;
	std::string phone_number_;
	std::string profession_;
	std::string school_;
	Location location_;
	bool isPremium_;
	int minAge_pref_;
	int maxAge_pref_;
	int max_dist_;
	std::string gender_pref_;
	std::vector<std::string> likes_;
};