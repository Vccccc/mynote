#include <vector>

using namespace std;

struct PriorityQueueData
{
    typedef int Handle;
    PriorityQueueData(Handle handle = 0, int key = -1) : handle_(handle), key_(key) {  }
    Handle handle_;
    int key_;
    friend bool operator<(const PriorityQueueData& lhs, const PriorityQueueData& rhs)
    {
        return lhs.key_ < rhs.key_;
    }
    friend bool operator>(const PriorityQueueData& lhs, const PriorityQueueData& rhs)
    {
        return lhs.key_ > rhs.key_;
    }
};



class Algorithm
{
public:
    Algorithm() : dataCapacity_(0), priorityQueueCapacity_(0) {  }

    void printData();
    void printMaxPriorityQueue();

    // 堆的高度为 lg n， 时间复杂度为 O(lg n)
    // 最大堆的性质 A[PARENT(i)] >= A[i]
    void setHeapData(const vector<int>& data);
    void maxHeap() { buildMaxHeap(data_, data_.size()); }
    // 最大堆的性质 A[PARENT(i)] <= A[i]
    void minHeap();
    void sortByMaxHeap();
    void sortByMinHeap();

    // 优先队列(priority queue)
    void setPriorityQueueData(const vector<PriorityQueueData>& data);
    void maxPriorityQueueInsert(const PriorityQueueData& data);
    PriorityQueueData maxPriorityQueueMaximum();
    PriorityQueueData maxPriorityQueueExtractMax();
    void maxPriorityQueueIncreaseKey(int idx, int key);
private:
    template<typename Heap>
    void maxHeapify(Heap& heap, const int heapSize, const int idx);
    template<typename Heap>
    void buildMaxHeap(Heap& heap, int heapSize);
    void minHeapify(const int heapSize, const int idx);
    void buildMinHeap(const int heapSize);




private:
    vector<int> data_;
    int dataCapacity_;
    vector<PriorityQueueData> priorityQueue_;
    int priorityQueueCapacity_;




};


