class Solution {
public:
    int ladderLength(string beginWord,
                     string endWord,
                     vector<string>& wordList) {

        unordered_set<string> st(
            wordList.begin(),
            wordList.end()
        );

        // Target must exist in the dictionary
        if (!st.count(endWord)) {
            return 0;
        }

        queue<pair<string, int>> q;

        q.push({beginWord, 1});

        while (!q.empty()) {

            auto [word, steps] = q.front();
            q.pop();

            // Try changing every character
            for (int i = 0; i < word.size(); i++) {

                char original = word[i];

                // Try all lowercase letters
                for (char ch = 'a'; ch <= 'z'; ch++) {

                    word[i] = ch;

                    // Check whether the generated word exists
                    if (st.count(word)) {

                        // Target found
                        if (word == endWord) {
                            return steps + 1;
                        }

                        // Add to BFS queue
                        q.push({word, steps + 1});

                        // Mark visited
                        st.erase(word);
                    }
                }

                // Restore original character
                word[i] = original;
            }
        }

        return 0;
        
    }
};
