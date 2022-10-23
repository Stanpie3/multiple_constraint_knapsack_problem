#include "profile.h"
#include "full_enum.h"
#include "branch_and_bound.h"
#include "inpoutp.h"
#include "shih.h"

using namespace std;

int main()
{
    vector<int> v, b;       //Постарался сохранить нотацию оригинала, A у меня стал матрицей M на N
    vector<vector<int>> A, Subscripts;
    input_and_preparation_manual(v, b, A, Subscripts);           //Функция считывает (или генерирует) данные по моим лекалам (пример в input.txt)
    output(v.size(), A.size(), v, b, A, Subscripts);
    int variant;
    cout << "Do you want too simply generate - 1, or to test algorithm - 2?\n";
    cin >> variant;
    if (variant == 1) {
        return 0;
    }   
    {
        LOG_DURATION("Shih duration");
        pair<int, vector<int>> sol_shih = shih_algorithm(Subscripts, v, A, b);
        cout << "\nShih solution: " << sol_shih.first << "\nVector indexes for solution Shih: ";
        for (auto c : sol_shih.second) {
            cout << c << " ";
        }
        cout << "\n";
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
    return 0;
}