#include "QueueEngine.h"
#include <stdexcept>
#include <algorithm>

OrderDispatchQueue::OrderDispatchQueue() {}

int OrderDispatchQueue::calculatePriorityScore(const Order& order) const {
    int score = 0;
    if (order.shippingClass == ShippingClass::EXPRESS) score += 30;
    else if (order.shippingClass == ShippingClass::STANDARD) score += 20;
    else if (order.shippingClass == ShippingClass::ECO) score += 10;

    if (order.paymentStatus == PaymentStatus::PAID) score += 50;
    else if (order.paymentStatus == PaymentStatus::PENDING) score += 10;

    return score;
}

bool OrderDispatchQueue::hasHigherPriority(const Order& a, const Order& b) const {
    return calculatePriorityScore(a) > calculatePriorityScore(b);
}

void OrderDispatchQueue::siftUp(int index) {
    while (index > 0) {
        int parent = (index - 1) / 2;
        if (hasHigherPriority(heap[index], heap[parent])) {
            std::swap(heap[index], heap[parent]);
            index = parent;
        } else {
            break;
        }
    }
}

void OrderDispatchQueue::siftDown(int index) {
    int maxIndex = index;
    int size = (int)heap.size();
    int left = 2 * index + 1;
    int right = 2 * index + 2;

    if (left < size && hasHigherPriority(heap[left], heap[maxIndex])) maxIndex = left;
    if (right < size && hasHigherPriority(heap[right], heap[maxIndex])) maxIndex = right;

    if (index != maxIndex) {
        std::swap(heap[index], heap[maxIndex]);
        siftDown(maxIndex);
    }
}

void OrderDispatchQueue::enqueue(const Order& order) {
    heap.push_back(order);
    siftUp((int)heap.size() - 1);
}

Order OrderDispatchQueue::dequeue() {
    if (isEmpty()) throw std::runtime_error("Dispatch Queue is empty.");
    Order topOrder = heap[0];
    heap[0] = heap.back();
    heap.pop_back();
    if (!heap.empty()) siftDown(0);
    return topOrder;
}

bool OrderDispatchQueue::isEmpty() const { return heap.empty(); }
