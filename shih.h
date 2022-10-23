#pragma once
#include <vector>
#include <map>
#include <iostream>
#include <queue>
#include "branch_and_bound.h"
using namespace std;

pair<int, vector<int>> shih_algorithm(const vector<vector<int>>& Subscripts, const vector<int>& v, const vector<vector<int>>& A, vector<int>& b);

bool operator < (const Node& lhs, const Node& rhs);