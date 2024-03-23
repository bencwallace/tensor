#include <numeric>
#include <sstream>

#include "ops.h"

namespace gradstudent {

Tensor dot(const Tensor &left, const Tensor &right) {
  const array_t &left_shape = left.shape();
  const array_t &right_shape = right.shape();

  if (left_shape[left_shape.size() - 1] != right_shape[0]) {
    std::ostringstream ss;
    ss << "Incompatible shapes: " << left_shape << " and " << right_shape;
    throw std::invalid_argument(ss.str());
  }

  array_t result_shape(left_shape.size() + right_shape.size() - 2);
  for (size_t i = 0; i < left_shape.size() - 1; ++i) {
    result_shape[i] = left_shape[i];
  }
  for (size_t i = 1; i < right_shape.size(); ++i) {
    result_shape[left_shape.size() + i - 2] = right_shape[i];
  }

  Tensor result(result_shape);
  const array_t &left_strides = left.strides();
  const array_t &right_strides = right.strides();

  size_t i = 0;
  for (auto &resultMultiIdx : MultiIndexIter(result.shape())) {
    size_t thisIndex = std::inner_product(
        resultMultiIdx.begin(), resultMultiIdx.end() - right.ndims() + 1,
        left_strides.begin(), 0);
    size_t otherIndex = std::inner_product(
        resultMultiIdx.rbegin(), resultMultiIdx.rend() - left.ndims() + 1,
        right_strides.rbegin(), 0);

    result[i] = 0;
    for (size_t j = 0; j < left.shape()[left.ndims() - 1]; ++j) {
      result[i] += left[thisIndex] * right[otherIndex];
      thisIndex += left_strides[left.ndims() - 1];
      otherIndex += right_strides[0];
    }

    ++i;
  }
  return result;
}

Tensor norm2(const Tensor &tensor) {
  Tensor flat = flatten(tensor);
  return dot(flat, flat);
}

} // namespace gradstudent
