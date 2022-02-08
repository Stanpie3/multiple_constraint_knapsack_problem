#include "full_enum.h"

bool feasible(const vector<int>& sol, const vector<vector<int>>& A, const vector<int>& b) {
    for (int i = 0; i < A.size(); i++) {
        int fes = 0;
        for (int j = 0; j < A.at(i).size(); j++) {
            fes += sol[j] * A[i][j];
        }
        if (fes > b[i]) {
            return false;
        }
    }
    return true;
}

pair<int, vector<int>> full_enum(const vector<int>& v, const vector<vector<int>>& A, const vector<int>& b) {
    int N = pow(2, v.size());
    map<int, vector<int>> result;
    for (int i = 0; i < N; i++) {
        vector<int> solution(v.size(), 0);
        int t = i;
        int j = 0;
        while (t > 0) {
            solution[j] = t % 2;
            j += 1;
            t /= 2;
        }
        if (feasible(solution, A, b)) {
            int profit = 0;
            for (int k = 0; k < v.size(); k++) {
                profit += v[k] * solution[k];
            }
            result[profit] = solution;
        }
    }
    return { result.rbegin()->first, result.rbegin()->second };
}