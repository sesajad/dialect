#include "functor.hpp"

#include <string>
using namespace std;
namespace ops {
    struct Concat : Operation<string, string> {
        stringstream ss;
        Concat () : priority(1) { }
        
        virtual void pre(string a) {
            // todo raise exception
        }
        virtual void post(string b) {
            ss << b;
        };
        virtual bool finished() {
            return true;
        };
        virtual std::optional<string> result() {
            make_optional<string>(res.str());
        };
    };
}