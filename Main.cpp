#include <iostream>
#include <vector>
#include <string>
#include <chrono>
#include <stdexcept>
#include <algorithm>

// ==========================================
// 1. ENTITY MODEL & ENUMERATIONS
// ==========================================

enum class PaymentStatus {
    PAID,
    PENDING,
    FAILED
};

enum class ShippingClass {
    EXPRESS,
    STANDARD,
    ECO
};

inline std::string paymentStatusToString(PaymentStatus status) {
    switch (status) {
        case PaymentStatus::PAID: return "PAID";
        case PaymentStatus::PENDING: return "PENDING";
        case PaymentStatus::FAILED: return "FAILED";
        default: return "UNKNOWN";
    }
}

inline std::string shippingClassToString(ShippingClass sc) {
    switch (sc) {
        case ShippingClass::EXPRESS: return "EXPRESS";
        case ShippingClass::STANDARD: return "STANDARD";
        case ShippingClass::ECO: return "ECO";
        default: return "UNKNOWN";
    }
}

struct Order {
    int orderId;
    std::string customerName;
    double totalValue;
    PaymentStatus paymentStatus;
    ShippingClass shippingClass;

    bool operator<(const Order& other) const { return totalValue < other.totalValue; }
    bool operator>(const Order& other) const { return totalValue > other.totalValue; }
    bool operator==(const Order& other) const { return orderId == other.orderId; }
};


// ==========================================
// 2. DOUBLY LINKED LIST ENGINE (Customer Ledger)
// ==========================================

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
    LinkedListEngine() : head(nullptr), tail(nullptr), size(0) {}

    ~LinkedListEngine() {
        clear();
    }

    void append(const Order& order) {
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

    void traverseForward() const {
        LedgerNode* current = head;
        while (current) {
            std::cout << "  [Ledger] Order ID: " << current->data.orderId
                      << " | Customer: " << current->data.customerName
                      << " | Value: $" << current->data.totalValue
                      << " | Shipping: " << shippingClassToString(current->data.shippingClass)
                      << " | Payment: " << paymentStatusToString(current->data.paymentStatus) << "\n";
            current = current->next;
        }
    }

    void clear() {
        LedgerNode* current = head;
        while (current) {
            LedgerNode* nextNode = current->next;
            delete current;
            current = nextNode;
        }
        head = tail = nullptr;
        size = 0;
    }

    bool isEmpty() const { return size == 0; }
};


// ==========================================
// 3. PRIORITY DISPATCH QUEUE ENGINE
// ==========================================

class OrderDispatchQueue {
private:
    std::vector<Order> heap;

    int calculatePriorityScore(const Order& order) const {
        int score = 0;
        if (order.shippingClass == ShippingClass::EXPRESS) score += 30;
        else if (order.shippingClass == ShippingClass::STANDARD) score += 20;
        else if (order.shippingClass == ShippingClass::ECO) score += 10;

        if (order.paymentStatus == PaymentStatus::PAID) score += 50;
        else if (order.paymentStatus == PaymentStatus::PENDING) score += 10;

        return score;
    }

    bool hasHigherPriority(const Order& a, const Order& b) const {
        return calculatePriorityScore(a) > calculatePriorityScore(b);
    }

