#ifndef ENTITY_MODEL_H
#define ENTITY_MODEL_H

#include <string>
#include <chrono>

// Order status enumeration
enum class OrderStatus {
    PENDING,
    PROCESSING,
    FULFILLED,
    AUDITED,
    CANCELLED
};

// Converts OrderStatus to string for display/logging
inline std::string orderStatusToString(OrderStatus status) {
    switch (status) {
        case OrderStatus::PENDING: return "PENDING";
        case OrderStatus::PROCESSING: return "PROCESSING";
        case OrderStatus::FULFILLED: return "FULFILLED";
        case OrderStatus::AUDITED: return "AUDITED";
        case OrderStatus::CANCELLED: return "CANCELLED";
        default: return "UNKNOWN";
    }
}

// Order domain record
struct Order {
    int orderId;
    std::string customerName;
    double totalAmount;
    int priorityLevel; // Higher number = higher priority
    OrderStatus status;
    long long timestamp; // Epoch time in seconds

    // Comparison operators for sorting and searching
    bool operator<(const Order& other) const {
        if (priorityLevel != other.priorityLevel)
            return priorityLevel < other.priorityLevel;
        return orderId < other.orderId;
    }

    bool operator>(const Order& other) const {
        if (priorityLevel != other.priorityLevel)
            return priorityLevel > other.priorityLevel;
        return orderId > other.orderId;
    }

    bool operator==(const Order& other) const {
        return orderId == other.orderId;
    }
};

// Audit Log domain record
struct AuditLog {
    int logId;
    int orderId;
    std::string actionType;
    std::string details;
    long long timestamp;
};

#endif // ENTITY_MODEL_H
