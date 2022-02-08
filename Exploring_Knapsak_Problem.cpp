#include "profile.h"
#include "full_enum.h"
#include "branch_and_bound.h"
#include "inpoutp.h"

using namespace std;

int main()
{
    int N, M;               //N - количество переменных, M - количество ограничений
    vector<int> v, b;       //Постарался сохранить нотацию оригинала, A у меня стал матрицей M на N
    vector<vector<int>> A, Subscripts;
    input_and_preparation(N, M, v, b, A, Subscripts);           //Функция считывает (или генерирует) данные по моим лекалам (пример в input.txt)
    output(N, M, v, b, A, Subscripts);
    {
        LOG_DURATION("Shih duration");
        vector<Node> Nodes;                                         // Здесь будем хранить ноды
        Node root;                                                  //Начало расчетной части (задаем корневую ноду)
        bound_for_node(root, Subscripts, v, A, b);                  //Нода передается по ссылке и приводится к окончательному виду при завершении функции
        Nodes.push_back(root);
        while (true) {
            set<int> I_tmp = root.I, E_tmp = root.E;                //Передаем во множества граничные элементы
            I_tmp.insert(root.bound_item_index);
            E_tmp.insert(root.bound_item_index);
            Node child_left, child_right;                           //Задаем два потомка для текущего корняи вызываем для них функцию подсчёта границы
            child_left.number = Nodes.back().number + 1;
            child_right.number = Nodes.back().number + 2;
            child_left.E = E_tmp;
            child_left.I = root.I;
            child_right.I = I_tmp;
            child_right.E = root.E;
            bound_for_node(child_left, Subscripts, v, A, b);
            bound_for_node(child_right, Subscripts, v, A, b);
            Nodes.push_back(child_left);                            //Записываем полученные ноды с границами в массив
            Nodes.push_back(child_right);
            auto no_max = remove(Nodes.begin(), Nodes.end(), root); //удалять сразу
            Nodes.erase(no_max, Nodes.end());
            Node max_child = find_max_child(Nodes);                 //Ищем ноду с максимальной границей
            if (max_child.bound_item_index == -1) {                 //Если для ноды с максимальной из границ достигнуто условие выполнимости, завершаем работу
                cout << "Result at node " << max_child.number << " with value of " << max_child.Bound << "\nVector indexes for solution B&B: ";
                float s = 0.0;
                for (int i = 0; i < v.size(); i++) {
                    cout << max_child.solution[i] << " ";
                    s += v[i] * max_child.solution[i];
                }
                cout << endl;
                cout << "Just checking: " << s << "\n";
                break;
            }
            else {                                                  //Если условие не достигнуто, максимальная нода становится родителем
                root = max_child;
            }
        }
    }
    {
        LOG_DURATION("Full enumeration duration");
        pair<int, vector<int>> sol_enum = full_enum(v, A, b);
        cout << "\nFull enum solution: " << sol_enum.first << "\nVector indexes for solution full enum: ";
        for (auto c : sol_enum.second) {
            cout << c << " ";
        }
        cout << endl;
    }
}