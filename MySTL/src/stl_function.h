#ifndef MYSTL_STL_FUNCTION_H
#define MYSTL_STL_FUNCTION_H
namespace MySTL {

template<class Arg, class Result>
struct unary_function {
    typedef Arg argument_type;
    typedef Result result_type;
};


    template<class Arg1, class Arg2, class Result>
    struct binary_function {
        typedef Arg1 first_argument_type;
        typedef Arg2 second_argument_type;
        typedef Result result_type;
    };


    template<class Tp>
    struct plus : public binary_function<Tp, Tp, Tp> {
        Tp operator()(const Tp &x, const Tp &y) const { return x + y; }
    };

    template<class Tp>
    struct minus : public binary_function<Tp, Tp, Tp> {
        Tp operator()(const Tp &x, const Tp &y) const { return x - y; }
    };

    template<class Tp>
    struct multiplies : public binary_function<Tp, Tp, Tp> {
        Tp operator()(const Tp &x, const Tp &y) const { return x * y; }
    };

    template<class Tp>
    struct divides : public binary_function<Tp, Tp, Tp> {
        Tp operator()(const Tp &x, const Tp &y) const { return x / y; }
    };


// identity_element (not part of the C++ standard).

    template<class Tp>
    inline Tp identity_element(plus<Tp>) { return Tp(0); }

    template<class Tp>
    inline Tp identity_element(multiplies<Tp>) { return Tp(1); }

    template<class Tp>
    struct modulus : public binary_function<Tp, Tp, Tp> {
        Tp operator()(const Tp &x, const Tp &y) { return x % y; }
    };

    template<class Tp>
    struct negate : public unary_function<Tp, Tp> {
        Tp operator()(const Tp &x) { return -x; }
    };

    template<class Tp>
    struct equal_to : public binary_function<Tp, Tp, bool> {
        Tp operator()(const Tp &x, const Tp &y) { return x == y; }
    };

    template<class Tp>
    struct not_equal_to : public binary_function<Tp, Tp, bool> {
        Tp operator()(const Tp &x, const Tp &y) { return x != y; }
    };

    template<class Tp>
    struct greater : public binary_function<Tp, Tp, bool> {
        bool operator()(const Tp &x, const Tp &y) const { return x > y; }
    };

    template<class Tp>
    struct less : public binary_function<Tp, Tp, bool> {
        bool operator()(const Tp &x, const Tp &y) const { return x < y; }
    };

    template<class Tp>
    struct greater_equal : public binary_function<Tp, Tp, bool> {
        bool operator()(const Tp &x, const Tp &y) const { return x >= y; }
    };

    template<class Tp>
    struct less_equal : public binary_function<Tp, Tp, bool> {
        bool operator()(const Tp &x, const Tp &y) const { return x <= y; }
    };

    template<class Tp>
    struct logical_and : public binary_function<Tp, Tp, bool> {
        bool operator()(const Tp &x, const Tp &y) const { return x && y; }
    };

    template<class Tp>
    struct logical_or : public binary_function<Tp, Tp, bool> {
        bool operator()(const Tp &x, const Tp &y) const { return x || y; }
    };

    template<class Tp>
    struct logical_not : public unary_function<Tp, bool> {
        bool operator()(const Tp &x) const { return !x; }
    };

//TODO 这里开始的没太看明白怎么用
    template<class Predicate>
    class unary_negate : public unary_function<typename Predicate::argument_type, bool> {
    protected:
        Predicate M_pred;
    public:
        explicit unary_negate(const Predicate &x) : M_pred(x) {}

        bool operator()(const typename Predicate::argument_type &x) const { return !M_pred(x); }
    };

    template<class Predicate>
    inline unary_negate<Predicate> not1(const Predicate *pred) { return unary_negate<Predicate>(pred); }

    template<class Predicate>
    class binary_negate : public binary_function<typename Predicate::first_argument_type,
            typename Predicate::second_argument_type, bool> {
    protected:
        Predicate M_pred;
    public:
        explicit binary_negate(const Predicate &x) : M_pred(x) {}

        bool operator()(const typename Predicate::first_argument_type &x,
                        const typename Predicate::second_argument_type &y) const { return !M_pred(x, y); }
    };

