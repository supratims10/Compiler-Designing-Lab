#include <bits/stdc++.h>
using namespace std;
const int TOTAL_MACHINES = 5;

enum State
{
    non_accepting,
    accepting,
    accepting_and_retracting,
    invalid
};

struct Node
{
    int val;
    State _state;
};

bool isLetter(char x)
{
    bool op1 = x >= 'a' && x <= 'z';
    bool op2 = x >= 'A' && x <= 'z';
    return op1 || op2;
}

bool isDigit(char x) { return (x >= '0' && x <= '9'); }

bool isDelim(char x) { return (x == ' ' || x == '\t' || x == '\n'); }

Node keyword_machine(Node state, char x)
{
    int currState = state.val;
    switch (currState)
    {
    case 0:
        if (x == 'e')
            return {7, non_accepting};
        if (x == 't')
            return {1, non_accepting};
        if (x == 'i')
            return {6, non_accepting};
        break;

    case 1:
        if (x == 'h')
            return {2, non_accepting};
        break;

    case 2:
        if (x == 'e')
            return {3, non_accepting};
        break;

    case 3:
        if (x == 'n')
            return {4, non_accepting};
        break;

    case 4:
        if (!isDigit(x) && !isLetter(x))
            return {5, accepting_and_retracting};
        break;

    case 6:
        if (x == 'f')
            return {4, non_accepting};
        break;

    case 7:
        if (x == 'l')
            return {8, non_accepting};
        break;

    case 8:
        if (x == 's')
            return {9, non_accepting};
        break;

    case 9:
        if (x == 'e')
            return {4, non_accepting};
        break;
    }

    return {-1, invalid};
}

Node relop_machine(Node state, char x)
{
    int currState = state.val;
    switch (currState)
    {
    case 0:
        if (x == '<')
            return {1, non_accepting};
        if (x == '=')
            return {5, accepting};
        if (x == '>')
            return {6, non_accepting};
        break;

    case 1:
        if (x == '=')
            return {2, accepting};
        if (x == '>')
            return {3, accepting};
        return {4, accepting_and_retracting};
        break;

    case 6:
        if (x == '=')
            return {7, accepting};
        return {8, accepting_and_retracting};
        break;
    }

    return {-1, invalid};
}

Node identifier_machine(Node state, char x)
{
    int currState = state.val;
    switch (currState)
    {
    case 0:
        if (isLetter(x))
            return {1, non_accepting};
        break;

    case 1:
        if (isLetter(x) || isDigit(x))
            return {1, non_accepting};
        return {2, accepting_and_retracting};
        break;
    }

    return {-1, invalid};
}

Node number_machine(Node state, char x)
{
    int currState = state.val;
    switch (currState)
    {
    case 0:
        if (isDigit(x))
            return {1, non_accepting};
        break;

    case 1:
        if (isDigit(x))
            return {1, non_accepting};
        if (x == 'E')
            return {4, non_accepting};
        if (x == '.')
            return {2, non_accepting};
        return {8, accepting_and_retracting};
        break;

    case 2:
        if (isDigit(x))
            return {3, non_accepting};
        break;

    case 3:
        if (isDigit(x))
            return {3, non_accepting};
        if (x == 'E')
            return {4, non_accepting};
        return {9, accepting_and_retracting};
        break;

    case 4:
        if (isDigit(x))
            return {6, non_accepting};
        if (x == '+' || x == '-')
            return {5, non_accepting};
        break;

    case 5:
        if (isDigit(x))
            return {6, non_accepting};
        break;

    case 6:
        if (isDigit(x))
            return {6, non_accepting};
        return {7, accepting_and_retracting};
        break;
    }

    return {-1, invalid};
}

Node delim_machine(Node state, char x)
{
    int currState = state.val;
    switch (currState)
    {
    case 0:
        if (isDelim(x))
            return {1, accepting};
        break;
    }
    return {-1, invalid};
}

Node getNext(Node state, char x, int machine)
{
    if (machine == 0)
        return keyword_machine(state, x);
    if (machine == 1)
        return relop_machine(state, x);
    if (machine == 2)
        return identifier_machine(state, x);
    if (machine == 3)
        return number_machine(state, x);
    if (machine == 4)
        return delim_machine(state, x);
    return {-1, invalid};
}

string getTokenClass(int machine)
{
    if (machine == 0)
        return "";
    if (machine == 1)
        return "relop";
    if (machine == 2)
        return "id";
    if (machine == 3)
        return "number";
    if (machine == 4)
        return "delim";
    return "";
}

int parser(string &s, int i)
{
    for (int machine = 0; machine < TOTAL_MACHINES; machine++)
    {
        int j = i;
        Node curr = {0, non_accepting};
        while (j < s.length() && curr._state == 0)
        {
            curr = getNext(curr, s[j], machine);
            j++;
        }
        if (curr._state == 1 || curr._state == 2)
        {
            int nextIndx = curr._state == 1 ? j : j - 1;
            if (machine != 4)
            {
                string token = getTokenClass(machine);
                if (token.size() != 0)
                    cout << "(" << token << "," << s.substr(i, nextIndx - i)
                         << ")" << endl;
                else
                    cout << "(" << s.substr(i, nextIndx - i) << "," << token
                         << ")" << endl;
            }
            return nextIndx;
        }
    }
    return -1;
}

int main()
{
    string inputProgram, temp;
    int indx = 0;

    do
    {
        getline(cin, temp);
        inputProgram += temp + " ";
    } while (temp.length() != 0);

    inputProgram += ' ';
    bool possible = true;

    while (indx < inputProgram.length())
    {
        int temp = parser(inputProgram, indx);
        if (temp == -1)
        {
            cout << "Error parsing program at char " << indx << endl;
            possible = false;
            break;
        }
        indx = temp;
    }

    return 0;
}