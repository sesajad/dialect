#include "expr.hpp"
using namespace expr;

int main() {
    auto t = make_unique<T4Exact>("Hi.");
    auto n = make_unique<T4Exact>("3");
    auto b = make_unique<T4Exact>("ok");
    cout << "start!" << endl;
    auto res = naming(len(concat(repeat(move(t), move(n)), move(b))), "shit")->match("11 asghar");
    if (res) {
        cout << "result : "<< res.value().length << ", " << res.value().captures["shit"] << endl;

    }
    cout << "done!" << endl;
}