#include <iostream>
#include <string>
#include <complex>
#include <cstring>
#include <utility>

using namespace std;
using complex_t = complex<double>;

struct node {
    enum {
        NODE_COMPLEX,
        NODE_OPERATOR
    } nodeType;
    complex_t value;
    string str;
    node(): value{} {};
    node(char c): nodeType(node::NODE_OPERATOR) { str = c; };
    node(const string& compstr): nodeType(node::NODE_COMPLEX) {
        str = compstr;
        // modifying this to feed to input stream of std::complex
        string expr = compstr;
        stringstream ss;
        ss << '(';
        size_t opPos = expr.find_first_of("+-");
        size_t iPos = expr.find_first_of("i");
        if (opPos == 0 || opPos == string::npos) {
            if (iPos == string::npos) {
                // pure real
                ss << expr;
            } else {
                expr[iPos] = expr[opPos + 1] == 'i' ? '1' : ' ';
                // pure imageary
                ss << "0," << expr;
            }
        } else {
            auto iterOp = expr.begin() + opPos;
            ss << string(expr.begin(), iterOp);
            ss << ',';
            if (iPos == opPos + 1) {
                iterOp++;
                expr[iPos] = '1';
            } else {
                expr[iPos] = ' ';
            }
            ss << string(iterOp, expr.end());
        }
        ss << ')';
        ss >> this->value;
    };
};

static inline int pred(int c) {
    switch (c) {
    case '(': return 0;
    case '+': case '-': return 1;
    case '*': case '/': return 2;
    }
    __builtin_unreachable();
};

int main() {
    int n;

    ios_base::sync_with_stdio(0);
    cin.tie(nullptr);

    cin >> n;
    cin.get();
    while (n--) {
        string s;
        getline(cin, s);

        string output;
        auto iter = s.begin();

        char stack[60];
        int top = 0;

        node nodePool[4096];
        int nodePtr = 0;

        while (iter != s.end()) {
            if (*iter == '[') {
                iter++;  // skip [
                auto iterEnd = iter;
                while (*iterEnd != ']') ++iterEnd;
                nodePool[nodePtr++] = node(string(iter, iterEnd));
                iter = iterEnd;
            } else if (*iter == '(') {
                stack[top++] = *iter;
            } else if (*iter == ')') {
                while (top > 0 && stack[top - 1] != '(') {
                    nodePool[nodePtr++] = node(stack[--top]);
                }
                top--;
            } else {
                while (top > 0 && pred(stack[top - 1]) >= pred(*iter)) {
                    nodePool[nodePtr++] = node(stack[--top]);
                }
                stack[top++] = *iter;
            }
            iter++;
        }

        while (top > 0) {
            nodePool[nodePtr++] = node(stack[--top]);
        }

        complex_t evalStack[4096];
        int evalTop = 0;

        for (int i = 0; i < nodePtr; i++) {
            if (nodePool[i].nodeType == node::NODE_COMPLEX) {
                cout << '[' << nodePool[i].str << ']';
            } else {
                cout << nodePool[i].str;
            }
        }
        cout << '\n';

        for (int i = 0; i < nodePtr; i++) {
            if (nodePool[i].nodeType == node::NODE_COMPLEX) {
                evalStack[evalTop++] = nodePool[i].value;
            } else {
                complex_t& fst = evalStack[evalTop - 2];
                complex_t& snd = evalStack[evalTop - 1];
                switch (nodePool[i].str[0]) {
                case '+': evalStack[evalTop - 2] = fst + snd; break;
                case '-': evalStack[evalTop - 2] = fst - snd; break;
                case '*': evalStack[evalTop - 2] = fst * snd; break;
                case '/': evalStack[evalTop - 2] = fst / snd; break;
                }
                evalTop--;
            }
        }

        const complex_t& result = evalStack[0];
        cout << '[';
        if (result == 0.) {
            cout << '0';
        } else {
            if (result.real()) {
                cout << result.real();
            }
            if (result.imag()) {
                if (result.real() != 0. && result.imag() > 0.) {
                    cout << '+';
                }
                if (result.imag() == 1.) {
                    cout << "i";
                } else if (result.imag() == -1.) {
                    cout << "-i";
                } else {
                    cout << result.imag() << 'i';
                }
            }
        }
        cout << "]\n";
    }
}