    template<class Predicate>
    inline binary_negate<Predicate> not2(const Predicate &pred) { return binary_negate<Predicate>(pred); }

    template<class Operation>
    class binder1st : unary_function<typename Operation::second_argument_type,
            typename Operation::result_type> {
    protected:
        Operation op;
        typename Operation::first_argument_type value;
    public:
        binder1st(const Operation &x, const typename Operation::first_argument_type &y)
                : op(x), value(y) {}

        typename Operation::result_type operator()(const typename Operation::second_argumen_type &x) const {
            return op(value, x);
        }
    };

    template<class Operation, class Tp>
    inline binder1st<Operation> bind1st(const Operation &fn, const Tp &x) {
        typedef typename Operation::first_argument_type Arg1_type;
        return binder1st<Operation>(fn, Arg1_type(x));
    }

    template<class Operation>
    class binder2end : unary_function<typename Operation::first_argument_type,
            typename Operation::result_type> {
    protected:
        Operation op;
        typename Operation::second_argument_type value;
    public:
        binder2end(const Operation &x, const typename Operation::second_argument_type &y)
                : op(x), value(y) {}

        typename Operation::result_type operator()(const typename Operation::first_argumen_type &x) const {
            return op(value, x);
        }
    };

    template<class Operation, class Tp>
    inline binder2end<Operation> bind2end(const Operation &fn, const Tp &x) {
        typedef typename Operation::second_argument_type Arg2_type;
        return binder2end<Operation>(fn, Arg2_type(x));
    }


    template<class Operation1, class Operation2>
    class unary_compose : unary_function<typename Operation2::argument_type,
            typename Operation1::result_type> {
    protected:
        Operation1 M_fn1;
        Operation2 M_fn2;
    public:
        unary_compose(const Operation1 &x, const Operation2 &y)
                : M_fn1(x), M_fn2(y) {}

        typename Operation1::result_type
        operator()(const typename Operation2::argument_type &x) const { return M_fn1(M_fn2(x)); }
    };

    template<class Operation1, class Operation2>
    inline unary_compose<Operation1, Operation2>
    compose1(const Operation1 &fn1, const Operation2 &fn2) { return unary_compose<Operation1, Operation2>(fn1, fn2); }


    template<class Operation1, class Operation2, class Operation3>
    class binary_compose : public unary_function<typename Operation2::argument_type,
            typename Operation1::result_type> {
    protected:
        Operation1 M_fn1;
        Operation2 M_fn2;
        Operation3 M_fn3;

    public:
        binary_compose(const Operation1 &x, const Operation2 &y, const Operation3 &z)
                : M_fn1(x), M_fn2(y), M_fn3(z) {}

        typename Operation1::result_type
        operator()(const typename Operation2::argument_type &x) const { return M_fn1(M_fn2(x), M_fn3(x)); }
    };

    template<class Operation1, class Operation2, class Operation3>
    inline binary_compose<Operation1, Operation2, Operation3>
    compose2(const Operation1 &fn1, const Operation2 &fn2, const Operation3 &fn3)
    {
        return binary_compose<Operation1, Operation2, Operation3>(fn1, fn2, fn3);
    }

template <class Arg, class Result>
class pointer_to_unary_function : public unary_function<Arg, Result>
{
protected:
    Result (*M_ptr)(Arg);
public:
    pointer_to_unary_function() {}
    explicit pointer_to_unary_function(Result (*x)(Arg)):M_ptr(x) {}
    Result operator()(Arg x) const { return M_ptr(x); }
};

template <class Arg, class Result>
inline pointer_to_unary_function<Arg, Result> ptr_fun(Result(*x)(Arg))
{ return pointer_to_unary_function<Arg, Result>(x); }

template <class Arg1, class Arg2, class Result>
class pointer_to_binary_function : public binary_function<Arg1, Arg2, Result>
{
protected:
    Result (*M_ptr)(Arg1, Arg2);
public:
    pointer_to_binary_function() {}
    explicit pointer_to_binary_function(Result (*x)(Arg1, Arg2)) : M_ptr(x) {}

