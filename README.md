## Regular representations and finite automata

Our goal is to write a program that checks if a given text matches a given regular expression and simulates automata. The task is divided into two parts:

1. In the first part, we need to automatically convert a given regular expression into an equivalent non-deterministic finite automaton of a specific type.
2. In the second part, we should write a program that simulates the non-deterministic automaton and returns the result.

When simulating a non-deterministic automaton, we don't need to convert it into a deterministic automaton. Just remember the conditions at each step and the number of possible transitions at a given stage, so we can simulate the automaton.

We can choose any programming language for this assignment, preferably one that is easy to install and run on Linux. Commonly used languages include Python 3, Python 2, Java, and C++. This particular implementation that I wrote uses **C++**, as you can see.

The program should read data from standard input and write to standard output.

### Convert Image to Automaton

In this part, our program is given a single line of input from standard input that represents a regular expression. The regular expression contains lowercase letters and numbers of the Latin alphabet. The regular expression may also contain the symbols `|` (union) and `*` (repetition). Concatenation is denoted by writing the images in sequence. Parentheses `( )` are used for grouping. Empty strings are represented by empty parentheses `()`. For example, a regular expression can be `(ab*c(0|1)*)*`.

We need to construct a non-deterministic finite automaton that represents the given regular expression. **The automaton should have no more states than the length of the regular expression + 1.** **Additionally, the automaton should not contain ε-transitions (transitions with an empty string).** To eliminate ε-transitions without increasing the number of states, we can state that whatever was transmitted by ε should also be transmitted without ε. Whenever a state A had an ε-transition to state B, replace that ε-transition with all the transitions from state A to state B.

The program should output the constructed automaton in the following format to standard output:

- The first line should contain three integers: `n`, `a`, and `t`, separated by spaces. `n` is the number of states, `a` is the number of accepting states, and `t` is the number of transitions.
- The second line should contain the indices of the accepting states, separated by spaces.
- For each state, there should be one line of information. The line starts with an integer `ki` that indicates the number of transitions from that state. The sum of all `ki` values should be equal to `t`. Following `ki`, there should be `ki` pairs of values, where the first value represents the symbol of the transition and the second value represents the state number.

Here's an example of the expected output format:

Input:
```
(a|b)*(c|())
```
Output:
```
2 2 3
0 1
3 a 0 b 0 c 1
0
```

The program should be called **build.cpp**.

### Machine Simulation

In this part, we need to write a program called **run.cpp** that reads an input string from the first line and, starting from the second line reads the automaton description in the format specified in the previous section. The program should simulate the working of the automaton on the input string and output a string equal in length to the input string.

The output string should represent whether the automaton was in an accepting state or not after processing the first `i` characters of the input string. For positions where the automaton was in at least one accepting state, the symbol `Y` should be displayed. For positions where the automaton was not in any accepting state, the symbol `N` should be displayed.

The program should be designed to handle the simulation using no more than `O((N + n) · n)` operations, where `N` represents the length of the input string and `n` represents the number of automaton states. Please note that this time complexity is an upper bound, and your program should aim to be efficient within these constraints.

Here's an example input and the corresponding output:


| **Input**                       | **Output**           |
| ------------------------------- | -------------------- |
| aababacab                     | YYYYYYYNN          |
| abbc1acabbbbc001cabc          | NNNYYYNNNNNNYYYYYNNNN |


