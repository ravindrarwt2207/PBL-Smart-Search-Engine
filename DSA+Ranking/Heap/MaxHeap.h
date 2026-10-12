#ifndef MAXHEAP_H
#define MAXHEAP_H

#include "../Common/Common.h"

class MaxHeap {
private:
    RankedResult heap[1000];   // Fixed size array (no vector)
    int size;

    void heapifyUp(int index);
    void heapifyDown(int index);

public:
    MaxHeap();

    void push(const RankedResult& item);
    RankedResult top() const;
    void pop();
    bool empty() const;
    int getSize() const;
    void clear();
};

#endif
