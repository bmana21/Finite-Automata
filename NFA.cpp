//
// Created by Beso Managadze on 03/05/23.
//

#include "NFA.h"

class state;

NFA::NFA(bool is_final) {
    auto *new_state = new state();
    if (is_final) {
        new_state->make_final();
        final_states.push_back(new_state);
    }
    new_state->is_start = true;
    start_state = new_state;
    states.push_back(new_state);
}

void NFA::deep_delete() {
    for (auto &state: states)
        delete state;
}

void NFA::add_state(state *new_state) {
    states.push_back(new_state);
}


void NFA::add_final_state(state *final_state) {
    final_state->make_final();
    final_states.push_back(final_state);
}

state *NFA::get_start_state() {
    return start_state;
}

vector<state *> *NFA::get_final_states() {
    return &final_states;
}

vector<state *> *NFA::get_states() {
    return &states;
}

void NFA::concatenate_NFA(NFA *nfa) {
    if (nfa->get_states()->size() == 1)
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
    for (int k = 0; k < states.size(); k++)
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

}

void NFA::star_NFA() {
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

void NFA::union_NFA(NFA *nfa) {
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
    for (int k = 0; k < states.size(); k++)
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
}

void NFA::print_NFA() {
    int num_transitions = 0;
    for (auto state: states)
        num_transitions += (int) state->get_transitions()->size();
    cout << states.size() << ' ' << final_states.size() << ' ' << num_transitions << endl;
    for (auto state: final_states)
        cout << state->index << ' ';
    cout << endl;
    for (auto &state: states) {
        auto transitions = state->get_transitions();
        cout << transitions->size() << ' ';
        for (auto transition: *transitions)
            cout << transition.first << ' ' << transition.second->index << ' ';
        cout << endl;
    }
}
