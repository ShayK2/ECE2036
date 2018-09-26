#include <iostream>
#include "complex.h"
#include <cstdlib>
using namespace std;

Complex::Complex(const char* s) {
    string r = "";
    string i = "";
    NaN = false;
    
    if (*s != '(') {
        real = atof(s);
        imag = 0;
    } else {
        while (*s != ',') {
            s++;
            r = r + *s;
        }
        real = strtof((r).c_str(), 0);
        while (*s != ')') {
            s++;
            i = i + *s;
        }
        imag = strtof((i).c_str(), 0);
    }
}

Complex Complex::operator+(const Complex& b) const {
    if (NaN || b.NaN) return Complex();
    return Complex(real + b.real, imag + b.imag);
}

Complex Complex::operator-(const Complex& b) const {
    if (NaN || b.NaN) return Complex();
    return Complex(real - b.real, imag - b.imag);
}

Complex Complex::operator*(const Complex& b) const {
    if (NaN || b.NaN) return Complex();
    return Complex(real * b.real - imag * b.imag, real * b.imag + imag * b.real);
}

double Complex::Mag() const {
    return sqrt(real * real + imag * imag);
}

double Complex::Angle() const {
    return atan(imag / real);
}

Complex Complex::Conj() const {
    return Complex(real, -imag);
}

std::ostream& operator << (std::ostream &os, Complex c) {
    if (c.real < pow(10, -10) && c.real > -pow(10, -10)) c.real = 0;
    if (c.imag < pow(10, -10) && c.imag > -pow(10, -10)) c.imag = 0;
    if (c.NaN) os << "NaN";
    else os << "(" << c.real << ", " << c.imag << ")";
    return os;
}
