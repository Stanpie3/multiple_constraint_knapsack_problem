#include "shih.h"


bool operator < (const Node& lhs, const Node& rhs) {
    return lhs.Bound < rhs.Bound;
}

pair<int, vector<int>> shih_algorithm(const vector<vector<int>>& Subscripts, const vector<int>& v, const vector<vector<int>>& A, vector<int>& b) {
    priority_queue<Node> Nodes;
    Node root;                                                  //������ ��������� ����� (������ �������� ����)
    int num = 1;
    bound_for_node(root, Subscripts, v, A, b);                  //���� ���������� �� ������ � ���������� � �������������� ���� ��� ���������� �������
    Nodes.push(root);
    while (true) {
        set<int> I_tmp = root.I, E_tmp = root.E;                //�������� �� ��������� ��������� ��������
        I_tmp.insert(root.bound_item_index);
        E_tmp.insert(root.bound_item_index);
        Node child_left, child_right;                           //������ ��� ������� ��� �������� ������ �������� ��� ��� ������� �������� �������
        child_left.number = num + 1;
        child_right.number = num + 2;
        num += 2;
        child_left.E = E_tmp;
        child_left.I = root.I;
        child_right.I = I_tmp;
        child_right.E = root.E;
        bound_for_node(child_left, Subscripts, v, A, b);
        bound_for_node(child_right, Subscripts, v, A, b);
        Nodes.pop();
        Nodes.push(child_left);                            //���������� ���������� ���� � ��������� � ������
        Nodes.push(child_right);
        Node max_child = Nodes.top();
        if (max_child.bound_item_index == -1 || test_feasible(max_child.solution, A, b)) {                 //���� ��� ���� � ������������ �� ������ ���������� ������� ������������, ��������� ������
            std::cout << "Result at node " << max_child.number << " with value of " << max_child.Bound << "\nVector indexes for solution B&B: ";
            float s = 0;
            vector<int> sol(max_child.solution.size());
            for (int i = 0; i < v.size(); i++) {
                //cout << max_child.solution[i] << " ";
                s += (v[i] * max_child.solution[i]);
                sol[i] = (int)max_child.solution[i];
            }
            return {(int)s, sol};
            break;
        }
        else {                                                  //���� ������� �� ����������, ������������ ���� ���������� ���������
            root = max_child;
        }
    }
}