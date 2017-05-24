#ifndef LAYER_HPP
#define LAYER_HPP

#include <vector>

#include "Neuron.hpp"

//#define UPDATE
//#define PROPAGATE

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
        Layer(std::size_t n, T... param) : Layer()  {
            for (std::size_t i = 0; i < n; ++i)
                push_back(make_neuron<Function>(param...));
        };

        void updateNeurons()
        {
            for (std::size_t i = 0; i < size(); ++i)
            {
#ifdef UPDATE
             std::cout << "Updating neuron " << i << " from the layer."<< std::endl;
#endif
               (*this)[i].update();
            }
        }

        void propagate(double deltaOutput)
        {
            for (std::size_t i = 0; i < size(); ++i)
            {
#ifdef PROPAGATE
                std::cout << "Propagating " << deltaOutput << " to synapsises from neuron " << i << "." << std::endl;
#endif
                (*this)[i].updateSynapsises(deltaOutput);
            }
        }

        void print() const
        {
            for (std::size_t i = 0; i < size(); ++i)
                {
                    (*this)[i].printWeight(i);
                    (*this)[i].printBias();
                }
        }

    private:
};

}

#endif // LAYER_HPP
