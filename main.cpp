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
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0,1);

    ezn::Network<Sigmoid> network;

    while (true)
    {
        std::vector<double> inputs;
        inputs.push_back(dis(gen));
        inputs.push_back(dis(gen));

        network.setInputLayer(inputs);
        network.addHiddenLayer(3);
        network.createSynapsises();
    }
    return 0;
}
