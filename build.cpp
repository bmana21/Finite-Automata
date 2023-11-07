#include <iostream>
#include <utility>
#include <vector>
#include "NFA.h"


using namespace std;

pair<NFA *, int> build_NFA(string &expression, int start_index, bool do_star) {
    NFA *nfa = new NFA(true);
    bool operation = false;
    for (int k = start_index; k < (int) (expression.size()); k++) {
        if (expression[k] == '*') {
            nfa->star_NFA();
            continue;
        }
        if (expression[k] == '|') {
            auto ans = build_NFA(expression, k + 1, false);
            NFA *new_nfa = ans.first;
            k = ans.second;
            nfa->union_NFA(new_nfa);
            k = ans.second;
            if (k == (int) (expression.size()))
                continue;
        }
        if (expression[k] != '(' && expression[k] != ')') {
            operation = true;
            NFA *new_nfa = new NFA(false);
            auto *new_state = new state();
            new_state->index = 1;
            new_state->make_final();
            new_nfa->add_final_state(new_state);
            new_nfa->add_state(new_state);
            new_nfa->get_start_state()->add_transition(expression[k], new_state);
            new_state->add_incoming(expression[k], new_nfa->get_start_state());
            if (((k + 1) < (int) (expression.size())) && expression[k + 1] == '*') {
                new_nfa->star_NFA();
                k++;
            }
            nfa->concatenate_NFA(new_nfa);
            continue;
        }
        if (expression[k] == '(') {
            auto ans = build_NFA(expression, k + 1, true);
            NFA *new_nfa = ans.first;
            k = ans.second;
            if (!operation) {
                nfa->deep_delete();
                delete nfa;
                nfa = new_nfa;
                operation = true;
            } else {
                nfa->concatenate_NFA(new_nfa);
            }
            continue;
        }
        if (expression[k] == ')') {
            if (do_star)
                if (expression[k + 1] == '*') {
                    nfa->star_NFA();
                    k++;
                }
            return {nfa, k};
        }
    }
    return {nfa, (int) (expression.size())};
}

void solve_p1() {
    string expression;
    cin >> expression;
    auto answer_nfa = build_NFA(expression, 0, true).first;
    answer_nfa->print_NFA();
    answer_nfa->deep_delete();
    delete answer_nfa;
}

int main() {

    solve_p1();

    return 0;
}