#include "multi_index.h"
#include "ops.h"

namespace gradstudent {

Tensor operator+(const Tensor &left, const Tensor &right) {
  auto [x, y] = broadcast(left, right);
  Tensor result(x.shape());
  for (auto mIdx : MultiIndexIter(result.shape())) {
    result[mIdx] = x[mIdx] + y[mIdx];
  }
  return result;
}

Tensor operator*(const Tensor &left, const Tensor &right) {
  auto [x, y] = broadcast(left, right);
  Tensor result(x.shape());
  for (auto &resultIdx : MultiIndexIter(result.shape())) {
    result[resultIdx] = x[resultIdx] * y[resultIdx];
  }
  return result;
}

Tensor operator-(const Tensor &tensor) {
  Tensor result(tensor.shape());
  for (size_t i = 0; i < tensor.size(); ++i) {
    result[i] = -tensor[i];
  }
  return result;
}

Tensor operator-(const Tensor &left, const Tensor &right) {
  return left + (-right);
}

bool operator==(const Tensor &left, const Tensor &right) {
  checkCompatibleShape(left, right);
  for (auto &mIdx : MultiIndexIter(left.shape())) {
    if (left[mIdx] != right[mIdx]) {
      return false;
    }
  }

  return true;
}

} // namespace gradstudent