#ifndef LAYER_HPP
#define LAYER_HPP

#include <vector>

#include "Neuron.hpp"

namespace ezn
{

template<typename Function, typename... T>
class Layer : private std::vector<Neuron<Function>>
{
    public:
        using std::vector<Neuron<Function>>::size;
        using std::vector<Neuron<Function>>::operator[];
        using std::vector<Neuron<Function>>::push_back;

        Layer() : std::vector<Neuron<Function>>{} {};
        Layer(std::size_t n, T... param) : std::vector<Neuron<Function>>(n,make_neuron<Function>(param...)) {};

        void updateNeurons()
        {
            for (std::size_t i = 0; i < size(); ++i)
                (*this)[i].update();
        }

        void propagate(double deltaOutput)
        {
            for (std::size_t i = 0; i < size(); ++i)
                (*this)[i].updateSynapsises(deltaOutput);
        }

    private:
};

}

#endif // LAYER_HPP
