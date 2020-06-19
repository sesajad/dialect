// ROP
// isomorphism reversible representation
// morphism reduce
// section represent

// template<typename O, typename R>
// struct repr {
//     O o;
//     repr(O _o) o(_o);
//     virtual R get() = 0;
// }

// // general 

// // 
// template<typename O>
// operator repr<O, std::string>(repr<O, int> i) {
//     return repr<O, std::string>("42");
// }


#include <iostream>

struct C {
    int a;
};

struct B {
    int a;
    
    operator C() {
        std::cout << "b => c" << std::endl;
        return C{ a };
    }
};

struct A {
    int a;
    operator B() {
        std::cout << "b => c" << std::endl;
        return B{ a };
    }

};




int main() {
    A a { 11 };
    B b = dynamic_cast<B&>(a);
    std::cout << b.a << std::endl;
}