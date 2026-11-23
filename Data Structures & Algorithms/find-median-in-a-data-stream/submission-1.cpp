class MedianFinder {
private:

    // Stores the smaller half
        priority_queue<int> maxHeap;

            // Stores the larger half
                priority_queue<int, vector<int>, greater<int>> minHeap;

                public:

                    MedianFinder() {
                        }

                            void addNum(int num) {

                                    // Step 1: Insert into Max Heap
                                            maxHeap.push(num);

                                                    // Step 2: Move largest element of smaller half
                                                            // into the larger half
                                                                    minHeap.push(maxHeap.top());
                                                                            maxHeap.pop();

                                                                                    // Step 3: Balance the heap sizes
                                                                                            if (minHeap.size() > maxHeap.size()) {

                                                                                                        maxHeap.push(minHeap.top());
                                                                                                                    minHeap.pop();
                                                                                                                            }
                                                                                                                                }

                                                                                                                                    double findMedian() {

                                                                                                                                            // Odd number of elements
                                                                                                                                                    if (maxHeap.size() > minHeap.size()) {
                                                                                                                                                                return maxHeap.top();
                                                                                                                                                                        }

                                                                                                                                                                                // Even number of elements
                                                                                                                                                                                        return ((double)maxHeap.top() + minHeap.top()) / 2.0;
        
    }
};
