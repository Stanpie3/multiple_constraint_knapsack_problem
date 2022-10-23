#include "branch_and_bound.h"
#include<iostream>

template <class T>
void print_vector(const vector<T> v) {
    cout << "\n";
    for (const auto c : v) {
        cout << c << " ";
    }
    cout << "\n";
}

bool operator == (const Node& lhs, const Node& rhs) {
    return lhs.number == rhs.number;
}

void remove_a_from_b(const set<int>& A, vector<int>& b) {
    for (auto a : A)
    {
        auto iter = std::find(b.begin(), b.end(), a);
        if (iter != b.end())
        {
            b.erase(iter);
        }
    }
}

bool test_feasible(const vector<float> sol, const vector<vector<int>>& A, const vector<int>& b) {
    for (int j = 0; j < A.size(); j++) {
        int rest = 0;
        for (int i = 0; i < sol.size(); i++) {
            if (sol[i] > 0) {
                rest += A[j][i];
            }
            if (rest > b[j]) { return false; }
        }
    }
    return true;
}

void bound_for_node(Node& node, const vector<vector<int>>& Subscripts,
    const vector<int>& v, const vector<vector<int>>& A, const vector<int>& b) {
    vector<float> Z;            //����� ������� Z � bound_items - � ��� ����� �������� ������� � ��������� �������� ������ ��������
    vector<int> bound_items;
    vector<vector<float>> Solutions;
    Solutions.assign(b.size(), vector<float>(v.size()));      // ������ ������ solutions. � ���� ����� ���������� ������� ��� ������� �����������, ����������� ���������� � �����. ����

    for (int j = 0; j < b.size(); j++) {
        float sum = 0;
        float z = 0;
        for (auto c : node.I) {             //������� ������������ ������� �� ����� � ������� ������������� � 1 ���������
            sum += A[j][c];                 //��� ������?
            z += v[c];
            Solutions[j][c] = 1.0;
        }
        if (sum > b[j]) {                   //���� ��� ����� ���� �� ���� �������� �����������, ����������� �����
            node.Bound = -sum;
            node.bound_item_index = 0;
            fill(node.solution.begin(), node.solution.end(), 0.0);
            return;
        }
        vector<int> Idx(Subscripts[j]); // ������ ������ �������� ��� �������� (���������� - ������ Subscr[j]) � ������� �� ���� ��� ������������� ��������. 
        remove_a_from_b(node.E, Idx);
        remove_a_from_b(node.I, Idx);
        auto i = Idx.begin();
        float coef = 0;
        while (sum < b[j]) {
            if (i != Idx.end()) {
                if (sum + A[j][*i] < b[j]) {          //���� ����� � ��������� ������ �����������, ����������
                    sum += A[j][*i];
                    z += v[*i];
                    Solutions[j][*i] = 1.0;
                    i++;
                }
                else if (sum + A[j][*i] == b[j]) { //� ������ ��������� �� �������� ���������� �������, �������� ���
                    z += v[*i];
                    Solutions[j][*i] = 1.0;
                    bound_items.push_back(Idx.back());  // ��������
                    break;
                }
                else {
                    coef = (b[j] - sum) / A[j][*i];       //������� ����������� ��� ������� �� �����������
                    Solutions[j][*i] = coef;
                    bound_items.push_back(Idx.back());            //��������� ��������� ������� � ��� ��� � ������������ � ����������� ������� //��������
                    z += v[*i] * coef;
                    break;
                }
            }
            else {
                if (test_feasible(Solutions[j], A, b)) {
                    bound_items.push_back(-1);
                }
                else {
                    bound_items.push_back(-2);
                }
                break;
            }
        }
        Z.push_back(z);         //����� � ������ z ������ ������������� ������� 
    }
    auto it = min_element(Z.begin(), Z.end());          //������� ������� ����� ������, ����� ��������������� �� ��������� �������
    node.Bound = *it;
    node.bound_item_index = bound_items.at(distance(Z.begin(), it));
    if (node.bound_item_index == -2) {
        node.Bound = -9999;
    }
    node.solution = Solutions.at(distance(Z.begin(), it));
}

Node find_max_child(vector<Node>& v) { //����� ���� � ������������ ��������
    Node result;
    for (const auto& n : v) {
        if (n.Bound > result.Bound) {
            result = n;
        }
    }
    return result;
}