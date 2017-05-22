#ifndef TRAINER_HPP
#define TRAINER_HPP

#include "Network.hpp"

namespace ezn
{

template<typename Function, typename... T>
class Trainer
{
    public:
        Trainer() : network{} {};

        void setNetwork(const Network<Function, T...> & network)
        {
            (*this).network = network;
        }

        Network<Function, T...> getNetwork() const
        {
            return network;
        }

        double trainNetwork(const std::vector<double> & input, double target)
        {
            network.predict(input);
            return network.propagate(target);
        }

    private:
        Network<Function, T...> network;
};

}

#endif // TRAINER_HPP
