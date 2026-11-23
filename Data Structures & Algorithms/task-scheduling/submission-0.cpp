class Solution {
public:
    int leastInterval(vector<char>& tasks, int n) {
        vector<int> freq(26, 0);

                // Count task frequencies
                        for (char task : tasks) {
                                    freq[task - 'A']++;
                                            }

                                                    // Max Heap
                                                            priority_queue<int> maxHeap;

                                                                    for (int count : freq) {
                                                                                if (count > 0) {
                                                                                                maxHeap.push(count);
                                                                                                            }
                                                                                                                    }

                                                                                                                            // {remaining frequency, available time}
                                                                                                                                    queue<pair<int, int>> cooldown;

                                                                                                                                            int time = 0;

                                                                                                                                                    while (!maxHeap.empty() || !cooldown.empty()) {

                                                                                                                                                                time++;

                                                                                                                                                                            // Check whether a task has completed cooldown
                                                                                                                                                                                        if (!cooldown.empty() &&
                                                                                                                                                                                                        cooldown.front().second <= time) {

                                                                                                                                                                                                                        maxHeap.push(cooldown.front().first);
                                                                                                                                                                                                                                        cooldown.pop();
                                                                                                                                                                                                                                                    }

                                                                                                                                                                                                                                                                // Execute the task with highest frequency
                                                                                                                                                                                                                                                                            if (!maxHeap.empty()) {

                                                                                                                                                                                                                                                                                            int count = maxHeap.top();
                                                                                                                                                                                                                                                                                                            maxHeap.pop();

                                                                                                                                                                                                                                                                                                                            count--;

                                                                                                                                                                                                                                                                                                                                            // If task still has remaining executions
                                                                                                                                                                                                                                                                                                                                                            if (count > 0) {

                                                                                                                                                                                                                                                                                                                                                                                cooldown.push({
                                                                                                                                                                                                                                                                                                                                                                                                        count,
                                                                                                                                                                                                                                                                                                                                                                                                                                time + n + 1
                                                                                                                                                                                                                                                                                                                                                                                                                                                    });
                                                                                                                                                                                                                                                                                                                                                                                                                                                                    }
                                                                                                                                                                                                                                                                                                                                                                                                                                                                                }
                                                                                                                                                                                                                                                                                                                                                                                                                                                                                        }

                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                return time;
    }
};
