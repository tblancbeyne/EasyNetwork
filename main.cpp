#include <iostream>
#include <cmath>

#include "Trainer.hpp"

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

    std::vector<double> inputs;
    inputs.push_back(dis(gen));
    inputs.push_back(dis(gen));

    network.setInputLayer(inputs);
    network.addHiddenLayer(3);
    network.createSynapsises();

    ezn::Trainer<Sigmoid> trainer;

    trainer.setNetwork(network);

    bool trainingNeeded = true;

    while (trainingNeeded)
    {
        inputs[0] = dis(gen);
        inputs[1] = dis(gen);

        if (fabs(trainer.trainNetwork(inputs,!inputs[0] != !inputs[1])) > 0.001)
        {
            network = trainer.getNetwork();
            trainingNeeded = false;
            for (std::size_t i = 0; i <= 1; ++i)
            {
                for (std::size_t j = 0; j <= 1; ++j)
                {
                    inputs[0] = i;
                    inputs[1] = j;
                    double result = network.predict(inputs);
                    if ((result - (!inputs[0] != !inputs[1])) > 0.001)
                    {
                        trainingNeeded = true;
                    }
                }
            }

        }
    }

    std::cout << "The network has been trained" << std::endl;

    return 0;
}
