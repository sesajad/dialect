#include "../core/expr.h"
#include "../functors.hpp"
using namespace expr;

T1ContextSensitive parseExpr(generator<Token> tokens) {
    Stack runningFunctors = Stack of make_unique<functors::Concat>();
    for (t : tokens) {
        if (t.)
    }
}