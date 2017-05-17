#ifndef LAYER_HPP
#define LAYER_HPP

#include <vector>

#include "Neuron.hpp"

namespace ezn
{

template<typename Function>
class Layer
{
    public:
        Layer() : neurons{} {};
        Layer(std::size_t n, const Function & f = Function{}) : neurons(n,make_neuron(f)) {};

        void updateNeurons()
        {
            for (std::size_t i = 0; i < neurons.size(); ++i)
                neurons[i].update();
        }

    private:
        std::vector<ezn::Neuron<Function>> neurons;
};

}

#endif // LAYER_HPP
