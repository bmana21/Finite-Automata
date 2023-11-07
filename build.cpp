#include <iostream>
#include <utility>
#include <set>
#include <vector>


using namespace std;

class state {
public:
    int index = 0;
    bool is_start = false;

    void make_final() {
        final = true;
    }

    void make_reject() {
        final = false;
    }

    bool is_final() {
        return final;
    }

    void add_transition(char letter, state *next_state) {
        transitions.insert({letter, next_state});
    }

    void add_transition(pair<char, state *> transition) {
        transitions.insert(transition);
    }

    void add_incoming(char letter, state *in_state) {
        incomings.insert({letter, in_state});
    }

    void remove_transition(pair<char, state *> transition) {
        transitions.erase(transition);
    }

    void remove_incoming(pair<char, state *> incoming) {
        incomings.erase(incoming);
    }

    void add_incoming(pair<char, state *> incoming) {
        incomings.insert(incoming);
    }

    set<pair<char, state *> > *get_transitions() {
        return &transitions;
    }

    set<pair<char, state *> > *get_incomings() {
        return &incomings;
    }

private:
    bool final = false;
    set<pair<char, state *> > transitions;
    set<pair<char, state *> > incomings;

};

class NFA {
public:
    NFA(bool is_final) {
        state *new_state = new state();
        if (is_final) {
            new_state->make_final();
            final_states.push_back(new_state);
        }
        new_state->is_start = true;
        start_state = new_state;
        states.push_back(new_state);
    }

    void deep_delete() {
        for (auto &state: states)
            delete state;
    }

    void add_state(state *new_state) {
        states.push_back(new_state);
    }


    void add_final_state(state *final_state) {
        final_state->make_final();
        final_states.push_back(final_state);
    }

    state *get_start_state() {
        return start_state;
    }

    vector<state *> *get_final_states() {
        return &final_states;
    }

    vector<state *> *get_states() {
        return &states;
    }

    void concatenate_NFA(NFA *nfa) {
        if ((int) (nfa->get_states()->size()) == 1)
            return;
        auto states2 = *nfa->get_states();
        auto start = nfa->get_start_state();
        auto finals = *nfa->get_final_states();

        for (auto &final_state: final_states) {
            auto transitions = start->get_transitions();
            auto incomings = start->get_incomings();
            for (auto transition: *transitions) {
                if (transition.second == start)
                    continue;
                final_state->add_transition(transition);
                transition.second->add_incoming(transition.first, final_state);
            }
            for (auto incoming: *incomings) {
                if (incoming.second == start) {
                    final_state->add_transition(incoming.first, final_state);
                    final_state->add_incoming(incoming.first, final_state);
                    continue;
                }
                incoming.second->add_transition(incoming.first, final_state);
                final_state->add_incoming(incoming);
            }

        }

        for (auto state: states2)
            if (!state->is_start)
                states.push_back(state);
        for (int k = 0; k < (int) (states.size()); k++)
            states[k]->index = k;
        for (auto transition: *(start->get_transitions())) {
            transition.second->remove_incoming({transition.first, start});
        }
        for (auto incoming: *(start->get_incomings())) {
            incoming.second->remove_transition({incoming.first, start});
        }
        if (!start->is_final()) {
            for (auto final: final_states)
                final->make_reject();
            final_states.clear();
        }
        for (auto final: finals)
            if (nfa->start_state != final)
                final_states.push_back(final);
        delete (nfa->start_state);
        delete nfa;

    }

    void star_NFA() {
        for (auto final_state: final_states) {
            if (final_state == start_state)
                continue;
            auto transitions = *start_state->get_transitions();
            for (auto transition: transitions) {
                final_state->add_transition(transition);
                transition.second->add_incoming(transition.first, final_state);
            }
        }
        if (!start_state->is_final()) {
            start_state->make_final();
            final_states.push_back(start_state);
        }
        int index = 0;
        for (auto state: states) {
            state->index = index;
            index++;
        }
    }

    void union_NFA(NFA *nfa) {
        auto states2 = *nfa->get_states();
        auto start = nfa->get_start_state();
        auto finals = *nfa->get_final_states();

        auto transitions = start->get_transitions();
        auto incomings = start->get_incomings();
        for (auto transition: *transitions) {
            if (transition.second == start)
                continue;
            start_state->add_transition(transition);
            transition.second->add_incoming(transition.first, start_state);
        }
        for (auto incoming: *incomings) {
            if (incoming.second == start) {
                start_state->add_transition(incoming.first, start_state);
                start_state->add_incoming(incoming.first, start_state);
                continue;
            }
            incoming.second->add_transition(incoming.first, start_state);
            start_state->add_incoming(incoming);
        }


        for (auto state: states2)
            if (!state->is_start)
                states.push_back(state);
        for (int k = 0; k < (int) (states.size()); k++)
            states[k]->index = k;
        for (auto transition: *(start->get_transitions())) {
            transition.second->remove_incoming({transition.first, start});
        }
        for (auto incoming: *(start->get_incomings())) {
            incoming.second->remove_transition({incoming.first, start});
        }
        for (auto final: finals)
            if (nfa->start_state != final)
                final_states.push_back(final);
        if (nfa->start_state->is_final() && !start_state->is_final()) {
            start_state->make_final();
            final_states.push_back(start_state);
        }
        delete (nfa->start_state);
        delete nfa;
    }

    void print_NFA() {
        int num_transitions = 0;
        for (auto state: states)
            num_transitions += (int) (state->get_transitions()->size());
        cout << (int) (states.size()) << ' ' << (int) (final_states.size()) << ' ' << num_transitions << endl;
        for (auto state: final_states)
            cout << state->index << ' ';
        cout << endl;
        for (int k = 0; k < (int) (states.size()); k++) {
            auto transitions = states[k]->get_transitions();
            cout << (int) (transitions->size()) << ' ';
            for (auto transition: *transitions)
                cout << transition.first << ' ' << transition.second->index << ' ';
            cout << endl;
        }
    }

private:
    vector<state *> states;
    state *start_state;
    vector<state *> final_states;
};

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
            state *new_state = new state();
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