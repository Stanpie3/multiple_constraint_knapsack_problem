#include "branch_and_bound.h"

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

void bound_for_node(Node& node, const vector<vector<int>>& Subscripts,
    const vector<int>& v, const vector<vector<int>>& A, const vector<int>& b) {
    vector<float> Z;            //Задаём вектора Z и bound_items - в них будут отходить границы и граничные элементы каждой операции
    vector<int> bound_items;
    vector<vector<float>> Solutions;
    Solutions.assign(b.size(), vector<float>(v.size()));      // Создаём вектор solutions. В него будем записывать решения для каждого ограничения, минимальное записываем в соотв. ноду

    for (int j = 0; j < b.size(); j++) {
        float sum = 0;
        float z = 0;
        for (auto c : node.I) {             //Заранее подсчитываем влияние на сумму и границу фиксированных в 1 элементов
            sum += A[j][c];
            z += v[c];
            Solutions[j][c] = 1.0;
        }
        if (sum > b[j]) {                   //Если эта сумма сама по себе нарушает ограничение, отбрасываем ветвь
            node.Bound = -sum;
            node.bound_item_index = 0;
            fill(node.solution.begin(), node.solution.end(), 0.0);
            return;
        }
        vector<int> Idx(Subscripts[j]); // Создаём вектор индексов для итерации (изначально - строка Subscr[j]) и удаляем из него все фиксированные элементы. 
        remove_a_from_b(node.E, Idx);
        remove_a_from_b(node.I, Idx);
        auto i = Idx.begin();
        float coef = 0;
        while (sum < b[j]) {
            if (i != Idx.end()) {
                if (sum + A[j][*i] < b[j]) {          //Если сумма с элементом меньше ограничения, прибавляем
                    sum += A[j][*i];
                    z += v[*i];
                    Solutions[j][*i] = 1.0;
                    i++;
                }
                else if (sum + A[j][*i] == b[j]) { //В случае равенства мы получили достижимую границу, отмечаем это
                    z += v[*i];
                    Solutions[j][*i] = 1.0;
                    bound_items.push_back(-1);  // Показатель достижимости (проходим по ограничению, так как в точности достигли раницы)
                    break;
                }
                else {
                    coef = (b[j] - sum) / A[j][*i];       //Находим коэффициент для границы из ограничения
                    Solutions[j][*i] = coef;
                    bound_items.push_back(*i);            //Добавляем граничный элемент и его вес с коэффицентом к оцениваемой функции
                    z += v[*i] * coef;
                    break;
                }
            }
            else {
                bound_items.push_back(-1);
                break;
            }
        }
        Z.push_back(z);         //Пушим в вектор z каждую промежуточную границу 
    }
    /*for (auto z : Z) {
        cout << z << " ";
    }
    cout << endl;*/
    auto it = min_element(Z.begin(), Z.end());          //Находим минимум среди границ, берем соответствующий ей граничный элемент
    node.Bound = *it;
    node.bound_item_index = bound_items.at(distance(Z.begin(), it));
    node.solution = Solutions.at(distance(Z.begin(), it));
}

Node find_max_child(vector<Node>& v) { //Поиск ноды с максимальной границей
    Node result;
    for (const auto& n : v) {
        if (n.Bound > result.Bound) {
            result = n;
        }
    }
    return result;
}