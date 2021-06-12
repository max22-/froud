#include <iostream>
#include "froud/froud.h"
#include "froud/components.h"

int main(int argc, char *argv[])
{
    Circuit circuit;

    auto clock = Clock();

    auto generator = Generator<int>([]() {
        static int i = 0;
        return (i++) % 10;
    });

    auto sum = Function2i<int, int, int>([](int a, int b) {
         return a + b; 
    });

    auto square = Function1i<int, int>([](int a) {
        return a*a;
    });

    auto sink = Sink<int>([](int i) {
        std::cout << i << std::endl;
    });

    generator.in = clock.out;
    sum.in0 = generator.out;
    square.in = generator.out;
    sum.in1 = square.out;
    sink.in = sum.out;

    circuit.registerNodes({&clock, &generator, &sum, &square, &sink});
    
    for(int i = 0; i < 20; i++)
        circuit.tick();

    return 0;
}
