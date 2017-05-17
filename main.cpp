#include <iostream>
#include <cmath>

#include "Network.hpp"

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
    ezn::Network<Sigmoid> network(std::vector<double>(),1,2);
    return 0;
}
