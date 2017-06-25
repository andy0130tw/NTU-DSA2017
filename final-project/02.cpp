#include <iostream>
#include <unordered_map>
#include <vector>

using namespace std;
using strpair = unordered_map<string, string>;

strpair::const_iterator find_head(const strpair& fwd, const strpair& bwd) {
    auto ithead = fwd.begin();
    for (; ithead != fwd.end(); ++ithead) {
        if (bwd.find(ithead->first) == bwd.end() &&
            fwd.find(ithead->second) != fwd.end()) {
            break;
        }
    }
    return ithead;
}

int main() {
    int n;
    strpair plist, plist_back;
    vector<string*> oddlist, evenlist;
    string fst(10, '\0'), snd(10, '\0');

    cin.tie(0);
    ios::sync_with_stdio(false);

    cin >> n;
    plist.reserve(n);
    plist_back.reserve(n);

    for (int i = 0; i < n; i++) {
        cin >> fst >> snd;
        plist.emplace(fst, snd);
        plist_back.emplace(snd, fst);
    }

    auto ithead = find_head(plist, plist_back);
    auto ittail = plist.find(find_head(plist_back, plist)->second);
    auto itheadnxt = plist.find(plist.find("0")->second);

    plist.emplace(ittail->second, "0");

    while (ithead != plist.end()) {
        oddlist.push_back((string*) &(ithead->first));
        if (ithead->second == "0") {
            break;
        } else {
            ithead = plist.find(ithead->second);
        }
    }

    while (itheadnxt != plist.end()) {
        evenlist.push_back((string*) &(itheadnxt->first));
        if (itheadnxt->second == "0") {
            break;
        } else {
            itheadnxt = plist.find(itheadnxt->second);
        }
    }

    int nreal = oddlist.size() + evenlist.size();

    if (nreal <= 2) {
        cout << ithead->first << " " << ittail->second << '\n';
        cout << (n - 2) << '\n';
    } else {
        cout << *oddlist[0];
        for (int i = 1; i < nreal; i++) {
            cout << ' ' << *(i % 2 ? evenlist : oddlist)[i / 2];
        }
        cout << '\n' << (n - nreal) << '\n';
    }
}
