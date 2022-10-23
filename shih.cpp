#include "shih.h"


bool operator < (const Node& lhs, const Node& rhs) {
    return lhs.Bound < rhs.Bound;
}

pair<int, vector<int>> shih_algorithm(const vector<vector<int>>& Subscripts, const vector<int>& v, const vector<vector<int>>& A, vector<int>& b) {
    priority_queue<Node> Nodes;
    Node root;                                                  //Начало расчетной части (задаем корневую ноду)
    int num = 1;
    bound_for_node(root, Subscripts, v, A, b);                  //Нода передается по ссылке и приводится к окончательному виду при завершении функции
    Nodes.push(root);
    while (true) {
        set<int> I_tmp = root.I, E_tmp = root.E;                //Передаем во множества граничные элементы
        I_tmp.insert(root.bound_item_index);
        E_tmp.insert(root.bound_item_index);
        Node child_left, child_right;                           //Задаем два потомка для текущего корняи вызываем для них функцию подсчёта границы
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
        Nodes.push(child_left);                            //Записываем полученные ноды с границами в массив
        Nodes.push(child_right);
        Node max_child = Nodes.top();
        if (max_child.bound_item_index == -1 || test_feasible(max_child.solution, A, b)) {                 //Если для ноды с максимальной из границ достигнуто условие выполнимости, завершаем работу
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
        else {                                                  //Если условие не достигнуто, максимальная нода становится родителем
            root = max_child;
        }
    }
}