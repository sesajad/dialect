#include <functional>
#include <map>
#include <string>
#include <set>
#include <tuple>

typedef std::string Name;
typedef std::string Text;

template<X, Y>
struct Operation {
    int priority;
    
    virtual void pre(X x) = 0;
    virtual void post(X x) = 0;
    virtual bool finished() = 0;
    virtual std::optional<Y> result() = 0;
};

template<X, R>
struct SectionOperation : Operation<X, X> {
    SectionOperation(std::function<R(X)> to, 
                    std::function<R(X)> from,
                    Operation<R, R> ropr) { }
}

template<X, R>
struct MorphOperation : Operation<X, R> {
    MorphOperation(std::function<R(X)> to, 
                   Operation<R, R> ropr) { }
}