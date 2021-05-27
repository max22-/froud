#ifndef FROUD_H
#define FROUD_H

#include <iostream>
#include <unordered_set>
#include <string>

#define FROUD_MAJOR 0
#define FROUD_MINOR 1
#define FROUD_PATCH 0

template <typename T>
class Input;

template <typename T>
class Output {
public:
    Output() {}

    #warning make put() inaccessible from users
    void put(T data) {
        //std::cout << "Output::put " << wires.size() << " wires" << std::endl;
        for(auto wire: wires)
            wire->acceptData(data);
    }

private:
    Output& operator=(const Output&) = delete;



    void connect(Input<T>* i) {
        //std::cout << "Output::connect()" << std::endl;
        wires.insert(i);
        //std::cout << "output::connect() : " << wires.size() << " wires" << std::endl;
    }

    void disconnect(Input<T>* i) {
        wires.erase(i);
    }

    std::unordered_set<Input<T>*> wires;

    friend class Input<T>;
};


class InputBase {
public:
    InputBase() : _isFull(false) {}
    virtual bool isFull() final {
        return _isFull;
    }
protected:
    bool _isFull;
};


template <typename T>
class Input : public InputBase {
public:
    Input() {}
    T get() { _isFull = false; return data; }
    Input<T> operator=(Output<T>& o) {
        #warning Implement disconnection when input is destructed
        o.connect(this);
        return *this;
    }
private:
    Input<T> operator=(const Input<T>& i) = delete;

    void acceptData(T data) {
        this->data = data;
        _isFull = true;
    }

    T data;

    friend class Output<T>;
};

class Node;

class Circuit {
public:
    static Circuit& instance() {
        static Circuit c;
        return c;
    }

    void registerNode(Node* node);
    void unregisterNode(Node* node);
    void tick();

private:
    Circuit() {}
    ~Circuit() {};
    Circuit(const Circuit&) = delete;
    Circuit& operator=(const Circuit&) = delete;

    std::unordered_set<Node*> nodes;
};

class Node {
public:
    Node() {
        Circuit::instance().registerNode(this);
    }

    ~Node() {
        Circuit::instance().unregisterNode(this);
    }

    bool isFireable() {
        //std::cout << "isFireable : " << inputs.size() << " inputs" << std::endl;
        for(auto i: inputs)
            if(!i->isFull()) {
                //std::cout << "isFireable() == false" << std::endl;
                return false;
            }
        //std::cout << "isFireable() == true" << std::endl;
        return true;
    }
    
protected:
    virtual void process() = 0;
    std::unordered_set<InputBase*> inputs;

private:

    friend class Circuit;
};





template <typename T>
class Source : public Node {
public:
    Source(T (*f)()) : f(f) {}
    Output<T> out;
protected:
    T (*f)();
private:
    void process() override {
        //std::cout << "source" << std::endl;
        out.put(f());
    }
};


template <typename T>
class Sink : public Node {
public:
    #warning find a better way to register inputs
    Sink(void (*f)(T)) : f(f) { inputs.insert(&in); }
    Input<T> in;

private:
    void (*f)(T);
    void process() override {
        f(in.get());
    }
};

template <typename A, typename B>
class Function1i : public Node {
public:
    Function1i(B (*f)(A a)) : f(f) { inputs.insert(&in); }

    Input<A> in;
    Output<B> out;

protected:

    B (*f)(A a);

private:
    void process() override {
        out.put(f(in.get()));
    }
};

template <typename A, typename B, typename C>
class Function2i : public Node {
public:
    Function2i(C (*f)(A a, B b)) : f(f) { inputs.insert(&in0); inputs.insert(&in1); }

    Input<A> in0;
    Input<B> in1;
    Output<C> out;

protected:

    C (*f)(A a, B b);

private:
    void process() override {
        out.put(f(in0.get(), in1.get()));
    }
};

#endif