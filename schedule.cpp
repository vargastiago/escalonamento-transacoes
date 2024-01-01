#include <algorithm>
#include <map>
#include <set>
#include <vector>

#include "schedule.hpp"

// Função recursiva auxiliar para a detecção de ciclo no grafo.
bool cyclic_recur(std::map<int, std::vector<int>> &graph, int v,
                  std::map<int, bool> &visited, std::map<int, bool> &stack) {

    visited.at(v) = true;
    stack.at(v) = true;

    for (auto const &u : graph.at(v)) {
        if (!visited.at(u) && cyclic_recur(graph, u, visited, stack)) {
            return true;
        } else if (stack.at(u)) {
            return true;
        }
    }

    stack.at(v) = false;
    return false;
}

// Detecta um ciclo no grafo utilizando uma busca em profundidade recursiva.
bool cyclic(std::map<int, std::vector<int>> &graph) {
    std::map<int, bool> visited;
    std::map<int, bool> stack;

    for (auto const &p : graph) {
        visited[p.first] = false;
        stack[p.first] = false;
    }

    for (auto const &p : graph) {
        for (int v : p.second) {
            if (!visited.at(v) && cyclic_recur(graph, v, visited, stack)) {
                return true;
            }
        }
    }

    return false;
}

// Verifica se um agendamento é serializável por conflito.
bool serializable(std::vector<transaction> &schedule) {
    std::map<int, std::vector<int>> graph;

    for (auto it_a = schedule.begin(); it_a != schedule.end(); it_a++) {

        // se transação ainda não está no grafo
        if (graph.find(it_a->id) == graph.end()) {
            graph[it_a->id] = {};
        }

        for (auto it_b = it_a + 1; it_b != schedule.end(); it_b++) {

            if (it_a->id != it_b->id && it_a->attr == it_b->attr) {
                if ((it_a->op == 'R' && it_b->op == 'W') ||
                    (it_a->op == 'W' && (it_b->op == 'R' || it_b->op == 'W'))) {

                    // adiciona uma aresta no grafo
                    graph[it_a->id].push_back(it_b->id);
                }
            }
        }
    }

    return !cyclic(graph);
}

// Retorna quantas escritas houveram antes da leitura do mesmo
// atributo em transações distintas.
int write_before_read(std::vector<transaction> &schedule) {
    int count = 0;
    auto it_b = schedule.begin();

    for (auto it_a = schedule.begin() + 1; it_a != schedule.end(); it_a++) {
        if (it_a->op == 'R') {
            for (; it_b != it_a - 1; it_b++) {
                if (it_b->op == 'W' &&
                    it_b->attr == it_a->attr && it_b->id != it_a->id) {

                    count++;
                    it_b = it_a;
                    break;
                }
            }
        }
    }

    return count;
}

// Função auxiliar que analisa as condições 2 e 3 do algoritmo
// de visão equivalente.
bool view_equivalent_util(std::vector<transaction> &s1, std::vector<transaction> &s2,
                          std::set<char> attrs) {

    // condição 2
    if (write_before_read(s1) != write_before_read(s2)) {
        return false;
    }

    // condição 3
    for (char attr : attrs) {
        auto it_s1 = find_if(s1.rbegin(), s1.rend(),
                             [attr](transaction &t) { return t.op == 'W' &&
                                                             t.attr == attr; });

        auto it_s2 = find_if(s2.rbegin(), s2.rend(),
                             [attr](transaction &t) { return t.op == 'W' &&
                                                             t.attr == attr; });

        if (it_s1 != s1.rend() && it_s1->id != it_s2->id) {
            return false;
        }
    }

    return true;
}

// Verifica se um agendamento é serializável por visão equivalente.
bool view_equivalent(std::vector<transaction> &schedule) {
    std::map<int, std::vector<transaction>> m;
    std::set<int> ids;
    std::set<char> attrs;

    for (transaction t : schedule) {
        m[t.id].push_back(t);
        ids.insert(t.id);
        attrs.insert(t.attr);
    }

    attrs.extract('-');

    std::vector<int> tids{ids.begin(), ids.end()}; // vetor de permutações

    do {
        std::vector<transaction> s; // visão
        int time = 0;

        // condição 1: o mesmo conjunto de transações participam em S e S'
        for (int id : tids) {
            for (transaction t : m.at(id)) {
                t.time = time++;
                s.push_back(t);
            }
        }

        if (view_equivalent_util(schedule, s, attrs)) {
            return true;
        }

    } while (next_permutation(tids.begin(), tids.end()));

    return false;
}
