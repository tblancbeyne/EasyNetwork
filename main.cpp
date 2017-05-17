#include <iostream>
#include <cmath>

#include "Neuron.hpp"

class Sigmoid
{
    public:
        Sigmoid(double lambda=1) : lambda(lambda){};
        double operator()(double x)
        {
            return 1/(1+std::exp(-lambda*x));
        }

        double prime(double x)
        {
            double y = (*this)(x);
            return lambda*y*(1-y);
        }

    private:
        double lambda;

};

int main(int argc, char *argv[])
{
    auto neuron = ezn::make_neuron(Sigmoid{});
    return 0;
}
