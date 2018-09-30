#ifndef LECTURE2_DARRAY_H_
#define LECTURE2_DARRAY_H_

#include <cstddef>

class DArray {
 public:
  explicit DArray(size_t buffer_size);
  virtual ~DArray();
  virtual void PushBack(double value);
  double PopBack();

  double At(int index);

  double& operator[](size_t index);
  const double& operator[](size_t index) const;

  virtual void Display();
  size_t Size() const { return actual_size_; }
  size_t Capacity() const { return buf_size_; }
  bool IsEmpty() const { return actual_size_ == 0; }

 private:
  void Reallocate();
  void DtorHelper();

  double* buffer_;
  size_t buf_size_;
  size_t actual_size_;
};

#endif  // LECTURE2_DARRAY_H_