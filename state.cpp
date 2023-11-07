//
// Created by Beso Managadze on 03/05/23.
//

#include "state.h"


void state::make_final() {
    final = true;
}

void state::make_reject() {
    final = false;
}

bool state::is_final() {
    return final;
}

void state::add_transition(char letter, state *next_state) {
    transitions.insert({letter, next_state});
}

void state::add_transition(pair<char, state *> transition) {
    transitions.insert(transition);
}

void state::add_incoming(char letter, state *in_state) {
    incomings.insert({letter, in_state});
}

void state::remove_transition(pair<char, state *> transition) {
    transitions.erase(transition);
}

void state::remove_incoming(pair<char, state *> incoming) {
    incomings.erase(incoming);
}

void state::add_incoming(pair<char, state *> incoming) {
    incomings.insert(incoming);
}

set<pair<char, state *> > *state::get_transitions() {
    return &transitions;
}

set<pair<char, state *> > *state::get_incomings() {
    return &incomings;
}
