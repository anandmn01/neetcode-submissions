class Solution {
public:
    vector<int> partitionLabels(string s) {
         vector<int> last(26, 0);

        // Store the last occurrence of each character
        for (int i = 0; i < s.size(); i++) {
            last[s[i] - 'a'] = i;
        }

        vector<int> result;

        int start = 0;
        int end = 0;

        // Traverse the string
        for (int i = 0; i < s.size(); i++) {

            // Extend the current partition
            end = max(end, last[s[i] - 'a']);

            // Current partition is complete
            if (i == end) {

                result.push_back(end - start + 1);

                start = i + 1;
            }
        }

        return result;
    }
};
