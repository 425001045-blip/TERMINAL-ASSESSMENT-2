#ifndef STACK_ENGINE_H
#define STACK_ENGINE_H

#include "ENTITY MODEL.h"

#include <vector>

class CartStack {
private:
    std::vector<CartItem> items;
public:
    void pushItem(const CartItem& item);
    CartItem popItem();
    bool isEmpty() const;
    double getTotalCartPrice() const;
};

class CouponStack {
private:
    std::vector<Coupon> coupons;
public:
    void pushCoupon(const Coupon& coupon);
    Coupon popCoupon();
    bool isEmpty() const;
    double getTotalDiscount() const;
};

#endif // STACK_ENGINE_H
