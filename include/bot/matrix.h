#pragma once

#include <cmath>
#include <iomanip>
#include <istream>
#include <cstdlib>
#include <ctime>
#include <limits>
#include <vector>
#include <random>
#include <string>
#include <utility>

namespace prep {
class Matrix {
 private:
    std::vector<std::vector<float>> items_;
    Matrix outColRow(std::size_t row, std::size_t column) const;

 public:
    explicit Matrix(std::size_t rows = 0, std::size_t cols = 0);
    explicit Matrix(std::size_t rows, std::size_t cols, float base_value);
    explicit Matrix(std::istream& is);
    explicit Matrix(const std::vector<std::string>& input_vector);
    explicit Matrix(const std::vector<float>& input_vector, bool column);
    explicit Matrix(std::size_t rows, std::size_t cols, float start, float stop);


    Matrix(const Matrix& rhs) = default;
    Matrix& operator=(const Matrix& rhs) = default;
    ~Matrix() = default;

    std::size_t getRows() const;
    std::size_t getCols() const;
    std::pair<std::size_t, std::size_t> getSize() const;

    float operator()(std::size_t i, std::size_t j) const;
    float& operator()(std::size_t i, std::size_t j);

    bool operator==(const Matrix& rhs) const;
    bool operator!=(const Matrix& rhs) const;

    Matrix operator+(const Matrix& rhs) const;
    Matrix operator-(const Matrix& rhs) const;
    Matrix operator*(const Matrix& rhs) const;
    Matrix operator%(const Matrix& rhs) const;

    Matrix operator*(float val) const;

    friend Matrix operator*(float val, const Matrix& matrix);
    friend std::ostream& operator<<(std::ostream& os, const Matrix& matrix);

    Matrix transp() const;
    float det() const;
    Matrix adj() const;
    Matrix inv() const;

    std::pair<std::size_t, float> findMaxArg() const;
};

Matrix operator*(float val, const Matrix& matrix);
std::ostream& operator<<(std::ostream& os, const Matrix& matrix);
}  // namespace prep
