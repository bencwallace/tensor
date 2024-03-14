#pragma once

#include <initializer_list>
#include <memory>

#include "array.h"
#include "multi_index.h"
#include "tensor_data.h"

class Tensor {

private:
  size_t offset_;
  size_t size_;
  Array shape_;
  Array strides_;
  std::shared_ptr<TensorData> data_;

  size_t toIndex(const Array &) const;
  Array toMultiIndex(size_t) const;
  void checkCompatibleShape(const Tensor &) const;

public:
  Tensor(const Tensor &);
  explicit Tensor(const Array &shape, const Array &strides, const Tensor &,
                  size_t offset = 0);

  Tensor(const Array &shape);

  Tensor(const Array &shape, const Array &strides,
         std::initializer_list<double> data);
  Tensor(const Array &shape, std::initializer_list<double> data);
  Tensor(double);

  Tensor &operator=(const Tensor &);
  double operator[](size_t) const;
  double &operator[](size_t);
  double operator[](const Array &) const;
  double &operator[](const Array &);
  inline double operator[](const MultiIndex &mIdx) const {
    return (*this)[mIdx.data()];
  }
  inline double &operator[](const MultiIndex &mIdx) {
    return (*this)[mIdx.data()];
  }
  Tensor operator+(const Tensor &) const;
  Tensor operator-() const;
  Tensor operator-(const Tensor &) const;
  Tensor operator*(const Tensor &) const;
  bool operator==(const Tensor &) const;
  explicit operator double() const;

  MultiIndexIter multiIndexRange() const;

  Tensor slice(const Array &mIdx);

  size_t size() const;
  size_t ndims() const;
  const Array &shape() const;
  const Array &strides() const;

  friend Tensor operator*(double, const Tensor &);
};

Tensor operator*(double, const Tensor &);
