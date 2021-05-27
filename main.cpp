#include <iostream>
#include "froud.h"

int main(int argc, char *argv[])
{
    auto source = Source<int>([]() {
        static int i = 0;
        return (i++) % 10;
    });

    auto sum = Node_2i_1o<int, int, int>([](int a, int b) {
         return a + b; 
    });

    auto square = Node_1i_1o<int, int>([](int a) {
        return a*a;
    });

    auto sink = Sink<int>([](int i) {
        std::cout << i << std::endl;
    });

    sum.in0 = source.out;
    square.in = source.out;
    sum.in1 = square.out;
    sink.in = sum.out;
    
    for(int i = 0; i < 20; i++)
        Circuit::instance().tick();

    return 0;
}
