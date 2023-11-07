//
// Created by Beso Managadze on 03/05/23.
//
#include <utility>
#include <set>
#include <algorithm>

#ifndef TI__1_STATE_H
#define TI__1_STATE_H
using namespace std;

class state {
public:

    int index = 0;
    bool is_start = false;

    void make_final();

    void make_reject();

    [[nodiscard]] bool is_final() const;

    void add_transition(char letter, state *next_state);

    void add_transition(pair<char, state *> transition);

    void add_incoming(char letter, state *in_state);

    void remove_transition(pair<char, state *> transition);

    void remove_incoming(pair<char, state *> incoming);

    void add_incoming(pair<char, state *> incoming);

    set<pair<char, state *> > *get_transitions();

    set<pair<char, state *> > *get_incomings();

private:
    bool final = false;
    set<pair<char, state *> > transitions;
    set<pair<char, state *> > incomings;
};


#endif //TI__1_STATE_H
