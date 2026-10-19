#include "SearchSortEngine.h"
#include <chrono>
#include <algorithm>

void SearchSortEngine::merge(std::vector<Order>& orders, int left, int mid, int right, SortMetrics& metrics) {
    int n1 = mid - left + 1;
    int n2 = right - mid;
    std::vector<Order> L(n1), R(n2);

    for (int i = 0; i < n1; i++) L[i] = orders[left + i];
    for (int j = 0; j < n2; j++) R[j] = orders[mid + 1 + j];

    int i = 0, j = 0, k = left;
    while (i < n1 && j < n2) {
        metrics.comparisons++;
        if (L[i].totalValue <= R[j].totalValue) orders[k++] = L[i++];
        else orders[k++] = R[j++];
        metrics.swaps++;
    }
    while (i < n1) { orders[k++] = L[i++]; metrics.swaps++; }
    while (j < n2) { orders[k++] = R[j++]; metrics.swaps++; }
}

void SearchSortEngine::mergeSortHelper(std::vector<Order>& orders, int left, int right, SortMetrics& metrics) {
    if (left < right) {
        int mid = left + (right - left) / 2;
        mergeSortHelper(orders, left, mid, metrics);
        mergeSortHelper(orders, mid + 1, right, metrics);
        merge(orders, left, mid, right, metrics);
    }
}

void SearchSortEngine::mergeSort(std::vector<Order>& orders, SortMetrics& metrics) {
    metrics = {0, 0, 0.0};
    auto start = std::chrono::high_resolution_clock::now();
    if (!orders.empty()) mergeSortHelper(orders, 0, static_cast<int>(orders.size()) - 1, metrics);
    auto end = std::chrono::high_resolution_clock::now();
    metrics.executionTimeMs = std::chrono::duration<double, std::milli>(end - start).count();
}

int SearchSortEngine::partition(std::vector<Order>& orders, int low, int high, SortMetrics& metrics) {
    Order pivot = orders[high];
    int i = (low - 1);
    for (int j = low; j <= high - 1; j++) {
        metrics.comparisons++;
        if (orders[j].totalValue <= pivot.totalValue) {
            i++;
            std::swap(orders[i], orders[j]);
            metrics.swaps++;
        }
    }
    std::swap(orders[i + 1], orders[high]);
    metrics.swaps++;
    return (i + 1);
}

void SearchSortEngine::quickSortHelper(std::vector<Order>& orders, int low, int high, SortMetrics& metrics) {
    if (low < high) {
        int pi = partition(orders, low, high, metrics);
        quickSortHelper(orders, low, pi - 1, metrics);
        quickSortHelper(orders, pi + 1, high, metrics);
    }
}

void SearchSortEngine::quickSort(std::vector<Order>& orders, SortMetrics& metrics) {
    metrics = {0, 0, 0.0};
    auto start = std::chrono::high_resolution_clock::now();
    if (!orders.empty()) quickSortHelper(orders, 0, static_cast<int>(orders.size()) - 1, metrics);
    auto end = std::chrono::high_resolution_clock::now();
    metrics.executionTimeMs = std::chrono::duration<double, std::milli>(end - start).count();
}
