#include <functional>
#include <algorithm>
#include <optional>
#include <map>
#include <string>
#include <set>
#include <tuple>

#include <iostream>

namespace token {
    typedef std::string Name;
    typedef std::string Text;
}

namespace match {
    struct Result {
        int length;
        std::map<Name, Text> captures;
    };

    typedef std::function<std::optional<Result>(Text)> Matcher;

    // we define t4 as exact grammars
    struct T1ContextSensitive {
        virtual Matcher match = 0; 
    }
    struct T2ContextFree {

    }
    struct T3Regular {

    }
    struct T4Exact {
        T4Exact(Text _text) : text(_text) {
            std::cout << "Created with " << text << " at @" << this << std::endl;
        };
        ~T4Exact() {
            std::cout << "Deleted with " << text << " at @" << this << std::endl;
        }
        Text text;
    }

    // A. string operations
    /**
        concats two exact terms.
    */
    unique_ptr<T4Exact> concat(unique_ptr<T4Exact> a, unique_ptr<T4Exact> b) {
        return T4Exact(a->text + b->text);
    }

    /**
        repeats the term t, n times.
        note that it may leads to exception
    */
    unique_ptr<T4Exact> repeat(unique_ptr<T4Exact> t, unique_ptr<T4Exact> n) {
        stringstream res = "";
        int n_value = stoi(n);
        for (int i = 0; i < n_value; i ++)
            res << t->text;
        return T4Exact(res);
    }

    
}


struct RegExpr : PatternExpr {
    RegExpr(std::string repr) : repr(repr), PatternExpr([] (Text t) -> std::optional<Result> { 
        return std::optional<Result>(); // TODO
        }) {}
    std::string repr;
};

struct TextExpr : RegExpr {
    Text text;
    std::string regexEscape(Text text) {
        std::string ret = "";
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
    std::set<Name> unbounds;
    std::map<Name, Text> bounds;
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
    PlaceholderExpr(Name n) : missings(std::set(n));
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

int main() {
    std::unique_ptr match:: t("Hi.");
    std::cout << (t + star(t)).repr << std::endl;
}