    Result operator()(Arg1 x, Arg2 y) const
    { return M_ptr(x, y); }
};

template <class Arg1, class Arg2, class Result>
inline pointer_to_binary_function<Arg1, Arg2, Result>
ptr_fun(Result (*x)(Arg1, Arg2))
{
    return pointer_to_binary_function<Arg1, Arg2, Result>(x);
}


//TODO -end 此部分未看懂怎么用



//identity
template <class Tp>
struct Identity : public unary_function<Tp, Tp>
{ const Tp& operator()(const Tp& x) const { return x; } };

template <class Tp> struct identity : public Identity<Tp> {};


//select1st and select2nd
template <class Pair>
struct Select1st : public unary_function<Pair, typename Pair::first_type>
{ const typename Pair::first_type& operator()(const Pair& x) const { return x.first; } };

template <class Pair>
struct Select2nd : public unary_function<Pair, typename Pair::second_type>
{ const typename Pair::second_type& operator()(const Pair& x) const { return x.second; } };

template <class Pair> struct select1st:public Select1st<Pair> {};
template <class Pair> struct select2nd:public Select2nd<Pair> {};

// project1st and project2nd
template <class Arg1, class Arg2>
struct Project1st : public binary_function<Arg1, Arg2, Arg1>
{
    Arg1 operator()(const Arg1& x, const Arg2&) const { return x; }
};

template <class Arg1, class Arg2>
struct Project2nd : binary_function<Arg1, Arg2, Arg2>
{
    Arg2 operator()(const Arg1&, const Arg2& y) const { return y;}
};

template <class Arg1, class Arg2>
struct project1st : public Project1st<Arg1, Arg2> {};

template <class Arg1, class Arg2>
struct project2nd : public Project2nd<Arg1, Arg2> {};



template <class Result>
struct Constant_void_fun
{
    typedef Result result_type;
    result_type M_val;

    Constant_void_fun(const result_type& v) : M_val(v) {}
    const result_type& operator()() const { return M_val; }
};

template <class Result, class Argument>
struct Constant_unary_fun
{
    typedef Argument argument_type;
    typedef Result result_type;
    result_type M_val;

    Constant_unary_fun(const result_type& v) : M_val(v) {}
    const result_type& operator()(const Argument&) const { return M_val; }
};

template <class Result, class Arg1, class Arg2>
struct Constant_binary_fun
{
    typedef Arg1 first_argument_type;
    typedef Arg2 second_argument_type;
    typedef Result result_type;
    Result M_val;

    Constant_binary_fun(const Result& v) : M_val(v) {}
    const result_type& operator()(const Arg1&, const Arg2&) const { return M_val; }
};

template <class Result>
struct constant_void_fun : public Constant_void_fun<Result>
{ constant_void_fun(const Result& v): Constant_void_fun<Result>(v) {} };

template <class Result, class Argument = Result>
struct constant_unary_fun : Constant_unary_fun<Result, Argument>
{ constant_unary_fun(const Result& v) : Constant_unary_fun<Result, Argument>(v) {} };


template <class Result, class Arg1 = Result, class Arg2 = Arg1>
struct constant_binary_fun : public Constant_binary_fun<Result, Arg1, Arg2>
{ constant_binary_fun(const Result& v) : Constant_binary_fun<Result, Arg1, Arg2>(v) {} };


template <class Result>
inline constant_void_fun<Result> constant0(const Result& val)
{ return constant_void_fun<Result>(val); }

template <class Result>
inline constant_unary_fun<Result, Result> constant1(const Result& val)
{ return constant_unary_fun<Result, Result>(val); }

template <class Result>
inline constant_binary_fun<Result, Result, Result> constant2(const Result& val)
{ return constant_binary_fun<Result, Result, Result>(val); }




// Adaptor function objects

    template <class Ret, class Tp>
    class mem_fun_t : public unary_function<Tp*,Ret> {
    public:
        explicit mem_fun_t(Ret (Tp::*pf)()) : M_f(pf) {}
        Ret operator()(Tp* p) const { return (p->*M_f)(); }
    private:
        Ret (Tp::*M_f)();
    };

    template <class Ret, class Tp>
    class const_mem_fun_t : public unary_function<const Tp*,Ret> {
    public:
        explicit const_mem_fun_t(Ret (Tp::*pf)() const) : M_f(pf) {}
        Ret operator()(const Tp* p) const { return (p->*M_f)(); }
    private:
        Ret (Tp::*M_f)() const;
    };


