#include "Heap.h"

#include <stdexcept>
#include <utility>

namespace heap {

void SiftDown(DArray& arr, size_t i) {
  size_t left = 2 * i + 1;
  size_t right = 2 * i + 2;
  size_t largest = i;
  if (left < arr.Size() && arr[left] > arr[i])
    largest = left;
  if (right < arr.Size() && arr[right] > arr[largest])
    largest = right;

  if (largest != i) {
    std::swap(arr[i], arr[largest]);
    SiftDown(arr, largest);
  }
}

// SiftUp;
void SiftUp(DArray& arr, size_t i) {
  while (i > 0) {
    size_t parent = (i - 1) / 2;
    if (arr[i] <= arr[parent]) {
      return;
    }
    std::swap(arr[i], arr[parent]);
    i = parent;
  }
}

// http://en.cppreference.com/w/cpp/algorithm/make_heap
void MakeHeap(DArray& arr) {
  for (int i = arr.Size() / 2 - 1; i >= 0; --i) {
    SiftDown(arr, i);
  }
}

void Add(DArray& arr, double element) {
  arr.PushBack(element);
  SiftUp(arr, arr.Size() - 1);
}

double ExtractMax(DArray& arr) {
  if (arr.IsEmpty())
    throw std::runtime_error("head is empty");

  double res = arr[0];
  arr[0] = arr.PopBack();

  if (!arr.IsEmpty()) {
    SiftDown(arr, 0);
  }
  return res;
}

double HeapMaximum(DArray& arr) {
  if (arr.IsEmpty())
    throw std::runtime_error("head is empty");

  return arr[0];
}

}  // namespace heap
