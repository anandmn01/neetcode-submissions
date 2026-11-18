class Solution {
public:
    bool checkInclusion(string s1, string s2) {
        if(s1.size() > s2.size()) {
            return false;
        }

        vector<int> s1Freq(26, 0);
        vector<int> windowFreq(26, 0);

        int k = s1.size();

        // Count frequencies in s1
        for(char c : s1) {
            s1Freq[c - 'a']++;
        }

        // Create the first window
        for(int i = 0; i < k; i++) {
            windowFreq[s2[i] - 'a']++;
        }

        // Compare first window
        if(s1Freq == windowFreq) {
            return true;
        }

        // Slide the window
        for(int i = k; i < s2.size(); i++) {

            // Remove outgoing character
            windowFreq[s2[i - k] - 'a']--;

            // Add incoming character
            windowFreq[s2[i] - 'a']++;

            // Compare frequencies
            if(s1Freq == windowFreq) {
                return true;
            }
        }

        return false;
    }
};
