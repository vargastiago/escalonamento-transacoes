#ifndef _SCHEDULE_H_
#define _SCHEDULE_H_

typedef struct transaction {
    int time;  // tempo de chegada
    int id;    // id da transação
    char op;   // operação (R=read, W=write, C=commit)
    char attr; // atributo lido/escrito
} transaction;

bool serializable(std::vector<transaction> &schedule);
bool view_equivalent(std::vector<transaction> &schedule);

#endif
