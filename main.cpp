#include <iostream>
#include <cmath>
#include <string>

#include "Network.hpp"

//#define DEBUG

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

class TanH
{
    public:
        TanH(){};
        double operator()(double x)
        {
            return std::tanh(x);
        }

        double prime(double x)
        {
            return 1 - std::tanh(x)*std::tanh(x);
        }

    private:

};



int main(int argc, char *argv[])
{
    std::size_t count = 0;

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0,1);

    ezn::Network<Sigmoid> network;

    std::vector<double> inputs;
    inputs.push_back(dis(gen));
    inputs.push_back(dis(gen));


    network.setInputLayer(inputs);
    network.addHiddenLayer(3);
    network.createSynapses();

    bool trainingNeeded = true;

    while (trainingNeeded)
    {
        count++;

        inputs[0] = dis(gen);
        inputs[1] = dis(gen);

        double error = fabs(network.train(inputs,!inputs[0] != !inputs[1]));

        //std::cout << "Inputs : " << inputs[0] << "," << inputs[1] << ", error: " << error << std::endl;

        if (error < 0.1)
        {
            //std::cout << "Testing the training" << std::endl;
            trainingNeeded = false;
            for (std::size_t i = 0; i <= 1; ++i)
            {
                for (std::size_t j = 0; j <= 1; ++j)
                {
                    inputs[0] = i;
                    inputs[1] = j;
                    double result = network.predict(inputs);
                    if (fabs(result - (inputs[0] != inputs[1])) > 0.1)
                    {
                        trainingNeeded = true;
                    }
                }
            }
        }

#ifdef DEBUG
        std::string str;
        std::cout << "Press enter to continue.";
        std::getline(std::cin,str);
#endif

        if (count%100000 == 0)
        {
            std::cout << "Iteration " << count << ":" << std::endl;
            for (std::size_t i = 0; i <= 1; ++i)
            {
                for (std::size_t j = 0; j <= 1; ++j)
                {
                    inputs[0] = i;
                    inputs[1] = j;
                    double result = network.predict(inputs);
                    std::cout << "Inputs: " << inputs[0] << "," << inputs[1] << ", result: " << result << std::endl;
                }
            }

        }
    }

    std::cout << "Results:" << std::endl;
    for (std::size_t i = 0; i <= 1; ++i)
    {
        for (std::size_t j = 0; j <= 1; ++j)
        {
            inputs[0] = i;
            inputs[1] = j;
            double result = network.predict(inputs);
            std::cout << "Inputs : " << inputs[0] << "," << inputs[1] << ", result: " << result << std::endl;
        }
    }

    std::cout << "The network has been trained with " << count << " iterations." << std::endl;

    std::cout << "Parameters of the network: " << std::endl;
    network.print();

/*    std::cout << "-----------------------" << std::endl;

    while (true)
    {
        std::string str;
        std::cout << "Input[0]=";
        std::getline(std::cin,str);
        inputs[0] = std::stoi(str);
        std::cout << "Input[1]=";
        std::getline(std::cin,str);
        inputs[1] = std::stoi(str);

        std::cout << "The network predicted: " << network.predict(inputs) << std::endl;
    }*/

    return 0;
}
