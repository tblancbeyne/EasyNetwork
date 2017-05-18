#ifndef NETWORK_HPP
#define NETWORK_HPP

#include <vector>
#include <iostream>

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
            createSynapsises();
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
                for (std::size_t i = 0; i < hiddenLayers.size()-1; ++i)
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
            else
            {
                // Synapsises between input and output
                for (std::size_t i = 0; i < inputLayer.size(); ++i)
                {
                    auto synapsis = std::make_shared<Synapsis>();
                }
            }
        }

        void update()
        {
            for (std::size_t i = 0; i < hiddenLayers.size(); ++i)
            {
                hiddenLayers[i].updateNeurons();
            }
            output.update();
        }

        void updateInputLayer(const std::vector<double> & input)
        {
            assert(input.size() == inputLayer.size());
            for (std::size_t i = 0; i < input.size(); ++i)
                inputLayer[i].setValue(input[i]);
        }

        void propagate()
        {
            error = target - output.getOutput();
            output.updateSynapsises(error);
        }

    private:
        std::vector<ezn::Layer<Function>> hiddenLayers;
        Layer<Function> inputLayer;
        Neuron<Function> output;
        double error;
        double target;
};

}

#endif // NETWORK_HPP
