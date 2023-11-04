
#include <bits/stdc++.h>
using namespace std;
int numberOfChar = 0;
int nodeNumber = 1;

class Node
{
public:
    int val;
    map<char, set<Node *>> transition;
    Node()
    {
        val = nodeNumber++; // assigning a new number to the new node
    }
    void print()
    {
        cout << val;
    }
};

class Nfa
{
public:
    Node *start;
    set<Node *> end;
    Nfa()
    {
        start = new Node();
        end = {new Node()};
    }
    Nfa(char c)
    {
        start = new Node();
        end = {new Node()};
        for (auto it : end)
            start->transition[c].insert(it);
    }
    void print()
    {
        cout << "Start ";
        start->print();
        cout << " "
             << "End ";
        for (auto &it : end)
        {
            it->print();
            cout << " ";
        }
        cout << "\n";
        set<Node *> ss;
        queue<Node *> q;
        q.push(start);
        while (!q.empty())
        {
            Node *t = q.front();
            q.pop();
            ss.insert(t);
            if (t->transition.size() > 0)
            {
                t->print();
                cout << " -";
            }
            for (auto it : t->transition)
            {
                cout << " " << it.first << " -> ";
                for (auto iu : it.second)
                {
                    iu->print();
                    cout << " ";
                    if (ss.find(iu) == ss.end())
                    {
                        ss.insert(iu);
                        q.push(iu);
                    }
                }
            }
            if (t->transition.size() > 0)
            {
                cout << "\n";
            }
        }
    }
};
bool isOperand(char c)
{
    return c == '.' || c == '|' || c == '*';
}
int precedence(char c)
{
    switch (c)
    {
    case '*':
        return 3;
        break;
    case '.':
        return 2;
        break;
    case '|':
        return 1;
        break;
    default:
        return -1;
    }
    return 0;
}
string infixToPostfix(string &s)
{
    int n = s.size();
    string t;
    stack<char> st;
    for (int i = 0; i < n; i++)
    {
        if (s[i] == '(')
        {
            st.push('(');
        }
        else if (s[i] == ')')
        {
            while (!st.empty() && st.top() != '(')
            {
                t += st.top();
                st.pop();
            }
            st.pop();
        }
        else if (!isOperand(s[i]))
        {
            t += s[i];
        }
        else
        {
            while (!st.empty() && precedence(st.top()) >= precedence(s[i]))
            {
                t += st.top();
                st.pop();
            }
            st.push(s[i]);
        }
    }
    while (!st.empty())
    {
        t += st.top();
        st.pop();
    }
    return t;
}
Nfa *Concatinate(Nfa *&a, Nfa *&b)
{
    for (auto &it : a->end)
        it->transition['$'].insert(b->start);

    a->end = b->end;
    return a;
}
Nfa *Or(Nfa *&a, Nfa *&b)
{
    Nfa *c = new Nfa();
    c->start->transition['$'].insert(a->start);
    c->start->transition['$'].insert(b->start);
    for (auto it : c->end)
    {
        for (auto &iu : a->end)
        {

            iu->transition['$'].insert(it);
        }
        for (auto &iu : b->end)
        {
            iu->transition['$'].insert(it);
        }
    }
    return c;
}
Nfa *Cleanstar(Nfa *&a)
{
    Nfa *c = new Nfa();
    c->start->transition['$'].insert(a->start);
    for (auto it : c->end)
    {
        c->start->transition['$'].insert(it);
    }
    for (auto it : c->end)
    {
        for (auto &it1 : a->end)
        {
            it1->transition['$'].insert(it);
        }
    }
    for (auto it : a->end)
    {
        it->transition['$'].insert(a->start);
    }
    return c;
}
void dfs(set<Node *> &sn, Node *t, set<Node *> &vis)
{
    if (vis.find(t) != vis.end())
    {
        return;
    }
    vis.insert(t);
    if (t->transition.find('$') != t->transition.end())
    {
        for (auto it : t->transition['$'])
        {
            sn.insert(it);
            dfs(sn, it, vis);
        }
    }
}
Nfa *RemoveEpsilon(Nfa *&a)
{
    set<Node *> ss;
    queue<Node *> q;
    q.push(a->start);
    while (!q.empty())
    {
        Node *t = q.front();
        q.pop();
        ss.insert(t);
        for (auto it : t->transition)
        {
            for (auto iu : it.second)
            {
                if (ss.find(iu) == ss.end())
                {
                    q.push(iu);
                }
            }
        }

        set<Node *> sn;
        set<Node *> vis;
        dfs(sn, t, vis);

        for (int i = 0; i < numberOfChar; i++)
        {
            char c = (char)('a' + i);
            for (auto it : sn)
            {
                if (a->end.find(it) != a->end.end())
                {
                    a->end.insert(t);
                }
                if (it->transition.find(c) == it->transition.end())
                {
                    continue;
                }
                for (auto i : it->transition[c])
                {
                    t->transition[c].insert(i);
                }
            }
        }

        if (t->transition.find('$') != t->transition.end())
        {
            t->transition.erase(t->transition.find('$'));
        }
    }
    return a;
}
void dfs1(Node *n, set<Node *> &e, string &s, int i, Node *&nn)
{
    if (i == s.size())
    {
        n->print();
        cout << " ";
        if (e.find(n) != e.end())
        {
            cout << "\nThe test string is being accepted\n";
        }
        return;
    }
    n->print();
    cout << " - " << s[i] << " -> ";
    for (auto it : n->transition)
    {
        if (it.first == s[i])
        {
            for (auto iu : it.second)
            {
                dfs1(iu, e, s, i + 1, nn);
            }
        }
    }
}
int main()
{
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    cout << "Enter number of literals" << endl;
    cin >> numberOfChar;
    string s;
    cout << "enter the regex" << endl;
    cin >> s;
    cout << "\nThe postfic expression is\n";
    string t = infixToPostfix(s);
    cout << t << "\n";
    stack<Nfa *> st;
    int nn = t.size();
    for (int i = 0; i < nn; i++)
    {
        if (!isOperand(t[i]))
        {
            st.push(new Nfa(t[i]));
        }
        else
        {
            if (t[i] == '.')
            {
                Nfa *a = st.top();
                st.pop();
                Nfa *b = st.top();
                st.pop();
                Nfa *c = Concatinate(b, a);
                st.push(c);
            }
            else if (t[i] == '|')
            {
                Nfa *a = st.top();
                st.pop();
                Nfa *b = st.top();
                st.pop();
                Nfa *c = Or(b, a);
                st.push(c);
            }
            else
            {
                Nfa *a = st.top();
                st.pop();
                Nfa *c = Cleanstar(a);
                st.push(c);
            }
        }
    }
    Nfa *resul = st.top();
    st.pop();
    cout << "\nEpsilon NFA for given regex is\n";
    resul->print();
    resul = RemoveEpsilon(resul);
    cout << "\nNFA after using epsilon-star method\n";
    resul->print();
    cout << "\nenter the string to push into our NFA" << endl;
    string sim;
    cin >> sim;
    Node *n;
    dfs1(resul->start, resul->end, sim, 0, n);
    cout << "\n";
}