    template <class Ret, class Tp>
    class mem_fun_ref_t : public unary_function<Tp,Ret> {
    public:
        explicit mem_fun_ref_t(Ret (Tp::*pf)()) : M_f(pf) {}
        Ret operator()(Tp& r) const { return (r.*M_f)(); }
    private:
        Ret (Tp::*M_f)();
    };

    template <class Ret, class Tp>
    class const_mem_fun_ref_t : public unary_function<Tp,Ret> {
    public:
        explicit const_mem_fun_ref_t(Ret (Tp::*pf)() const) : M_f(pf) {}
        Ret operator()(const Tp& r) const { return (r.*M_f)(); }
    private:
        Ret (Tp::*M_f)() const;
    };

    template <class Ret, class Tp, class Arg>
    class mem_fun1_t : public binary_function<Tp*,Arg,Ret> {
    public:
        explicit mem_fun1_t(Ret (Tp::*pf)(Arg)) : M_f(pf) {}
        Ret operator()(Tp* p, Arg x) const { return (p->*M_f)(x); }
    private:
        Ret (Tp::*M_f)(Arg);
    };

    template <class Ret, class Tp, class Arg>
    class const_mem_fun1_t : public binary_function<const Tp*,Arg,Ret> {
    public:
        explicit const_mem_fun1_t(Ret (Tp::*pf)(Arg) const) : M_f(pf) {}
        Ret operator()(const Tp* p, Arg x) const
        { return (p->*M_f)(x); }
    private:
        Ret (Tp::*M_f)(Arg) const;
    };

    template <class Ret, class Tp, class Arg>
    class mem_fun1_ref_t : public binary_function<Tp,Arg,Ret> {
    public:
        explicit mem_fun1_ref_t(Ret (Tp::*pf)(Arg)) : M_f(pf) {}
        Ret operator()(Tp& r, Arg x) const { return (r.*M_f)(x); }
    private:
        Ret (Tp::*M_f)(Arg);
    };

    template <class Ret, class Tp, class Arg>
    class const_mem_fun1_ref_t : public binary_function<Tp,Arg,Ret> {
    public:
        explicit const_mem_fun1_ref_t(Ret (Tp::*pf)(Arg) const) : M_f(pf) {}
        Ret operator()(const Tp& r, Arg x) const { return (r.*M_f)(x); }
    private:
        Ret (Tp::*M_f)(Arg) const;
    };


template <class Tp>
class mem_fun_t<void, Tp> : public unary_function<Tp*,void> {
public:
  explicit mem_fun_t(void (Tp::*pf)()) : M_f(pf) {}
  void operator()(Tp* p) const { (p->*M_f)(); }
private:
  void (Tp::*M_f)();
};

template <class Tp>
class const_mem_fun_t<void, Tp> : public unary_function<const Tp*,void> {
public:
  explicit const_mem_fun_t(void (Tp::*pf)() const) : M_f(pf) {}
  void operator()(const Tp* p) const { (p->*M_f)(); }
private:
  void (Tp::*M_f)() const;
};

template <class Tp>
class mem_fun_ref_t<void, Tp> : public unary_function<Tp,void> {
public:
  explicit mem_fun_ref_t(void (Tp::*pf)()) : M_f(pf) {}
  void operator()(Tp& r) const { (r.*M_f)(); }
private:
  void (Tp::*M_f)();
};

template <class Tp>
class const_mem_fun_ref_t<void, Tp> : public unary_function<Tp,void> {
public:
  explicit const_mem_fun_ref_t(void (Tp::*pf)() const) : M_f(pf) {}
  void operator()(const Tp& r) const { (r.*M_f)(); }
private:
  void (Tp::*M_f)() const;
};

template <class Tp, class Arg>
class mem_fun1_t<void, Tp, Arg> : public binary_function<Tp*,Arg,void> {
public:
  explicit mem_fun1_t(void (Tp::*pf)(Arg)) : M_f(pf) {}
  void operator()(Tp* p, Arg x) const { (p->*M_f)(x); }
private:
  void (Tp::*M_f)(Arg);
};

template <class Tp, class Arg>
class const_mem_fun1_t<void, Tp, Arg>
  : public binary_function<const Tp*,Arg,void> {
public:
  explicit const_mem_fun1_t(void (Tp::*pf)(Arg) const) : M_f(pf) {}
  void operator()(const Tp* p, Arg x) const { (p->*M_f)(x); }
private:
  void (Tp::*M_f)(Arg) const;
};

template <class Tp, class Arg>
class mem_fun1_ref_t<void, Tp, Arg>
  : public binary_function<Tp,Arg,void> {
public:
  explicit mem_fun1_ref_t(void (Tp::*pf)(Arg)) : M_f(pf) {}
  void operator()(Tp& r, Arg x) const { (r.*M_f)(x); }
private:
  void (Tp::*M_f)(Arg);
};

template <class Tp, class Arg>
class const_mem_fun1_ref_t<void, Tp, Arg>
  : public binary_function<Tp,Arg,void> {
public:
  explicit const_mem_fun1_ref_t(void (Tp::*pf)(Arg) const) : M_f(pf) {}
  void operator()(const Tp& r, Arg x) const { (r.*M_f)(x); }
private:
  void (Tp::*M_f)(Arg) const;
};



