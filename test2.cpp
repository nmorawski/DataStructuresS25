public: 
    T data; 
    Node* next; 
    Node(T val) : data(val), next(nullptr) {} 
}; 

// template <class T> 
// Node<T>* mergeSortedLists(Node<T>* head1, Node<T>* head2) { 
//     // Base cases 
//     if (!head1) return head2; 
//     if (!head2) return head1; 
    
//     // Recursive case: choose the smaller node 
//     if (head1->data < head2->data) { 
//         head1->next = mergeSortedLists(head1->next, head2); 
//         return head1; 
//     } else { 
//         head2->next = mergeSortedLists(head1, head2->next); 
//         return head2; 
//     } 
// }

template <class T> 
Node<T>* merge_lists(Node<T>* list1, Node<T>* list2) { 
    // Base cases 
    if (list1->next == nullptr){
        list1->next = list2;
        list2 = nullptr;
    } else if (list1->data <= list2->data && list1->next->data >= list2->data) {
        Node<T>* temp1 = list2->next;
        Node<T>* temp2= list1->next;
        list1->next = list2;
        list2->next = temp2;
        list2 = temp1;
        list1 = list1->next;
    }
    return merge_lists(list1->next,list2);
} 

// Utility function to print the linked list 
template <class T> 
void printList(Node<T>* head) { 
    while (head) { 
        std::cout << head->data << " -> "; 
        head = head->next; 
    } 
    std::cout << "NULL" << std::endl; 
} 

// Utility function to insert a node at the end 
template <class T> 
void insert(Node<T>* &head, T value) { 
    if (!head) { 
        head = new Node<T>(value); 
        return; 
    } 
    Node<T>* temp = head; 
    while (temp->next) 
        temp = temp->next; 
    temp->next = new Node<T>(value); 
} 

int main() { 
    Node<int>* head1 = nullptr; 
    Node<int>* head2 = nullptr; 
    
    // Creating first sorted linked list: 1 -> 3 -> 5 
    insert(head1, 1); 
    insert(head1, 3); 
    insert(head1, 5); 
    
    // Creating second sorted linked list: 2 -> 4 -> 6 
    insert(head2, 2); 
    insert(head2, 4); 
    insert(head2, 6); 
    
    std::cout << "List 1: "; 
    printList(head1); 
    std::cout << "List 2: "; 
    printList(head2); 
    
    // Merge lists 
    //Node<int>* mergedHead = mergeSortedLists(head1, head2); 
    Node<int>* mergedHead = merge_lists(head1, head2); 
    std::cout << "Merged List: "; 
    printList(mergedHead); 
    return 0; 
}