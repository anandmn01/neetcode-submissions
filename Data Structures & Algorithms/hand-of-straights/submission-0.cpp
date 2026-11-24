class Solution {
public:
    bool isNStraightHand(vector<int>& hand, int groupSize) {
       int n = hand.size();

        // Total cards must be divisible by groupSize
        if (n % groupSize != 0) {
            return false;
        }

        // Store card frequencies in sorted order
        map<int, int> freq;

        for (int card : hand) {
            freq[card]++;
        }

        // Process cards until the map becomes empty
        while (!freq.empty()) {

            // Smallest available card
            int start = freq.begin()->first;

            // Number of groups starting with this card
            int count = freq.begin()->second;

            // Form consecutive groups
            for (int i = 0; i < groupSize; i++) {

                int card = start + i;

                // Required card must exist
                if (freq.find(card) == freq.end()) {
                    return false;
                }

                // Required card must have enough frequency
                if (freq[card] < count) {
                    return false;
                }

                // Consume the required cards
                freq[card] -= count;

                // Remove card when frequency becomes zero
                if (freq[card] == 0) {
                    freq.erase(card);
                }
            }
        }

        return true; 
    }
};
