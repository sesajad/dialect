#include <functional>
#include <map>
#include <string>
#include <set>
#include <tuple>

typedef std::string Name;
typedef std::string Text;

template<A, B, R>
struct Functor {
    int priority;
    
    virtual void pre(Any a) = 0;
    virtual void post(Any a) = 0;
    virtual bool finished() = 0;
    virtual std::optional<R> result() = 0;
};

