#ifndef SEARCH_SORT_ENGINE_H
#define SEARCH_SORT_ENGINE_H

#include "ENTITY MODEL.h"
#include <vector>

struct SortMetrics {
    long long comparisons;
    long long swaps;
    double executionTimeMs;
};

class SearchSortEngine {
public:
    static void mergeSort(std::vector<Order>& orders, SortMetrics& metrics);
    static void quickSort(std::vector<Order>& orders, SortMetrics& metrics);

private:
    static void merge(std::vector<Order>& orders, int left, int mid, int right, SortMetrics& metrics);
    static void mergeSortHelper(std::vector<Order>& orders, int left, int right, SortMetrics& metrics);
    static int partition(std::vector<Order>& orders, int low, int high, SortMetrics& metrics);
    static void quickSortHelper(std::vector<Order>& orders, int low, int high, SortMetrics& metrics);
};

#endif // SEARCH_SORT_ENGINE_H
