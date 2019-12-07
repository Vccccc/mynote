#include "algorithms.h"

#include <iostream>
using namespace std;



#define PARENT(idx) (((idx)+1) / 2 - 1)
#define LEFT(idx) (((idx)+1) * 2 - 1)
#define RIGHT(idx) (((idx)+1) * 2 )

void Algorithm::printData()
{
    for(auto elem : data_)
    {
        cout << elem << ", ";
    }
    cout << endl;
}

void Algorithm::printMaxPriorityQueue()
{
    for(int idx = 0; idx < priorityQueueCapacity_; idx++)
    {
        const auto& data = priorityQueue_[idx];
        cout << "{ handle: " << data.handle_ << ", key: " << data.key_ << " }, ";
    }
    cout << endl;
}
void Algorithm::setHeapData(const vector<int>& data)
{
    data_ = data; 
    dataCapacity_ = data_.size();
}

template<typename Heap>
void Algorithm::maxHeapify(Heap& heap, const int heapSize, const int idx)
{
    int left = LEFT(idx);
    int right = RIGHT(idx);
    int largest = idx;
    if(left < heapSize && heap[left] > heap[largest])
    {
        largest = left;
    }

    if(right < heapSize && heap[right] > heap[largest])
    {
        largest = right;
    }

    if(largest != idx)
    {
        swap(heap[largest], heap[idx]);
        maxHeapify(heap, heapSize, largest);
    }
}

template<typename Heap>
void Algorithm::buildMaxHeap(Heap& heap, int heapSize)
{
    // 遍历所有父节点，使得堆性质满足 A[parent(i)] >= A[i]
    for(int idx = PARENT(heapSize - 1); idx >= 0; idx--) 
    {
        maxHeapify(heap, heapSize, idx);
    }
}

void Algorithm::sortByMaxHeap()
{
    for(size_t heapSize = data_.size(); heapSize > 0; )
    {
        buildMaxHeap(data_, heapSize);
        swap(data_[0], data_[--heapSize]);
    }
}

void Algorithm::minHeapify(const int heapSize, const int idx)
{
    int left = LEFT(idx);
    int right = RIGHT(idx);
    int smallest = idx;
    if(left < heapSize && data_[left] < data_[smallest])
    {
        smallest = left;
    }

    if(right < heapSize && data_[right] < data_[smallest])
    {
        smallest = right;
    }

    if(smallest != idx)
    {
        swap(data_[smallest], data_[idx]);
        minHeapify(heapSize - 1, smallest);
    }
}

void Algorithm::buildMinHeap(const int heapSize)
{
    // 遍历所有父节点，使得堆性质满足 A[parent(i)] <= A[i]
    for(int idx = PARENT(heapSize - 1); idx >= 0; idx--) 
    {
        minHeapify(heapSize, idx);
    }
}

void Algorithm::sortByMinHeap()
{
    for(size_t heapSize = data_.size(); heapSize > 0; )
    {
        buildMinHeap(heapSize);
        swap(data_[0], data_[--heapSize]);
    }
}

void Algorithm::setPriorityQueueData(const vector<PriorityQueueData>& data)
{
    priorityQueue_ = data; 
    priorityQueueCapacity_ = priorityQueue_.size();
    buildMaxHeap(priorityQueue_, priorityQueueCapacity_);
}

void Algorithm::maxPriorityQueueInsert(const PriorityQueueData& data)
{
    priorityQueueCapacity_++;
    if(priorityQueueCapacity_ > priorityQueue_.size())
    {
        priorityQueue_.push_back(data);
    }
    else
    {
        priorityQueue_[priorityQueueCapacity_] = data;
    }
    maxPriorityQueueIncreaseKey(priorityQueueCapacity_ - 1, data.key_);
}

PriorityQueueData Algorithm::maxPriorityQueueMaximum()
{
    if(priorityQueueCapacity_ != 0)
    {
        return priorityQueue_.at(0);
    }
    return PriorityQueueData();
}

PriorityQueueData Algorithm::maxPriorityQueueExtractMax()
{
    if(priorityQueueCapacity_ == 0)
    {
        return PriorityQueueData();
    }
    auto result = priorityQueue_.at(0);
    swap(priorityQueue_[0], priorityQueue_[--priorityQueueCapacity_]);
    buildMaxHeap(priorityQueue_, priorityQueueCapacity_);
    return result;
}

void Algorithm::maxPriorityQueueIncreaseKey(int idx, int key)
{
    if(idx < 0 || idx >= priorityQueueCapacity_)
    {
        cout << "invalid idx" << idx << endl;
        return;
    }

    if(key < priorityQueue_[idx].key_)
    {
        cout << "new key is smaller than current key" << endl;
        return ;
    }
    priorityQueue_[idx].key_ = key;
    while(idx > 0 && priorityQueue_[PARENT(idx)] < priorityQueue_[idx])
    {
        swap(priorityQueue_[PARENT(idx)], priorityQueue_[idx]);
        idx = PARENT(idx);
    }
}
