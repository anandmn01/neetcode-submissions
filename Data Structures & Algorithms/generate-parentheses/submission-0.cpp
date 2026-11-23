class Solution {
private:

    void backtrack(
            int open,
                    int close,
                            int n,
                                    string& current,
                                            vector<string>& result
                                                ) {

                                                        // A complete valid combination is formed.
                                                                if (current.size() == 2 * n) {
                                                                            result.push_back(current);
                                                                                        return;
                                                                                                }

                                                                                                        // Choice 1: Add an opening parenthesis.
                                                                                                                if (open < n) {

                                                                                                                            current.push_back('(');

                                                                                                                                        backtrack(
                                                                                                                                                        open + 1,
                                                                                                                                                                        close,
                                                                                                                                                                                        n,
                                                                                                                                                                                                        current,
                                                                                                                                                                                                                        result
                                                                                                                                                                                                                                    );

                                                                                                                                                                                                                                                // Backtrack.
                                                                                                                                                                                                                                                            current.pop_back();
                                                                                                                                                                                                                                                                    }

                                                                                                                                                                                                                                                                            // Choice 2: Add a closing parenthesis.
                                                                                                                                                                                                                                                                                    if (close < open) {

                                                                                                                                                                                                                                                                                                current.push_back(')');

                                                                                                                                                                                                                                                                                                            backtrack(
                                                                                                                                                                                                                                                                                                                            open,
                                                                                                                                                                                                                                                                                                                                            close + 1,
                                                                                                                                                                                                                                                                                                                                                            n,
                                                                                                                                                                                                                                                                                                                                                                            current,
                                                                                                                                                                                                                                                                                                                                                                                            result
                                                                                                                                                                                                                                                                                                                                                                                                        );

                                                                                                                                                                                                                                                                                                                                                                                                                    // Backtrack.
                                                                                                                                                                                                                                                                                                                                                                                                                                current.pop_back();
                                                                                                                                                                                                                                                                                                                                                                                                                                        }
                                                                                                                                                                                                                                                                                                                                                                                                                                            }

                                                                                                                                                                                                                                                                                                                                                                                                                                            public:

                                                                                                                                                                                                                                                                                                                                                                                                                                                vector<string> generateParenthesis(int n) {

                                                                                                                                                                                                                                                                                                                                                                                                                                                        vector<string> result;
                                                                                                                                                                                                                                                                                                                                                                                                                                                                string current;

                                                                                                                                                                                                                                                                                                                                                                                                                                                                        backtrack(0, 0, n, current, result);

                                                                                                                                                                                                                                                                                                                                                                                                                                                                                return result;
        
    }
};
