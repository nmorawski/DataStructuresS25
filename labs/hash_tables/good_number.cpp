#include <iostream>
#include <unordered_set>
#include <unordered_map>

#define TABLE_SIZE 100
class Node {
public:
    int value;
    Node* next;
    Node(int val) : value(val), next(nullptr) {}
};

int hash(int key) { return key % TABLE_SIZE; }

int sum_squares(int n) {
    int sum = 0;
    while (n > 0) {
        int d = n % 10;
        sum += d * d;
        n /= 10;
    }
    return sum;
}

// bool isGood(int n) {
//     std::unordered_set<int> found_nums;
//     while (n != 1 && found_nums.find(n) == found_nums.end()) {
//         found_nums.insert(n);
//         n = sum_squares(n);
//     }
//     return n == 1;
// }

// bool isGood(int n) {
//     std::unordered_map<int, bool> found_nums;
//     while (n != 1 && found_nums.find(n) == found_nums.end()) {
//         found_nums[n] = true;
//         n = sum_squares(n);
//     }
//     return n == 1;
// }

bool isGood(int n) {
    Node* table[TABLE_SIZE] = {nullptr};

    while (n != 1) {
        int index = hash(n);
        Node* current = table[index];
        while (current != nullptr) {
            if (current->value == n) {
                // Clean up hash table
                for (int i = 0; i < TABLE_SIZE; ++i) {
                    Node* node = table[i];
                    while (node != nullptr) {
                        Node* temp = node;
                        node = node->next;
                        delete temp;
                    }
                }
                return false; // Cycle detected
            }
            current = current->next;
        }
        // Insert the current number into the hash table
        Node* newNode = new Node(n);
        newNode->next = table[index];
        table[index] = newNode;

        n = sum_squares(n);
    }

    // Clean up hash table
    for (int i = 0; i < TABLE_SIZE; ++i) {
        Node* node = table[i];
        while (node != nullptr) {
            Node* temp = node;
            node = node->next;
            delete temp;
        }
    }
    return true; // Reached 1
}

int main() {
	// Test cases
	// 2, 4, 5, 6, 17, 18, 20 are not good numbers.
	// 1, 7, 10, 13, 19, 23, 28, 68 are good numbers.

    int testCases[] = {2,4,5,6,17,18,20,1,7,10,13,19,23,28,68};

    for (int n : testCases) {
        if (isGood(n)) {
            std::cout << n << " is a good number." << std::endl;
        } else {
            std::cout << n << " is not a good number." << std::endl;
        }
    }

    return 0;
}