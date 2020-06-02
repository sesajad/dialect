#include <functional>
#include <map>
#include <string>
#include <set>
#include <tuple>

#define _FT__ASSERT

typedef std::string Name;
typedef std::string Text;

struct Range {
    Text reference;
    std::set<std::tuple<int, int>> ranges;
};

struct Result {
    Range match;
    std::map<Name, Text> captures;
};

typedef std::function<std::set<Result>(Text)> Pattern;
typedef std::function<Text(std::map<Name, Text>)> Value;

union Expr {
    Pattern p;
    Value v;
};

struct InAndIgnore {
    Pattern in;
    Pattern ignoring;
};

struct Rule: InAndIgnore {
    Name name;

    Pattern find;
    Value replace;
};

#ifdef _FT__ASSERT
struct Assertion: InAndIgnore {
    Name name;

    Pattern find;
    virtual bool assert(Text t) = 0;
};
#endif

struct Scope : InAndIgnore {
    Name name;

    std::set<Scope> superScopes;
    std::set<Scope> subScopes;

    std::map<Name, Expr> definitions;
    std::set<Rule> rules;

    #ifdef _FT__ASSERT
    std::set<Assertion> asserts;
    #endif
};


void evaluate(std::weak_ptr<Scope> s) {
    std::map(evaluate, s->superScopes);
    try {
        std::foreach(lambda (x): x.assert(), s->asserts);
    }
    

    std::map(evaluate, s->superScopes);
}