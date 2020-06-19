#include <functional>
#include <algorithm>
#include <optional>
#include <map>
#include <string>
#include <set>
#include <tuple>
#include <memory>
#include <sstream>
#include "../token/tokens.hpp"
#include <iostream>

using namespace std;

namespace expr {
    struct Result {
        long unsigned int length;
        map<token::Name, token::Text> captures;
        // todo: capture lists?
    };

    struct Matcher {
        virtual set<token::Name> names() const = 0;
        virtual optional<Result> match(token::Text input) = 0;
    };


    // we define t4 as exact grammars
    struct T1ContextSensitive {
    };
    struct T2ContextFree {
    };
    struct T3Regular {
        string regex;
        T3Regular(string _regex) : regex(_regex) {}
    };
    struct T4Exact : Matcher {
        map<token::Name, token::Text> keyvals;
        token::Text text;

        T4Exact(token::Text _text, map<token::Name, token::Text>  _keyvals) : text(_text), keyvals(_keyvals) {
            cout << "Created with " << text << " at @" << this << " -w keyvals" << endl;
        }
        T4Exact(token::Text _text) : text(_text) {
            cout << "Created with " << text << " at @" << this << endl;
        }
        T4Exact(const T4Exact &other) : text(other.text), keyvals(other.keyvals) {
            cout << "Copied from @" << &other << " to @" << this << endl;
        }
        T4Exact(T4Exact &&other) : text(other.text), keyvals(other.keyvals) {
            cout << "Moved from @" << &other << " to @" << this << endl;
        }
        ~T4Exact() {
            cout << "Deleted with " << text << " at @" << this << endl;
        }

        // match functions
        virtual set<token::Name> names() const {
            set<token::Name> s;
            transform(keyvals.begin(), keyvals.end(),
                inserter(s, s.end()),
                [](auto pair){ return pair.first; });
            return move(s);
        }

        virtual optional<Result> match(token::Text input) {
            if (input.find(text) == 0) {
                return make_optional(Result{text.length(), keyvals});
            } else {
                return nullopt;
            }
        }

        // upcasts
        operator T3Regular() const {
            stringstream ret;
            for (char c : text) 
                switch (c) {
                    case '\\': case '[': case '*': case '.': case ']': case '^': // TODO
                        ret << '\\';
                    default:
                        ret << c;
                }
            return T3Regular(ret.str());
        }
    };

    // A. string operations
    // TODO, make it template
    /**
        concats two exact terms.
    */
    unique_ptr<T4Exact> concat(unique_ptr<T4Exact> a, unique_ptr<T4Exact> b) {
        a->keyvals.insert(b->keyvals.begin(), b->keyvals.end());
        return make_unique<T4Exact>(a->text + b->text, move(a->keyvals));
    }

    /**
        repeats the term t, n times
        assumes: n is int.
    */
    unique_ptr<T4Exact> repeat(unique_ptr<T4Exact> t, unique_ptr<T4Exact> n) {
        stringstream res;
        int n_value = stoi(n->text);
        for (int i = 0; i < n_value; i ++)
            res << t->text;
        return make_unique<T4Exact>(res.str(), move(n->keyvals));
    }

    /**
        keyvals a constant term. it's useless but shows that a naming doesn't reduce T4 to T3
    */
    unique_ptr<T4Exact> naming(unique_ptr<T4Exact> p, const token::Name& name) {
        p->keyvals.insert(pair<token::Text, token::Name>(name, p->text));
        return make_unique<T4Exact>(p->text, move(p->keyvals));
    }

    // B. integer operations
    /**
        adds two numbers.
        assumes a, b are ints.
    */
    unique_ptr<T4Exact> add(unique_ptr<T4Exact> a, unique_ptr<T4Exact> b) {
        return make_unique<T4Exact>(to_string(stoi(a->text) + stoi(b->text)));
    }

    /**
        multiplies two numbers.
        assumes a, b are ints.
    */
    unique_ptr<T4Exact> mul(unique_ptr<T4Exact> a, unique_ptr<T4Exact> b) {
        return make_unique<T4Exact>(to_string(stoi(a->text) * stoi(b->text)));
    }
    
    /**
        returns length of an string
    */
    unique_ptr<T4Exact> len(unique_ptr<T4Exact> a) {
        return make_unique<T4Exact>(to_string(a->text.length()));
    }

    // C. regular operations
    T3Regular concat(const T3Regular& a, const T3Regular& b) {
        return T3Regular(a.regex + b.regex);
    }
    T3Regular star(const T3Regular& t) {
        return T3Regular("(" + t.regex + ")*");
    }
    T3Regular regex_or(const T3Regular& a, const T3Regular& b) {
        return T3Regular("(" + a.regex + ")|(" + b.regex + ")");
    }
    T3Regular naming(const T3Regular& p, const token::Name& name) {
        // TODO
        return T3Regular("(naming-group" + name + p.regex + ")");
    }

    
    template <typename T> 
    struct Unbound {
        set<Name> unbounds;
        map<Name, Text> bounds;
        Unbound bind(Name n, Text t) {
            unbounds -= n;
            bounds.insert(n, t);
        }
        operator bool() {
            return !unbounds;
        }
        virtual T get() = 0;
    };

    struct Placeholder : Unbound<T4Exact> {
        PlaceholderExpr(Name n) : missings(set(n));
        T get() {
            return bounds(n);
        }
    }
}

/*
struct RegExpr : PatternExpr {
    RegExpr(string repr) : repr(repr), PatternExpr([] (Text t) -> optional<Result> { 
        return optional<Result>(); // TODO
        }) {}
    string repr;
};

struct TextExpr : RegExpr {
    Text text;
    string regexEscape(Text text) {
        string ret = "";
        for (char c : text) 
            switch (c) {
                case '\\': case '[': case '*': case '.': case ']': case '^': // TODO
                    ret += '\\';
                default:
                    ret += c;
            }
        return ret;
    } 
    TextExpr(Text text) : text(text), RegExpr(regexEscape(text)) {}
};

TextExpr operator+(const TextExpr& a, const TextExpr& b) {
    return TextExpr(a.text + b.text);
}

RegExpr operator+(const RegExpr& a, const RegExpr& b) {
    return RegExpr(a.repr + b.repr);
}

PatternExpr operator+(const PatternExpr& a, const PatternExpr& b) {
    return PatternExpr([a, b] (Text t) {
        auto res = a.pattern(t);
        if (!res)
            return b.pattern(t.substr(res->length));
        else
            return res;
    });
}

RegExpr operator|(const RegExpr& a, const RegExpr& b) {
    return RegExpr("(" + a.repr + ")|(" + b.repr + ")");
}

RegExpr star(const RegExpr& a) {
    return RegExpr("(" + a.repr + ")*");
}

template <typename T> 
struct Unbound {
    set<Name> unbounds;
    map<Name, Text> bounds;
    void bind(Name n, Text t) {
        unbounds -= n;
        bounds.insert(n, t);
    }
    operator bool() {
        return !unbounds;
    }
    virtual T get() = 0;
};

struct PlaceholderExpr : Unbound<TextExpr> {
    PlaceholderExpr(Name n) : missings(set(n));
    T get() {
        return bounds(n);
    }
}

Unbound<PatternExpr>& as(const PatternExpr& f, const Unbound<PatternExpr>& u) {
    return PatternExpr([] (Text t) {
        auto res = a.pattern(t);
        if (!res)
            return b.pattern(t.substring(res->length));
        else
            return res;
    });
}
*/
