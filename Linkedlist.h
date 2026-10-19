#ifndef LINKED_LIST_ENGINE_H
#define LINKED_LIST_ENGINE_H

#include "ENTITY MODEL.h"

struct LedgerNode {
    Order data;
    LedgerNode* prev;
    LedgerNode* next;
    LedgerNode(const Order& order) : data(order), prev(nullptr), next(nullptr) {}
};

class LinkedListEngine {
private:
    LedgerNode* head;
    LedgerNode* tail;
    int size;

public:
    LinkedListEngine();
    ~LinkedListEngine();

    void append(const Order& order);
    bool remove(int orderId);
    void traverseForward() const;
    void clear();
    int getSize() const;
    bool isEmpty() const;
};

#endif // LINKED_LIST_ENGINE_H
