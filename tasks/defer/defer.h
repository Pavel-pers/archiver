#pragma once

template <typename Lambda>
class DeferredFunction {
public:
    DeferredFunction(Lambda callback) : callback_(callback) {
    }

    ~DeferredFunction() {
        callback_();
    };

private:
    Lambda callback_;
};

#define __DEFER_CONCAT_VAR_NAMES(ls, rs) ls##rs
#define __DEFER_MID_CONCAT(ls, rs) __DEFER_CONCAT_VAR_NAMES(ls, rs)
#define __DEFER_GEN_NEW_NAME __DEFER_MID_CONCAT(___defer_var_, __COUNTER__)
#define defer DeferredFunction __DEFER_GEN_NEW_NAME = [&]()
