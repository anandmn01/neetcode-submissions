class Solution {
public:
    int evalRPN(vector<string>& tokens) {
        stack<int> st;

        for (string token : tokens) {

            // If token is an operator
            if (token == "+" || token == "-" ||
                token == "*" || token == "/") {

                int b = st.top();
                st.pop();

                int a = st.top();
                st.pop();

                int result = 0;

                if (token == "+") {
                    result = a + b;
                }
                else if (token == "-") {
                    result = a - b;
                }
                else if (token == "*") {
                    result = a * b;
                }
                else if (token == "/") {
                    result = a / b;
                }

                st.push(result);
            }

            // If token is a number
            else {
                st.push(stoi(token));
            }
        }

        return st.top();
    }
};
