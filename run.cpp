#include <iostream>
#include <vector>
#include "NFA.h"
#include "state.h"

using namespace std;


string input;
vector<bool> answer;

void simulate_input(state *state, int index) {
    if (index == (int) (input.size()) + 1)
        return;
    if (index != 0)
        answer[index - 1] = state->is_final() | answer[index - 1];
    auto transitions = *state->get_transitions();
    for (auto transition: transitions) {
        if (transition.first == input[index])
            simulate_input(transition.second, index + 1);
    }
}

void solve_p2() {
    cin >> input;
    answer.resize(0);
    answer.resize((int) (input.size()), false);
    NFA nfa(false);
    int num_states, num_finals, num_transitions;
    cin >> num_states >> num_finals >> num_transitions;
    for (int k = 1; k < num_states; k++) {
        state *new_state = new state();
        new_state->index = k;
        nfa.add_state(new_state);
    }

    for (int k = 0; k < num_finals; k++) {
        int index;
        cin >> index;
        state *this_state = (*nfa.get_states())[index];
        nfa.add_final_state(this_state);
        this_state->make_final();
    }

    for (int k = 0; k < num_states; k++) {
        state *this_state = (*nfa.get_states())[k];
        int num_t;
        cin >> num_t;
        for (int i = 0; i < num_t; i++) {
            char symbol;
            int index;
            cin >> symbol >> index;
            state *other_state = (*nfa.get_states())[index];
            this_state->add_transition({symbol, other_state});
        }
    }
    simulate_input(nfa.get_start_state(), 0);
    string answer_to_print = "";
    for (auto ans: answer)
        if (ans) answer_to_print += 'Y';
        else answer_to_print += 'N';
    nfa.deep_delete();
    cout << answer_to_print << endl;
}


int main() {

    solve_p2();

    return 0;
}
