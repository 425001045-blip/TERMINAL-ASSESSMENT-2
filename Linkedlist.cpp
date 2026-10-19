#include "LinkedListEngine.h"
#include <iostream>

LinkedListEngine::LinkedListEngine() : head(nullptr), tail(nullptr), size(0) {}

LinkedListEngine::~LinkedListEngine() {
    clear();
}

void LinkedListEngine::append(const Order& order) {
    LedgerNode* newNode = new LedgerNode(order);
    if (!head) {
        head = tail = newNode;
    } else {
        tail->next = newNode;
        newNode->prev = tail;
        tail = newNode;
    }
    size++;
}

bool LinkedListEngine::remove(int orderId) {
    LedgerNode* current = head;
    while (current) {
        if (current->data.orderId == orderId) {
            if (current->prev) current->prev->next = current->next;
            else head = current->next;

            if (current->next) current->next->prev = current->prev;
            else tail = current->prev;

            delete current;
            size--;
            return true;
        }
        current = current->next;
    }
    return false;
}

void LinkedListEngine::traverseForward() const {
    LedgerNode* current = head;
    while (current) {
        std::cout << "  [Ledger] Order ID: " << current->data.orderId
                  << " | Customer: " << current->data.customerName
                  << " | Value: $" << current->data.totalValue
                  << " | Shipping: " << shippingClassToString(current->data.shippingClass) << "\n";
        current = current->next;
    }
}

void LinkedListEngine::clear() {
    LedgerNode* current = head;
    while (current) {
        LedgerNode* nextNode = current->next;
        delete current;
        current = nextNode;
    }
    head = tail = nullptr;
    size = 0;
}

int LinkedListEngine::getSize() const { return size; }
bool LinkedListEngine::isEmpty() const { return size == 0; }
