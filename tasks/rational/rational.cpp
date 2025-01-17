#include "rational.h"

Rational::Rational() : numer_(0), denom_(1) {
}

Rational::Rational(int val) : numer_(val), denom_(1) {
}  // NOLINT

Rational::Rational(const int numer, const int denom) {
    Set(numer, denom);
}

int Rational::GetNumerator() const {
    return numer_;
}

int Rational::GetDenominator() const {
    return denom_;
}

void Rational::SetNumerator(const int value) {
    Set(value, denom_);
}

void Rational::SetDenominator(const int value) {
    Set(numer_, value);
}

Rational& operator+=(Rational& lhs, const Rational& rhs) {
    const int64_t lhs_numer = lhs.GetNumerator();
    const int64_t rhs_numer = rhs.GetNumerator();
    const int64_t lhs_denom = lhs.GetDenominator();
    const int64_t rhs_denom = rhs.GetDenominator();

    lhs.Set(lhs_numer * rhs_denom + rhs_numer * lhs_denom, lhs_denom * rhs_denom);
    return lhs;
}

Rational& operator*=(Rational& lhs, const Rational& rhs) {
    lhs.Set(lhs.GetNumerator() * rhs.GetNumerator(), lhs.GetDenominator() * rhs.GetDenominator());
    return lhs;
}

Rational& operator++(Rational& ratio) {
    ratio.numer_ += ratio.denom_;
    return ratio;
}

Rational& operator--(Rational& ratio) {
    ratio.numer_ -= ratio.denom_;
    return ratio;
}

std::istream& operator>>(std::istream& is, Rational& ratio) {
    int32_t numer = 0;
    int32_t denom = 1;
    is >> numer;

    char c = 0;
    if (is >> c) {
        if (c == '/') {
            is >> denom;
        } else {
            is.unget();
        }
    }

    ratio = Rational(numer, denom);
    return is;
}

Rational operator+(const Rational& ratio) {
    return ratio;
}

Rational operator-(const Rational& ratio) {
    return Rational(-ratio.GetNumerator(), ratio.GetDenominator());
}

Rational operator~(const Rational& ratio) {
    return Rational(ratio.GetDenominator(), ratio.GetNumerator());
}

Rational& operator-=(Rational& lhs, const Rational& rhs) {
    return lhs += -rhs;
}

Rational& operator/=(Rational& lhs, const Rational& rhs) {
    return lhs *= ~rhs;
}

Rational operator+(const Rational& lhs, const Rational& rhs) {
    Rational res = lhs;
    res += rhs;
    return res;
}

Rational operator-(const Rational& lhs, const Rational& rhs) {
    return lhs + -rhs;
}

Rational operator*(const Rational& lhs, const Rational& rhs) {
    Rational res = lhs;
    res *= rhs;
    return res;
}

Rational operator/(const Rational& lhs, const Rational& rhs) {
    return lhs * ~rhs;
}

Rational operator++(Rational& ratio, int) {
    const Rational res = ratio;
    ++ratio;
    return res;
}

Rational operator--(Rational& ratio, int) {
    const Rational res = ratio;
    --ratio;
    return res;
}

bool operator<(const Rational& lhs, const Rational& rhs) {
    return static_cast<int64_t>(lhs.GetNumerator()) * static_cast<int64_t>(rhs.GetDenominator()) <
           static_cast<int64_t>(rhs.GetNumerator()) * static_cast<int64_t>(lhs.GetDenominator());
}

bool operator>(const Rational& lhs, const Rational& rhs) {
    return static_cast<int64_t>(lhs.GetNumerator()) * static_cast<int64_t>(rhs.GetDenominator()) >
           static_cast<int64_t>(rhs.GetNumerator()) * static_cast<int64_t>(lhs.GetDenominator());
}

bool operator<=(const Rational& lhs, const Rational& rhs) {
    return static_cast<int64_t>(lhs.GetNumerator()) * static_cast<int64_t>(rhs.GetDenominator()) <=
           static_cast<int64_t>(rhs.GetNumerator()) * static_cast<int64_t>(lhs.GetDenominator());
}

bool operator>=(const Rational& lhs, const Rational& rhs) {
    return static_cast<int64_t>(lhs.GetNumerator()) * static_cast<int64_t>(rhs.GetDenominator()) >=
           static_cast<int64_t>(rhs.GetNumerator()) * static_cast<int64_t>(lhs.GetDenominator());
}

bool operator==(const Rational& lhs, const Rational& rhs) {
    return lhs.GetDenominator() == rhs.GetDenominator() && lhs.GetNumerator() == rhs.GetNumerator();
}

bool operator!=(const Rational& lhs, const Rational& rhs) {
    return !(lhs == rhs);
}

std::ostream& operator<<(std::ostream& os, const Rational& ratio) {
    os << ratio.GetNumerator();
    if (ratio.GetDenominator() != 1) {
        os << '/' << ratio.GetDenominator();
    }

    return os;
}

void Rational::Set(int64_t numer, int64_t denom) {
    if (denom == 0) {
        throw RationalDivisionByZero();
    }

    int64_t com_val = std::gcd(numer, denom);
    if (denom < 0) {
        com_val = -com_val;
    }

    numer_ = static_cast<int>(numer / com_val);
    denom_ = static_cast<int>(denom / com_val);
}