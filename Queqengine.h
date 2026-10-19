#ifndef QUEUE_ENGINE_H
#define QUEUE_ENGINE_H

#include "ENTITY MODEL.h"
#include <vector>

class OrderDispatchQueue {
private:
    std::vector<Order> heap;

    int calculatePriorityScore(const Order& order) const;
    bool hasHigherPriority(const Order& a, const Order& b) const;
    void siftUp(int index);
    void siftDown(int index);

public:
    OrderDispatchQueue();
    void enqueue(const Order& order);
    Order dequeue();
    bool isEmpty() const;
};

#endif // QUEUE_ENGINE_H
