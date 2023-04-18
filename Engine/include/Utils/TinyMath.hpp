#ifndef TINYMATH_HPP
#define TINYMATH_HPP

#pragma once

#include <cmath>
#include <ostream>

namespace CSPill::Math {

// Forward references of each of the structs
struct Vec2D;
struct Matrix3D;

/** Vec2D performs vector operations with 3-dimensions
 *  The purpose of this class is primarily for 3D graphics
 *  applications.
 */
struct Vec2D {
  // I'm wrapping all of our member variables in a union.
  // So long as you make sure the byte-size is equivalent, this
  // allows you to access the data in whatever form is most convenient.
  // Meaning I can access Vec2D[0] or equivalently Vec2D.x .
  // I can also pass Vec2D internal array or just individual values if needed
  // conveniently.
  union {
    struct {
      // Note: x,y are a convention
      // x,y could be position, but also any 2-component value.
      float x{0};
      float y{0};
      float w{0};  // 'w' is a third hidden component that could be used
      // to determine if this is being used as a vector or a point
      // If w=0 then a vector, if w=1 then a point
      // DO not use 'w' for any computation however.
    };
    float data[3];  // for convenience if you want to access as 'data' or pass
                    // Vec2D as an array
  };

  // Default conostrutcor
  // 'why default?'
  // https://stackoverflow.com/questions/20828907/the-new-keyword-default-in-c11
  //    Vec2D() = default;

  /**
   * This initializes the values x,y, and the 'w' value
   * @param _x float, x component
   * @param _y float, y component
   * @param _w float, z component
   */
  Vec2D(float _x = 0, float _y = 0, float _w = 0) : x(_x), y(_y), w(_w) {}

  /**
   * Index operator, allowing us to access the individual x,y components of our
   * vector.
   * @param i the index
   * @return x,y components of our vector
   */
  float &operator[](int i) {
    // NOTE: Think about why this works.
    //       There is no code to change here.
    return ((&x)[i]);
  }

  /**
   * Index operator, allowing us to access the individual x,y components of our
   * vector.
   * @param i the index
   * @return x,y components of our vector
   */
  const float &operator[](int i) const {
    // NOTE: Think about why this works.
    //       There is no code to change here.
    return ((&x)[i]);
  }

  // General Note
  // You'll observer that some of the operator overloads appear in this class
  // as member functions versus some that are free functions.
  // In general, if you need access to private member variables or the 'this'
  // pointer then you must use a member function. Otherwise, I recommend to
  // 'free your functions' and not make them part of the class for maximum use.

  /**
   * Multiplication Operator
   * @param s a uniform-scalar
   * @return Vec2D, Multiplication result
   */
  Vec2D &operator*=(float s) {
    x *= s;
    y *= s;
    return (*this);
  }

  /**
   * Division Operator
   * @param s a uniform-scalar
   * @return Vec2D, Division result
   */
  Vec2D &operator/=(float s) {
    if (s == 0.f) s += (1e-6);
    x /= s;
    y /= s;
    return (*this);
  }

  /**
   * Addition operator
   * @param v a uniform-scalar
   * @return Vec2D, Addition result
   */
  Vec2D &operator+=(const Vec2D &v) {
    x += v.x;
    y += v.y;
    return (*this);
  }

  /**
   * Subtraction operator
   * @param v a uniform-scalar
   * @return Vec2D, Subtraction result
   */
  Vec2D &operator-=(const Vec2D &v) {
    x -= v.x;
    y -= v.y;
    return (*this);
  }

  float operator*(const Vec2D &v) const { return x * v.x + y * v.y; }

  friend std::ostream &operator<<(std::ostream &out, const Vec2D &a) {
    out << "x  : " << a.x << std::endl;
    out << "y  : " << a.y << std::endl;
    out << "(w): " << a.w << std::endl;
    return out;
  }

  /**
   * Dot product
   * @param b Vec2D to be dot product
   * @return scalar of dot product
   */
  inline float Dot(const Vec2D &b) { return *this * b; }

  // NOTE: Comparing floats is somewhat tricky, meaning that
  //       you will want to take the fabs(lhs.x - lhs.y) < 0.00001
  //       of each component to see if they are 'close enough'
  /**
   * equality
   * @param rhs Vec2D to be compared
   * @return true if they are equal, false otherwise
   */
  bool operator==(const Vec2D &rhs) {
    return fabs(x - rhs.x) < 1e-6 && fabs(y - rhs.y) < 1e-6;
  }

  /**
   * Multiplication of a vector by a scalar values
   * @param s a uniform-scalar
   * @return Vec2D, Multiplication result
   */
  inline Vec2D operator*(float s) { return {x * s, y * s, w}; }

  /**
   * Division of a vector by a scalar value.
   * @param s a uniform-scalar
   * @return Vec2D, Division result
   */
  inline Vec2D operator/(float s) const {
    if (s == 0.f) s += 1e-6;
    return {x / s, y / s, w};
  }

  /**
   * Get the magnitude of a vector
   * @return float, the magnitude of a vector
   */
  inline float Magnitude() { return sqrt(x * x + y * y); }

  /**
   * Add two vectors together
   * @param b Vec2D to be added
   * @return Vec2D, addition result
   */
  inline Vec2D operator+(const Vec2D &b) const {
    if (w == 1 && b.w == 1)
      throw std::invalid_argument("Two points can't be added.");
    return {x + b.x, y + b.y, fmax(w, b.w)};
  }

