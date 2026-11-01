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

    bool isEmpty() {
        return head == nullptr;
    }

    int size() {
        return cnt;
    }

    void clear() {
        while (!isEmpty()) {
            pop();
        }
    }
};