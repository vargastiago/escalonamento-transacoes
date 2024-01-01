#include <iostream>
#include <set>
#include <sstream>
#include <vector>

#include "schedule.hpp"

int main() {
    size_t commits = 0;                // contador de commits
    size_t sched_id = 0;               // identificador do agendamento
    std::vector<transaction> schedule; // agendamento (vetor de transações)
    std::set<int> tids;                // conjunto dos ids das transações
    std::string line;

    while (std::getline(std::cin, line)) {
        std::stringstream ss(line);
        transaction t;

        ss >> t.time >> t.id >> t.op >> t.attr;

        schedule.push_back(t);
        tids.insert(t.id);

        if (t.op == 'C') {
            commits++;
        }

        if (tids.size() == commits) {

            // imprime o identificador do escalonamento
            std::cout << ++sched_id << ' ';

            // imprime a lista de transações
            std::set<int>::iterator it;
            for (it = tids.begin(); it != prev(tids.end(), 1); it++) {
                std::cout << *it << ',';
            }
            std::cout << *it << ' ';

            // imprime o resultado dos algoritmos de seriabilidade por
            // conflito e visão equivalente
            std::cout << (serializable(schedule) ? "SS" : "NS") << ' '
                      << (view_equivalent(schedule) ? "SV" : "NV") << '\n';

            commits = 0;
            schedule.clear();
            tids.clear();
        }
    }
}