  /**
   * Subtract two vectors
   * @param b Vec2D to be added
   * @return Vec2D, subtraction result
   */
  inline Vec2D operator-(const Vec2D &b) const {
    if (w == 1 && b.w == 1)
      throw std::invalid_argument("Two points can't be subtracted.");
    return {x - b.x, y - b.y, fmax(w, b.w)};
  }

  /**
   * Vector Projection
   * @param b Vec2D to be projected
   * @return Projection Vec2D
   */
  inline Vec2D Project(Vec2D b) { return b * (this->Dot(b) / b.Dot(b)); }

  // Note: This is NOT generating a normal vector
  /**
   * Set a vectors magnitude to 1
   * @return Vec2D has magnitude of 1
   */
  inline Vec2D Normalize() { return *this / this->Magnitude(); }

  // a x b (read: 'a crossed b')
  // With a 3D vector we would yield another perpendicular cross product
  // For 2D cross product, this will yield a scalar.
  // You should write this to yield a scalar value.
  /**
   * Cross product of two vectors.
   * @param b Vec2D to be cross product
   * @return Vec2D, product result
   */
  inline float CrossProduct(const Vec2D &b) { return x * b.y - y * b.x; }
};

/**
 * Matrix 3D represents 3x3 matrices in Math
 */
struct Matrix3D {
 private:
  float n[3][3];  // Store each value of the matrix

 public:
  /**
   * Constructor of Matrix3D
   */
  Matrix3D() {
    n[0][0] = 1;
    n[0][1] = 0;
    n[0][2] = 0;
    n[1][0] = 0;
    n[1][1] = 1;
    n[1][2] = 0;
    n[2][0] = 0;
    n[2][1] = 0;
    n[2][2] = 1;
  }

  // Matrix constructor with 9 scalar values.
  /**
   * Constructor of Matrix3D
   */
  Matrix3D(float n00, float n01, float n02, float n10, float n11, float n12,
           float n20, float n21, float n22) {
    n[0][0] = n00;
    n[0][1] = n10;
    n[0][2] = n20;
    n[1][0] = n01;
    n[1][1] = n11;
    n[1][2] = n21;
    n[2][0] = n02;
    n[2][1] = n12;
    n[2][2] = n22;
  }

  // Matrix constructor from three vectors.
  /**
   * Constructor of Matrix3D
   */
  Matrix3D(const Vec2D &a, const Vec2D &b, const Vec2D &c) {
    n[0][0] = a.x;
    n[0][1] = a.y;
    n[0][2] = a.w;
    n[1][0] = b.x;
    n[1][1] = b.y;
    n[1][2] = b.w;
    n[2][0] = c.x;
    n[2][1] = c.y;
    n[2][2] = c.w;
  }

  // Example: M(1,1) returns row 1 and column 1 of matrix M.
  /**
   * Index operator with two dimensions.
   * @param i col number
   * @param j row number
   * @return float, target value
   */
  float &operator()(int i, int j) { return (n[j][i]); }

  // Index operator with two dimensions
  // Example: M(1,1) returns row 1 and column 1 of matrix M.
  /**
   * Index operator with two dimensions.
   * @param i col number
   * @param j row number
   * @return float, target value
   */
  const float &operator()(int i, int j) const { return (n[j][i]); }

  /**
   * Return a row from a matrix as a vector.
   * @param j jth row
   * @return a row from a matrix as a vector.
   */
  Vec2D &operator[](int j) { return (*reinterpret_cast<Vec2D *>(n[j])); }

  /**
   * Return a row from a matrix as a vector.
   * @param j jth row
   * @return a row from a matrix as a vector.
   */
  const Vec2D &operator[](int j) const {
    return (*reinterpret_cast<const Vec2D *>(n[j]));
  }
  friend std::ostream &operator<<(std::ostream &out, const Matrix3D &m) {
    out << "(" << m.n[0][0] << " , " << m.n[0][1] << " , " << m.n[0][2]
        << ")\n";
    out << "(" << m.n[1][0] << " , " << m.n[1][1] << " , " << m.n[1][2]
        << ")\n";
    out << "(" << m.n[2][0] << " , " << m.n[2][1] << " , " << m.n[2][2]
        << ")\n";
    return out;
  }

  /**
   * Check if two matrices are equal.
   * @param m matrix to be compared
   * @return true if they are equal, false otherwise
   */
  bool operator==(const Matrix3D &m) {
    for (int i = 0; i < 3; i++)
      for (int j = 0; j < 3; j++)
        if (fabs(n[i][j] - m[i][j]) > 1e-6) return false;
    return true;
  }

  /**
   * Matrix Multiplication
   * @param B matrix to be multiplied
   * @return Matrix3D, multiplication result
   */
  Matrix3D operator*(const Matrix3D &B) {
    Matrix3D result;
    for (int i = 0; i < 3; i++)
      for (int j = 0; j < 3; j++) {
        result[i][j] = 0;
        for (int k = 0; k < 3; k++) result[i][j] += this->n[i][k] * B[k][j];
      }
    return result;
  }

  /**
   * Matrix multiply by a vector
   * @param v Vec2D to be multiplied
   * @return Vec2D, multiplication result
   */
  Vec2D operator*(const Vec2D &v) {
    Vec2D res;
    for (int i = 0; i < 3; i++)
      for (int j = 0; j < 3; j++) res[i] += this->n[i][j] * v[j];
    return res;
  }
};

}  // namespace CSPill::Math

#endif
