#include <iostream>
#include <vector>
#include <algorithm>

#define TABLE_SIZE 100

class Node {
public:
    int value;
    Node* next;
    Node(int val) : value(val), next(nullptr) {}
};

int hash(int key) { return abs(key) % TABLE_SIZE; } // changed to abs(key)

class HashTable {
public:
    Node* table[TABLE_SIZE];

    HashTable();
    ~HashTable();

    void add(int value);
    bool identify(int value);
};

HashTable::HashTable() {
	for (int i = 0; i < TABLE_SIZE; ++i)
		table[i] = nullptr;
}

HashTable::~HashTable() {
	for (int i = 0; i < TABLE_SIZE; ++i) {
		Node* current = table[i];
		while (current != nullptr) {
			Node* temp = current;
			current = current->next;
			delete temp;
		}
	}
}

void HashTable::add(int value) {
	int index = hash(value);
	Node* newNode = new Node(value);
	newNode->next = table[index];
	table[index] = newNode;
}

bool HashTable::identify(int value) {
	int index = hash(value);
	Node* current = table[index];
	while (current != nullptr) {
		if (current->value == value) {
			return true;
		}
		current = current->next;
	}
	return false;
}

int longestConsecutive(std::vector<int>& nums) {
    if (nums.empty()) return 0;

    HashTable numSet;
    for (int num : nums) // Populate hash table
        numSet.add(num);

    int longestStreak = 0;

    for (int num : nums) {
        if (!numSet.identify(num - 1)) { // Check if smallest number of potential sequence
            int currentNum = num;
            int currentStreak = 1;

            while (numSet.identify(currentNum + 1)) { // Check for next number 
                currentNum += 1;
                currentStreak += 1;
            }

            longestStreak = std::max(longestStreak, currentStreak);
        }
    }

    return longestStreak;
}

int main() {
	//std::vector<int> nums = {100, 4, 200, 1, 3, 2};
	std::vector<int> nums = {100, 4, 200, 1, 3, 2, 2, 2, 2, 3};
	//std::vector<int> nums = {100, 4, 200, 1, 3, 2, 5, 6};
	//std::vector<int> nums = {0,3,7,2,5,8,4,6,0,1};
	//std::vector<int> nums = {100, 4, 200, 201, 202, 203, 205, 204, 1, 3, 2};
	//std::vector<int> nums = {-3,0,1,2,3,-2,-1,-5};
	int size = nums.size();
	std::cout<< "for vector {";
	for(int i=0;i<size-1;i++){
		std::cout<< nums[i] << ",";
	}
	std::cout<< nums[size-1] << "}" <<std::endl;
	int length = longestConsecutive(nums);
	std::cout << "The length of the longest consecutive sequence is: " << length << std::endl;
	return 0;
}
