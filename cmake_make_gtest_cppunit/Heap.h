#ifndef LECTURE2_HEAP_H_
#define LECTURE2_HEAP_H_

#include "DArray.h"

namespace heap {

// MaxHeapify or just Heapify
void SiftDown(DArray& arr, size_t i);

// MinHeapify or SiftDown or just Heapify
void SiftUp(DArray& arr, size_t i);

void MakeHeap(DArray& arr);

void Add(DArray& arr, double element);

// std::pop_heap;  http://en.cppreference.com/w/cpp/algorithm/pop_heap
double ExtractMax(DArray& arr);

double HeapMaximum(DArray& arr);

}  // namespace heap

#endif  // LECTURE2_HEAP_H_
