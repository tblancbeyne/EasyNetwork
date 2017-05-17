#ifndef NETWORK_HPP
#define NETWORK_HPP

#include <vector>

#include "Layer.hpp"

namespace ezn
{

template<typename Function, typename... T>
class Network
{
    public:
        Network() : hiddenLayers{}, inputLayer{}, output{1} {};
        Network(const std::vector<double> & inputs, std::size_t n, std::size_t m, T... param) : hiddenLayers(n,Layer<Function>{n,param...}), inputLayer{}, output{1}
        {
            // Create input layer
            setInputLayer(inputs);

            // Create synapsises
            if (hiddenLayers.size() > 0)
            {
                // First layer
                for (std::size_t i = 0; i < inputLayer.size(); ++i)
                {
                    for(std::size_t j = 0; j < hiddenLayers[0].size(); ++j)
                    {
                        auto synapsis = std::make_shared<Synapsis>();
                        inputLayer[i].addOutput(synapsis);
                        hiddenLayers[0][j].addInput(synapsis);
                    }
                }

                // Next layers
                for (std::size_t i = 0; i < hiddenLayers.size(); ++i)
                {
                    for (std::size_t j = 0; j < hiddenLayers[i].size(); ++j)
                    {
                        for(std::size_t k = 0; k < hiddenLayers[i+1].size(); ++k)
                        {
                            auto synapsis = std::make_shared<Synapsis>();
                            inputLayer[j].addOutput(synapsis);
                            hiddenLayers[i+1][k].addInput(synapsis);
                        }
                    }
                }

                // Last layer
                for (std::size_t i = 0; i < hiddenLayers[hiddenLayers.size()-1].size(); ++i)
                {
                    auto synapsis = std::make_shared<Synapsis>();
                    hiddenLayers[hiddenLayers.size()-1][i].addOutput(synapsis);
                    output.addInput(synapsis);
                }
            }

        };

        void addHiddenLayer(std::size_t n, T... param)
        {
            hiddenLayers.push_back(Layer<Function>{n,param...});
        }

        void setInputLayer(const std::vector<double> & inputs)
        {
            for (std::size_t i = 0; i < inputs.size(); ++i)
            {
                Neuron<Function> neuron = make_neuron<Function>();
                neuron.setValue(inputs[i]);
                inputLayer.push_back(neuron);
            }
        }

        void createSynapsises()
        {

        }

    private:
        std::vector<ezn::Layer<Function>> hiddenLayers;
        Layer<Function> inputLayer;
        Neuron<Function> output;

};

}

#endif // NETWORK_HPP
