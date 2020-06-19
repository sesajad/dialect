#ifndef __TOKENS_HPP
#define __TOKENS_HPP

namespace token {
    enum Type {
        NAME, TEXT
    }
    typedef string Name;
    typedef string Text;
    struct Token { // abstract
        operator string
    }
}

#endif