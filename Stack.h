#pragma once
#include <iostream>

template <typename T>
class Stack {
private:
    struct Node {
        T data;
        Node* next;
    };
    Node* head;
    int cnt;

public:

    Stack() {//its a constructor
        head = nullptr;
        cnt = 0;
    }

    // claude edited: destructor added so nodes are freed (was a memory leak)
    ~Stack() {
        clear();
    }

    // claude edited: copying is disabled, a copy would share nodes and double-delete them
    Stack(const Stack&) = delete;
    Stack& operator=(const Stack&) = delete;

    void push(T val) {
        Node* newNode = new Node();
        newNode->data = val;
        newNode->next = head;
        head = newNode;
        cnt++;
    }

    T pop() {
        if (isEmpty()) {
            std::cout << "Stack is empty!\n";
            return T();
        }

        Node* temp = head;
        T data = temp->data;
        head = head->next;
        delete temp;
        cnt--;
        return data;
    }

    bool isEmpty() const {   // claude edited: made const
        return head == nullptr;
    }

    int size() const {       // claude edited: made const
        return cnt;
    }

    void clear() {
        while (!isEmpty()) {
            pop();
        }
    }
};