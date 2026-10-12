#include "MaxHeap.h"

MaxHeap::MaxHeap() {
    size = 0;
}

void MaxHeap::heapifyUp(int index) {
    while (index > 0) {
        int parent = (index - 1) / 2;

        if (heap[parent].score >= heap[index].score) {
            break;
        }

        // Swap
        RankedResult temp = heap[parent];
        heap[parent] = heap[index];
        heap[index] = temp;

        index = parent;
    }
}

void MaxHeap::heapifyDown(int index) {
    while (true) {
        int largest = index;
        int left = 2 * index + 1;
        int right = 2 * index + 2;

        if (left < size && heap[left].score > heap[largest].score) {
            largest = left;
        }

        if (right < size && heap[right].score > heap[largest].score) {
            largest = right;
        }

        if (largest == index) {
            break;
        }

        // Swap
        RankedResult temp = heap[index];
        heap[index] = heap[largest];
        heap[largest] = temp;

        index = largest;
    }
}

void MaxHeap::push(const RankedResult& item) {
    if (size >= 1000) {
        return; // Heap is full
    }

    heap[size] = item;
    heapifyUp(size);
    size++;
}

RankedResult MaxHeap::top() const {
    if (size == 0) {
        return RankedResult(); // Return empty result if heap is empty
    }
    return heap[0];
}

void MaxHeap::pop() {
    if (size == 0) return;

    heap[0] = heap[size - 1];
    size--;

    if (size > 0) {
        heapifyDown(0);
    }
}

bool MaxHeap::empty() const {
    return size == 0;
}

int MaxHeap::getSize() const {
    return size;
}

void MaxHeap::clear() {
    size = 0;
}
