//
// Created by Beso Managadze on 03/05/23.
//

#include "state.h"
#include <iostream>

#ifndef TI__1_NFA_H
#define TI__1_NFA_H
using namespace std;

class NFA {
public:
    explicit NFA(bool is_final);

    void add_state(state *new_state);

    void deep_delete();

    void add_final_state(state *final_state);

    state *get_start_state();

    vector<state *> *get_final_states();

    vector<state *> *get_states();

    void concatenate_NFA(NFA *nfa);

    void star_NFA();

    void union_NFA(NFA *nfa);

    void print_NFA();

private:
    vector<state *> states;
    state *start_state;
    vector<state *> final_states;
};


#endif //TI__1_NFA_H
