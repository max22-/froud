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


template <typename T>
class Input {
public:
    Input() {}
    T get() { _isFull = false; return data; }
    Input<T> operator=(Output<T>& o) {
        #warning Implement disconnection when input is destructed
        o.connect(this);
        return *this;
    }
    virtual bool isFull() final {
        return _isFull;
    }

private:
    Input<T> operator=(const Input<T>& i) = delete;

    void acceptData(T data) {
        this->data = data;
        _isFull = true;
    }

    T data;
    bool _isFull;
    friend class Output<T>;
};

class Node {

protected:
    virtual void process() = 0;
    virtual bool isFireable()  = 0;

private:

    friend class Circuit;
};

class Circuit {
public:
    Circuit() { }
    void registerNodes(std::unordered_set<Node*> nodes) {
        this->nodes.insert(nodes.begin(), nodes.end());
    }
    void tick() {
    for(auto n: nodes)
        if(n->isFireable())
            n->process();
    }

private:
    Circuit(const Circuit&) = delete;
    Circuit& operator=(const Circuit&) = delete;

    std::unordered_set<Node*> nodes;
};


class Bang final {};

#endif