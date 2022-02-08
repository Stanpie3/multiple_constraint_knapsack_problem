#pragma once
#include <iostream>
#include <fstream>
#include <vector>
#include <map>
using namespace std;

void generate_sample(const string& filename) {   //Генерация данных
    srand(time(NULL));
    ofstream output;
    vector<int> restrictions;
    output.open(filename);
    int N = 15;
    int M = 5;
    restrictions.resize(M);
    output << N << " " << M << '\n';
    for (int i = 0; i < N; i++) {
        output << rand() % 95 + 5 << " ";
    }
    for (int j = 0; j < M; j++) {
        restrictions[j] = 0;
        for (int i = 0; i < N; i++) {
            int elem = rand() % 95 + 5;
            output << elem << " ";
            restrictions[j] += elem;
        }
        output << '\n';
    }
    for (auto e : restrictions) {
        output << e - rand() % (e / 2) << " ";
    }
    output.close();
}   // 

void input_and_preparation(int& N, int& M, vector<int>& v, vector<int>& b,
    vector<vector<int>>& A, vector<vector<int>>& Subscripts) {  // Ввод (или ввод+генерация) данных с проверочным выводом
    int answer = 0;
    cout << " Input: 0 - I want to work with example, 1 - I want to generate, 2 - I want to work with previously generated: ";
    cin >> answer;
    fstream input;
    if (answer == 1) {
        generate_sample("generated_input.txt");
        input.open("generated_input.txt");
    }
    else if (answer == 2) {
        input.open("generated_input.txt");
    }
    else {
        input.open("input.txt");
    }

    input >> N >> M;
    v.resize(N);
    for (int i = 0; i < N; i++) {
        input >> v[i];
    }

    for (int j = 0; j < M; j++) {          //Данный цикл с использованием вспомогательного map ранжирует переменные по приоритетности
        vector<int> a;
        map<float, int> order;
        a.resize(N);
        for (int i = 0; i < N; i++) {
            input >> a[i];
            float distinct = v[i] * 1.0 / a[i];
            order[distinct] = i;
        }
        A.push_back(a);
        vector<int> subscript;
        for (std::map<float, int>::reverse_iterator it = order.rbegin();
            it != order.rend(); ++it) {
            subscript.push_back(it->second);            // Отранжированные по возрастанию приоритетности переменные записываются (как вспомогательные индексы перестановок)
        }                                               // в матрицу Subscripts, где каждая строка соответсвует своему ограничению
        Subscripts.push_back(subscript);
    }

    for (int j = 0; j < M; j++) {
        int weight;
        input >> weight;
        b.push_back(weight);
    }

    input.close();
}

void output(int N, int M, const vector<int>& v, const vector<int>& b,
    const vector<vector<int>>& A, const vector<vector<int>>& Subscripts) {
    cout << "\nN, M: " << N << " " << M << endl;
    cout << "Maximisation function weights v: ";
    for (int i = 0; i < N; i++) {
        cout << v[i] << " ";
    }
    cout << "\n\nRestriction weights A:\n";

    for (int j = 0; j < M; j++) {          //Данный цикл с использованием вспомогательного map ранжирует переменные по приоритетности
        for (int i = 0; i < N; i++) {
            cout << A[j][i] << " ";
        }
        cout << endl;
    }
    cout << "\nRestrictions b: ";
    for (int j = 0; j < M; j++) {
        cout << b[j] << " ";
    }

    cout << "\n\nSubscripts: \n";
    for (int i = 0; i < Subscripts.size(); i++) {
        for (int j = 0; j < Subscripts[i].size(); j++) {
            cout << Subscripts[i][j] + 1 << " ";
        }
        cout << endl;
    }
    cout << endl;
}
