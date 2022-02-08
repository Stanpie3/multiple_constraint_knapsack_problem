#pragma once
#include <vector>
#include <set>
#include <map>
#include <algorithm>
#include <numeric>
using namespace std;

struct Node {
    int number = 1;
    float Bound = 0;
    int bound_item_index = 0;
    set<int> I, E;
    vector<float> solution;
};

bool operator == (const Node& lhs, const Node& rhs);

void remove_a_from_b(const set<int>& A, vector<int>& b);

void bound_for_node(Node& node, const vector<vector<int>>& Subscripts,
    const vector<int>& v, const vector<vector<int>>& A, const vector<int>& b);

Node find_max_child(vector<Node>& v);