class Solution {
public:
    void setZeroes(vector<vector<int>>& matrix) {
        int m = matrix.size();
        int n = matrix[0].size();

        vector<vector<int>> temp = matrix;

        for (int i = 0; i < m; i++) {

            for (int j = 0; j < n; j++) {

                if (matrix[i][j] == 0) {

                    // Mark entire row
                    for (int col = 0; col < n; col++) {
                        temp[i][col] = 0;
                    }

                    // Mark entire column
                    for (int row = 0; row < m; row++) {
                        temp[row][j] = 0;
                    }
                }
            }
        }

        matrix = temp;
    }
};
