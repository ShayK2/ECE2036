// Class declaration for Complex number class
// ECE2036 Lab 4

#include <iostream>
#include <math.h>

class Complex {
 private:
  double real;
  double imag;
  bool NaN;
 
 public:
  Complex():NaN(true) {}
  Complex(double r):real(r),imag(0),NaN(false) {}
  Complex(double r, double i):real(r),imag(i),NaN(0) {}
  Complex(const char*);

  Complex operator+ (const Complex& b) const;
  Complex operator- (const Complex& b) const;
  Complex operator* (const Complex& b) const;

  Complex operator/ (const Complex& b) const {
      if (NaN || b.NaN) return Complex();
      double m= Mag();
      double m_b =sqrt((b*b.Conj()).real);
      if (m_b==0) return Complex();
      else m/=m_b;
      double a= Angle()-b.Angle();
      return Complex(m*cos(a),m*sin(a));
  }

  double Mag() const;
  double Angle() const;
  Complex Conj() const;

  friend std::ostream& operator << (std::ostream &os, Complex c);
};

std::ostream& operator << (std::ostream &os, Complex c);