    void siftUp(int index) {
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

    void siftDown(int index) {
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

public:
    void enqueue(const Order& order) {
        heap.push_back(order);
        siftUp((int)heap.size() - 1);
    }

    Order dequeue() {
        if (isEmpty()) throw std::runtime_error("Dispatch Queue is empty.");
        Order topOrder = heap[0];
        heap[0] = heap.back();
        heap.pop_back();
        if (!heap.empty()) siftDown(0);
        return topOrder;
    }

    bool isEmpty() const { return heap.empty(); }
};


// ==========================================
// 4. SEARCH & SORT ENGINE (With Metrics)
// ==========================================

struct SortMetrics {
    long long comparisons;
    long long swaps;
    double executionTimeMs;
};

class SearchSortEngine {
private:
    static void merge(std::vector<Order>& orders, int left, int mid, int right, SortMetrics& metrics) {
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

    static void mergeSortHelper(std::vector<Order>& orders, int left, int right, SortMetrics& metrics) {
        if (left < right) {
            int mid = left + (right - left) / 2;
            mergeSortHelper(orders, left, mid, metrics);
            mergeSortHelper(orders, mid + 1, right, metrics);
            merge(orders, left, mid, right, metrics);
        }
    }

    static int partition(std::vector<Order>& orders, int low, int high, SortMetrics& metrics) {
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

    static void quickSortHelper(std::vector<Order>& orders, int low, int high, SortMetrics& metrics) {
        if (low < high) {
            int pi = partition(orders, low, high, metrics);
            quickSortHelper(orders, low, pi - 1, metrics);
            quickSortHelper(orders, pi + 1, high, metrics);
        }
    }

public:
    static void mergeSort(std::vector<Order>& orders, SortMetrics& metrics) {
        metrics = {0, 0, 0.0};
        auto start = std::chrono::high_resolution_clock::now();
        if (!orders.empty()) mergeSortHelper(orders, 0, static_cast<int>(orders.size()) - 1, metrics);
        auto end = std::chrono::high_resolution_clock::now();
        metrics.executionTimeMs = std::chrono::duration<double, std::milli>(end - start).count();
    }

    static void quickSort(std::vector<Order>& orders, SortMetrics& metrics) {
        metrics = {0, 0, 0.0};
        auto start = std::chrono::high_resolution_clock::now();
        if (!orders.empty()) quickSortHelper(orders, 0, static_cast<int>(orders.size()) - 1, metrics);
        auto end = std::chrono::high_resolution_clock::now();
        metrics.executionTimeMs = std::chrono::duration<double, std::milli>(end - start).count();
    }
};


// ==========================================
// 5. MAIN INTERACTIVE DRIVER
// ==========================================

void displayMenu() {
    std::cout << "\n=========================================\n";
    std::cout << "   E-COMMERCE ORDER FULFILLMENT MENU     \n";
    std::cout << "=========================================\n";
    std::cout << "1. Add Order to Customer Ledger\n";
    std::cout << "2. View Customer Ledger\n";
    std::cout << "3. Test Priority Dispatch Queue\n";
    std::cout << "4. Run Sorting Engine Performance Test (Merge vs Quick Sort)\n";
    std::cout << "5. Exit\n";
    std::cout << "Enter your choice (1-5): ";
}

int main() {
    LinkedListEngine ledger;
    int choice;

    do {
        displayMenu();
        std::cin >> choice;
        std::cin.ignore(); // Clear newline buffer

        switch (choice) {
            case 1: {
                Order newOrder;
                int payChoice, shipChoice;

                std::cout << "\n--- Enter Custom Order Details ---\n";
                std::cout << "Enter Order ID (integer): ";
                std::cin >> newOrder.orderId;
                std::cin.ignore();

                std::cout << "Enter Customer Name: ";
                std::getline(std::cin, newOrder.customerName);

                std::cout << "Enter Total Value ($): ";
                std::cin >> newOrder.totalValue;

                std::cout << "Select Payment Status (0: PAID, 1: PENDING, 2: FAILED): ";
                std::cin >> payChoice;
                if (payChoice == 0) newOrder.paymentStatus = PaymentStatus::PAID;
                else if (payChoice == 1) newOrder.paymentStatus = PaymentStatus::PENDING;
                else newOrder.paymentStatus = PaymentStatus::FAILED;

                std::cout << "Select Shipping Class (0: EXPRESS, 1: STANDARD, 2: ECO): ";
                std::cin >> shipChoice;
                if (shipChoice == 0) newOrder.shippingClass = ShippingClass::EXPRESS;
                else if (shipChoice == 1) newOrder.shippingClass = ShippingClass::STANDARD;
                else newOrder.shippingClass = ShippingClass::ECO;

                ledger.append(newOrder);
                std::cout << "Success: Order added to the Ledger!\n";
                break;
            }
            case 2: {
                std::cout << "\n--- Customer Orders Ledger ---\n";
                if (ledger.isEmpty()) {
                    std::cout << "Ledger is currently empty.\n";
                } else {
                    ledger.traverseForward();
                }
                break;
            }
            case 3: {
                std::cout << "\n--- Priority Dispatch Queue Test ---\n";
                OrderDispatchQueue dispatchQueue;

                dispatchQueue.enqueue({201, "Alice Express", 150.0, PaymentStatus::PAID, ShippingClass::EXPRESS});
                dispatchQueue.enqueue({202, "Bob Eco", 45.0, PaymentStatus::PENDING, ShippingClass::ECO});
                dispatchQueue.enqueue({203, "Charlie Standard", 300.0, PaymentStatus::PAID, ShippingClass::STANDARD});

                std::cout << "Dispatching orders based on Priority (Express Paid > Standard/Eco/Pending):\n";
                while (!dispatchQueue.isEmpty()) {
                    Order o = dispatchQueue.dequeue();
                    std::cout << "  -> Dispatched Order ID: " << o.orderId
                              << " | Customer: " << o.customerName
                              << " | Shipping: " << shippingClassToString(o.shippingClass)
                              << " | Payment: " << paymentStatusToString(o.paymentStatus) << "\n";
                }
                break;
            }
            case 4: {
                std::cout << "\n--- Sorting Engine Performance Benchmark ---\n";
                std::vector<Order> sampleOrders = {
                    {1, "Customer Alpha", 450.75, PaymentStatus::PAID, ShippingClass::EXPRESS},
                    {2, "Customer Beta", 35.50, PaymentStatus::PAID, ShippingClass::ECO},
                    {3, "Customer Gamma", 199.99, PaymentStatus::PAID, ShippingClass::STANDARD},
                    {4, "Customer Delta", 1200.00, PaymentStatus::PAID, ShippingClass::EXPRESS},
                    {5, "Customer Epsilon", 85.20, PaymentStatus::PENDING, ShippingClass::STANDARD}
                };

                std::vector<Order> mergeList = sampleOrders;
                std::vector<Order> quickList = sampleOrders;

                SortMetrics mMetrics, qMetrics;
                SearchSortEngine::mergeSort(mergeList, mMetrics);
                SearchSortEngine::quickSort(quickList, qMetrics);

                std::cout << "Sorting Orders by Monetary Total ($):\n";
                std::cout << "  Merge Sort -> Comparisons: " << mMetrics.comparisons
                          << " | Swaps: " << mMetrics.swaps
                          << " | Time: " << mMetrics.executionTimeMs << " ms\n";
                std::cout << "  Quick Sort -> Comparisons: " << qMetrics.comparisons
                          << " | Swaps: " << qMetrics.swaps
                          << " | Time: " << qMetrics.executionTimeMs << " ms\n";
                break;
            }
            case 5:
                std::cout << "Exiting program. Goodbye!\n";
                break;
            default:
                std::cout << "Invalid choice. Please choose between 1 and 5.\n";
        }
    } while (choice != 5);

    return 0;
}
