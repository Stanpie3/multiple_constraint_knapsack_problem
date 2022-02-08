#pragma once
#include <vector>
#include <map>
using namespace std;

bool feasible(const vector<int>& sol, const vector<vector<int>>& A, const vector<int>& b);
pair<int, vector<int>> full_enum(const vector<int>& v, const vector<vector<int>>& A, const vector<int>& b);