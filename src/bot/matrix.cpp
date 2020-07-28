#include "bot/matrix.h"
#include "bot/exceptions.h"

namespace prep {
Matrix Matrix::outColRow(std::size_t row, std::size_t column) const {
    std::size_t rows = getRows();
    std::size_t cols = getCols();
    if (cols == 1 && rows == 1) {
        Matrix new_matrix(1, 1);
        new_matrix(0, 0) = 1;
        return new_matrix;
    }

    Matrix new_matrix(rows - 1, cols - 1);
    std::size_t new_rows = new_matrix.getRows();
    std::size_t new_cols = new_matrix.getCols();

    std::size_t row_flag = 0;

    for (std::size_t i = 0; i < new_rows; i++) {
        if (i == row) {
            row_flag = 1;
        }

        std::size_t column_flag = 0;
        for (std::size_t j = 0; j < new_cols; j++) {
            if (j == column) {
                column_flag = 1;
            }
            new_matrix(i, j) = items_[i + row_flag][j + column_flag];
        }
    }

    return new_matrix;
}

Matrix::Matrix(std::size_t rows, std::size_t cols) {
    items_.resize(rows);
    for (auto& item : items_) {
        item.resize(cols);
    }
}

Matrix::Matrix(std::size_t rows, std::size_t cols, float base_value) {
    items_.resize(rows);
    for (auto& item : items_) {
        item.resize(cols, base_value);
    }
}

Matrix::Matrix(std::istream& is) {
    size_t rows = 0, cols = 0;

    if (!is || !(is >> rows >> cols)
            || rows < 1 || cols < 1) {
        throw InvalidMatrixStream();
    }

    items_.resize(rows);
    float item = 0;
    for (size_t r = 0; r < rows; r++) {
        items_[r].resize(cols);
        for (size_t c = 0; c < cols; c++) {
            if (!(is >> item)) {
                throw InvalidMatrixStream();
            }
            items_[r][c] = item;
        }
    }
}

Matrix::Matrix(const std::vector<std::string>& input_vector) {
    std::size_t cols = input_vector.size();
    items_.resize(1);
    items_[0].resize(cols);
    for (std::size_t i = 0; i < cols; i++) {
        items_[0][i] = static_cast<float >(std::atof(input_vector[i].c_str()));
    }
}

Matrix::Matrix(const std::vector<float>& input_vector, bool column) {
    if (column) {
        std::size_t rows = input_vector.size();
        items_.resize(rows);
        for (std::size_t i = 0; i < rows; i++) {
            items_[i].push_back(input_vector[i]);
        }
    } else {
        std::size_t cols = input_vector.size();
        items_.resize(1);
        items_[0].resize(cols);
        for (std::size_t i = 0; i < cols; i++) {
            items_[0][i] = input_vector[i];
        }
    }
}

Matrix::Matrix(std::size_t rows, std::size_t cols, float mean, float scale) {
    std::random_device rd{};
    std::mt19937 gen{rd()};
    // values near the mean are the most likely
    // standard deviation affects the dispersion of generated values from the mean
    std::normal_distribution<> d{mean, scale};
    items_.resize(rows);
    for (std::size_t i = 0; i < rows; i++) {
        items_[i].resize(cols);
        for (std::size_t j = 0; j < cols; j++) {
            items_[i][j] = static_cast<float >(d(gen));
        }
    }
}

std::size_t Matrix::getRows() const {
    return items_.size();
}

std::size_t Matrix::getCols() const {
    return items_.size() == 0 ? 0 : items_[0].size();
}

std::pair<std::size_t, std::size_t> Matrix::getSize() const {
    std::size_t rows = getRows();
    std::size_t cols = getCols();
    return {rows, cols};
}

float Matrix::operator()(std::size_t i, std::size_t j) const {
    if (i >= getRows() || j >= getCols()) {
        throw OutOfRange(i, j, *this);
    }

    return items_[i][j];
}

float& Matrix::operator()(std::size_t i, std::size_t j) {
    if (i >= getRows() || j >= getCols()) {
        throw OutOfRange(i, j, *this);
    }
    return items_[i][j];
}

bool Matrix::operator==(const Matrix& rhs) const {
    float eps = 1e-07f;

    std::size_t rows = getRows();
    std::size_t cols = getCols();

    if (rows != rhs.getRows() || cols != rhs.getCols()) {
        return false;
    }

    for (std::size_t i = 0; i < rows; i++) {
        for (std::size_t j = 0; j < cols; j++) {
            float diff = items_[i][j] - rhs(i, j);
            if (diff > eps || diff < -eps) {
                return false;
            }
        }
    }

    return true;
}

bool Matrix::operator!=(const Matrix& rhs) const {
    return !(*this == rhs);
}

Matrix Matrix::operator+(const Matrix& rhs) const {
    std::size_t rows = getRows();
    std::size_t cols = getCols();

    if (rows != rhs.getRows() || cols != rhs.getCols()) {
        throw DimensionMismatch(*this, rhs);
    }
    Matrix new_matrix(rows, cols);
    for (std::size_t i = 0; i < rows; i++) {
        for (std::size_t j = 0; j < cols; j++) {
            new_matrix(i, j) = items_[i][j] + rhs(i, j);
        }
    }
    return new_matrix;
}

Matrix Matrix::operator-(const Matrix& rhs) const {
    std::size_t rows = getRows();
    std::size_t cols = getCols();
    if (rows != rhs.getRows() || cols != rhs.getCols()) {
        throw DimensionMismatch(*this, rhs);
    }
    Matrix new_matrix(rows, cols);
    for (std::size_t i = 0; i < rows; i++) {
        for (std::size_t j = 0; j < cols; j++) {
            new_matrix(i, j) = items_[i][j] - rhs(i, j);
        }
    }
    return new_matrix;
}

Matrix Matrix::operator*(const Matrix& rhs) const {
    std::size_t rows = getRows();
    std::size_t cols = getCols();

    std::size_t rhs_cols = rhs.getCols();
    std::size_t rhs_rows = rhs.getRows();

    if (cols != rhs_rows) {
        throw DimensionMismatch(*this, rhs);
    }
    Matrix new_matrix(rows, rhs_cols);
    for (std::size_t i = 0; i < rows; i++) {
        for (std::size_t j = 0; j < rhs_cols; j++) {
            float sum = 0;
            for (std::size_t k = 0; k < cols; k++) {
                sum += items_[i][k] * rhs(k, j);
            }
            new_matrix(i, j) = sum;
        }
    }
    return new_matrix;
}

Matrix Matrix::transp() const {
    std::size_t rows = getRows();
    std::size_t cols = getCols();

    Matrix new_matrix(cols, rows);
    for (std::size_t i = 0; i < cols; i++) {
        for (std::size_t j = 0; j < rows; j++) {
            new_matrix(i, j) = items_[j][i];
        }
    }
    return new_matrix;
}

Matrix Matrix::operator*(float val) const {
    std::size_t rows = getRows();
    std::size_t cols = getCols();

    Matrix new_matrix(rows, cols);
    for (std::size_t i = 0; i < rows; i++) {
        for (std::size_t j = 0; j < cols; j++) {
            new_matrix(i, j) = val * items_[i][j];
        }
    }
    return new_matrix;
}

Matrix Matrix::operator%(const Matrix& rhs) const {
    std::size_t rows = getRows();
    std::size_t cols = getCols();

    if (cols != rhs.getCols() && rows != rhs.getRows()) {
        throw DimensionMismatch(*this, rhs);
    }

    Matrix new_matrix(rows, cols);
    for (std::size_t i = 0; i < rows; i++) {
        for (std::size_t j = 0; j < cols; j++) {
            new_matrix(i, j) = items_[i][j] * rhs(i, j);
        }
    }

    return new_matrix;
}

float Matrix::det() const {
    std::size_t rows = getRows();
    std::size_t cols = getCols();

    if (rows != cols) {
        throw DimensionMismatch(*this);
    }
    if (rows == 1) {
        return items_[0][0];
    }
    float result = 0.f;
    float val = 0.f;
    for (std::size_t i = 0; i < rows; i++) {
        Matrix new_matrix = outColRow(0, i);
        val = new_matrix.det();
        result += items_[0][i] * static_cast<float >(std::pow(-1, 2 + i)) * (val);
    }
    return result;
}

Matrix Matrix::adj() const {
    std::size_t rows = getRows();
    std::size_t cols = getCols();

    Matrix new_matrix(cols, rows);
    float val = 0;
    for (std::size_t i = 0; i < cols; i++) {
        for (std::size_t j = 0; j < rows; j++) {
            Matrix super_new_matrix = outColRow(i, j);
            val = super_new_matrix.det();
            new_matrix(j, i) = val * static_cast<float >(std::pow(-1, 2 + i + j));
        }
    }
    return new_matrix;
}

Matrix Matrix::inv() const {
    std::size_t rows = getRows();
    std::size_t cols = getCols();

    float val = det();
    if (val == 0) {
        throw SingularMatrix();
    }
    Matrix new_matrix = adj();
    for (std::size_t i = 0; i < rows; i++) {
        for (std::size_t j = 0; j < cols; j++) {
            new_matrix(i, j) *= 1 / val;
        }
    }
    return new_matrix;
}

std::pair<std::size_t, float> Matrix::findMaxArg() const {
    std::size_t rows = getRows();
    std::size_t cols = getCols();

    std::size_t max_index = 0;
    float max = __FLT_MIN__;

    for (std::size_t i = 0; i < rows; i++) {
        for (std::size_t j = 0; j < cols; j++) {
            if (max < items_[i][j]) {
                max = items_[i][j];
                max_index = i;
            }
        }
    }

    return {max_index, max};
}

Matrix operator*(float val, const Matrix& matrix) {
    std::size_t rows = matrix.getRows();
    std::size_t cols = matrix.getCols();

    Matrix new_matrix(rows, cols);
    for (std::size_t i = 0; i < rows; i++) {
        for (std::size_t j = 0; j < cols; j++) {
            new_matrix(i, j) = val * matrix(i, j);
        }
    }
    return new_matrix;
}

std::ostream& operator<<(std::ostream& os, const Matrix& matrix) {
    std::size_t rows = matrix.getRows();
    std::size_t cols = matrix.getCols();

    os << std::setprecision(std::numeric_limits<float>::max_digits10);

    os << rows << " " << cols << std::endl;
    for (std::size_t i = 0; i < rows; i++) {
        for (std::size_t j = 0; j < cols; j++) {
            if (j != 0) {
                os << " ";
            }
            os << matrix(i, j);
        }
        os << std::endl;
    }

    return os;
}
}  // namespace prep
