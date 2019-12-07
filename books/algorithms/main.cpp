#include "algorithms.h"

#include <iostream>

using namespace std;
void testHeap()
{
    Algorithm instance;
    instance.setHeapData({8, 7, 9, 3, 2, 4, 116, 14, 10});
    cout << "before maxHeap: ";
    instance.printData();

    instance.maxHeap();
    cout << "after maxHeap: ";
    instance.printData();

    cout << "before sortByMaxHeap: ";
    instance.printData();

    instance.sortByMaxHeap();
    cout << "after sortByMaxHeap: ";
    instance.printData();

    cout << "before sortByMinHeap: ";
    instance.printData();

    instance.sortByMinHeap();
    cout << "after sortByMinHeap: ";
    instance.printData();

    vector<PriorityQueueData> priorityQueueData;
    priorityQueueData.reserve(10);
    srand(time(NULL));
    for(int i = 0; i < 10; i++)
    {
        priorityQueueData.push_back(PriorityQueueData(i, rand() % 64));
    }

    instance.setPriorityQueueData(priorityQueueData);
    cout << "after build max priority queue" << endl;
    instance.printMaxPriorityQueue();

    cout << "max priority queue maximum key: " << endl;
    auto maximum = instance.maxPriorityQueueMaximum();
    cout << maximum.key_ << endl;

    cout << "after extract maximum key: " << endl;
    instance.maxPriorityQueueExtractMax();
    instance.printMaxPriorityQueue();

    cout << "insert a new data {1, 100}" << endl;
    instance.maxPriorityQueueInsert(PriorityQueueData(1, 100));
    instance.printMaxPriorityQueue();

    cout << "increase idx 3 with key 250" << endl;
    instance.maxPriorityQueueIncreaseKey(3, 250);
    instance.printMaxPriorityQueue();
}


void test()
{
    testHeap();
}

int main()
{
    test();
}