    template <class Ret, class Tp>
    inline mem_fun_t<Ret,Tp> mem_fun(Ret (Tp::*f)())
    { return mem_fun_t<Ret,Tp>(f); }

    template <class Ret, class Tp>
    inline const_mem_fun_t<Ret,Tp> mem_fun(Ret (Tp::*f)() const)
    { return const_mem_fun_t<Ret,Tp>(f); }

    template <class Ret, class Tp>
    inline mem_fun_ref_t<Ret,Tp> mem_fun_ref(Ret (Tp::*f)())
    { return mem_fun_ref_t<Ret,Tp>(f); }

    template <class Ret, class Tp>
    inline const_mem_fun_ref_t<Ret,Tp> mem_fun_ref(Ret (Tp::*f)() const)
    { return const_mem_fun_ref_t<Ret,Tp>(f); }

    template <class Ret, class Tp, class Arg>
    inline mem_fun1_t<Ret,Tp,Arg> mem_fun(Ret (Tp::*f)(Arg))
    { return mem_fun1_t<Ret,Tp,Arg>(f); }

    template <class Ret, class Tp, class Arg>
    inline const_mem_fun1_t<Ret,Tp,Arg> mem_fun(Ret (Tp::*f)(Arg) const)
    { return const_mem_fun1_t<Ret,Tp,Arg>(f); }

    template <class Ret, class Tp, class Arg>
    inline mem_fun1_ref_t<Ret,Tp,Arg> mem_fun_ref(Ret (Tp::*f)(Arg))
    { return mem_fun1_ref_t<Ret,Tp,Arg>(f); }

    template <class Ret, class Tp, class Arg>
    inline const_mem_fun1_ref_t<Ret,Tp,Arg>
    mem_fun_ref(Ret (Tp::*f)(Arg) const)
    { return const_mem_fun1_ref_t<Ret,Tp,Arg>(f); }

    template <class Ret, class Tp, class Arg>
    inline mem_fun1_t<Ret,Tp,Arg> mem_fun1(Ret (Tp::*f)(Arg))
    { return mem_fun1_t<Ret,Tp,Arg>(f); }

    template <class Ret, class Tp, class Arg>
    inline const_mem_fun1_t<Ret,Tp,Arg> mem_fun1(Ret (Tp::*f)(Arg) const)
    { return const_mem_fun1_t<Ret,Tp,Arg>(f); }

    template <class Ret, class Tp, class Arg>
    inline mem_fun1_ref_t<Ret,Tp,Arg> mem_fun1_ref(Ret (Tp::*f)(Arg))
    { return mem_fun1_ref_t<Ret,Tp,Arg>(f); }

    template <class Ret, class Tp, class Arg>
    inline const_mem_fun1_ref_t<Ret,Tp,Arg>
    mem_fun1_ref(Ret (Tp::*f)(Arg) const)
    { return const_mem_fun1_ref_t<Ret,Tp,Arg>(f); }
}
#endif //MYSTL_STL_FUNCTION_H
