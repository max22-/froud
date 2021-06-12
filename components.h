#ifndef COMPONENTS_H
#define COMPONENTS_H

template <typename T>
class Generator : public Node {
public:
    Generator(T (*f)()) : f(f) {}
    Input<Bang> in;
    Output<T> out;
protected:
    T (*f)();
private:
    bool isFireable() override { return in.isFull(); }
    void process() override {
        //std::cout << "source" << std::endl;
        out.put(f());
    }
};


template <typename T>
class Sink : public Node {
public:
    Sink(void (*f)(T)) : f(f) { }
    Input<T> in;

private:
    bool isFireable() override { return in.isFull(); };
    void (*f)(T);
    void process() override {
        f(in.get());
    }
};

template <typename A, typename B>
class Function1i : public Node {
public:
    Function1i(B (*f)(A a)) : f(f) { }

    Input<A> in;
    Output<B> out;

protected:

    B (*f)(A a);

private:
    bool isFireable() override { return in.isFull(); };
    void process() override {
        out.put(f(in.get()));
    }
};

template <typename A, typename B, typename C>
class Function2i : public Node {
public:
    Function2i(C (*f)(A a, B b)) : f(f) { }

    Input<A> in0;
    Input<B> in1;
    Output<C> out;

protected:

    C (*f)(A a, B b);

private:
    bool isFireable() override { return in0.isFull() && in1.isFull(); }
    void process() override {
        out.put(f(in0.get(), in1.get()));
    }
};

template <typename T>
class Constant {
public:
    Constant(T c) : c(c) { }
    Output<T> out;
private:
    void isFireable() override { return true; }
    void process() override() {
        out.put(c);
    }
    const T c;
};

class Clock : public Node {
public:
    Output<Bang> out;
private:
    bool isFireable() override { return true; }
    void process() override { out.put(Bang()); }
};

class Toggle : public Node {
public:
    Input<Bang> in;
    Output<bool> out;
private:
    bool isFireable() override { return in.isFull(); }
    void process() override { 
        state = !state;
        out.put(state);
    }

    bool state = false;
};

template <typename T1, typename T2>
class Cast : public Node {
public:
    Input<T1> in;
    Output<T2> out;
private:
    bool isFireable() override { return in.isFull(); }
    void process() override {
        out.put((T2)in.get());
    }
};

#endif