#include <iostream>

template <class T>
class Node {
public:
	T value;
	Node<T>* next;
	Node<T>* prev;

	// constructor
	Node(T val) : value(val), next(nullptr), prev(nullptr) {}
};

// function to merge two sorted doubly linked lists
// this function returns a pointer pointing to the head node of the merged list.
template <class T>
Node<T>* mergeLists(Node<T>* head_A, Node<T>* head_B) {
    if (!head_A) return head_B;
    if (!head_B) return head_A;

    Node<T>* head = nullptr;
    Node<T>* tail = nullptr;

    while (head_A && head_B) {
        Node<T>* nextNode = nullptr;
        if (head_A->value <= head_B->value) {
            nextNode = head_A;
            head_A = head_A->next;
        } else {
            nextNode = head_B;
            head_B = head_B->next;
        }

        if (!head) {
            head = tail = nextNode;
        } else {
            tail->next = nextNode;
            nextNode->prev = tail;
            tail = nextNode;
        }
    }

    if (head_A){
		tail->next = head_A;
		head_A->prev = tail;
		tail = head_A;
	} else {
		tail->next = head_B;
		head_B->prev = tail;
		tail = head_B;
	}

    return head;
}

template <class T>
void DeleteList(Node<T>* &head) {
    Node<T>* current = head;
    while (current) {
        Node<T>* next = current->next;
        delete current;
        current = next;
    }
    head = nullptr;
}

int main() {
	// create 5 nodes and link them to form a linked list, this is linked list A.
	Node<int>* head_A = new Node<int>(1);
	Node<int>* second_A = new Node<int>(3);
	Node<int>* third_A = new Node<int>(5);
	Node<int>* fourth_A = new Node<int>(7);
	Node<int>* fifth_A = new Node<int>(9);

	// link the nodes
	head_A->next = second_A;
	second_A->prev = head_A;
	second_A->next = third_A;
	third_A->prev = second_A;
	third_A->next = fourth_A;
	fourth_A->prev = third_A;
	fourth_A->next = fifth_A;
	fifth_A->prev = fourth_A;

	// traverse linked list A and print the values
	Node<int>* current = head_A;
	while (current != nullptr) {
		std::cout << current->value << " ";
		current = current->next;
	}
	std::cout << std::endl;

	// create 5 nodes and link them to form a linked list, this is linked list B.
	Node<int>* head_B = new Node<int>(2);
	Node<int>* second_B = new Node<int>(4);
	Node<int>* third_B = new Node<int>(6);
	Node<int>* fourth_B = new Node<int>(8);
	Node<int>* fifth_B = new Node<int>(10);

	// link the nodes
	head_B->next = second_B;
	second_B->prev = head_B;
	second_B->next = third_B;
	third_B->prev = second_B;
	third_B->next = fourth_B;
	fourth_B->prev = third_B;
	fourth_B->next = fifth_B;
	fifth_B->prev = fourth_B;

	// traverse linked list B and print the values
	current = head_B;
	while (current != nullptr) {
		std::cout << current->value << " ";
		current = current->next;
	}
	std::cout << std::endl;

	Node<int>* head_C;
	Node<int>* tail_C;
	head_C = mergeLists(head_A, head_B);

	// traverse linked list C and print the values
	current = head_C;
	while (current != nullptr) {
		std::cout << current->value << " ";
		// keep tracking current and when current reaches nullptr, tail_C will be the tail node.
		tail_C = current;
		current = current->next;
	}
	std::cout << std::endl;

	// traverse linked list C backwards and print the values
	current = tail_C;
	while (current != nullptr) {
		std::cout << current->value << " ";
		current = current->prev;
	}
	std::cout << std::endl;

	// clean up memory
    DeleteList(head_C);

	return 0;
}
