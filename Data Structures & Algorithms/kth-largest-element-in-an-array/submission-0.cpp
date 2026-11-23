class Solution {
public:
    int findKthLargest(vector<int>& nums, int k) {
        // Create Min Heap
                priority_queue<int, vector<int>, greater<int>> pq;

                        // Process every element
                                for (int num : nums) {

                                            pq.push(num);

                                                        // Maintain heap size K
                                                                    if (pq.size() > k) {
                                                                                    pq.pop();
                                                                                                }
                                                                                                        }

                                                                                                                // Top element is the Kth largest
                                                                                                                        return pq.top();
    }
};
