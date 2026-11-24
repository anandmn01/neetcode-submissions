/**
 * Definition of Interval:
 * class Interval {
 * public:
 *     int start, end;
 *     Interval(int start, int end) {
 *         this->start = start;
 *         this->end = end;
 *     }
 * }
 */

class Solution {
public:
    int minMeetingRooms(vector<Interval>& intervals) {
        // Sort meetings by starting time
        sort(intervals.begin(), intervals.end(),
             [](Interval& a, Interval& b) {
                 return a.start < b.start;
             });

        // Min Heap stores meeting ending times
        priority_queue<int, vector<int>, greater<int>> minHeap;

        // Process each meeting
        for (auto& meeting : intervals) {

            // Reuse a room if the earliest meeting has finished
            if (!minHeap.empty() &&
                minHeap.top() <= meeting.start) {

                minHeap.pop();
            }

            // Allocate the current meeting
            minHeap.push(meeting.end);
        }

        // Number of rooms required
        return minHeap.size();
    }
};
