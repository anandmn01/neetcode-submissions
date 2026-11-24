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
    bool canAttendMeetings(vector<Interval>& intervals) {
        // Sort meetings by starting time
        sort(intervals.begin(), intervals.end(),
             [](Interval& a, Interval& b) {
                 return a.start < b.start;
             });

        // Check adjacent meetings
        for (int i = 1; i < intervals.size(); i++) {

            // Overlapping meetings
            if (intervals[i].start < intervals[i - 1].end) {
                return false;
            }
        }

        // No scheduling conflicts
        return true;
    }
};
