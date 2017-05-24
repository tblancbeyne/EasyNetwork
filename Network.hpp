#ifndef NETWORK_HPP
#define NETWORK_HPP

#include <vector>
#include <iostream>

#include "Layer.hpp"

//#define SYNAPSIS
//#define NEURON
//#define PREDICT
//#define PROPAGATE

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

            // Create synapses
            createSynapses();
        };

        void addHiddenLayer(std::size_t n, T... param)
        {
            hiddenLayers.push_back(Layer<Function>{n,param...});
        }

        void setInputLayer(const std::vector<double> & inputs)
        {
            for (std::size_t i = 0; i < inputs.size(); ++i)
            {
#ifdef NEURON
                std::cout << "Creating input neuron " << i << " with value " << inputs[i] << "." << std::endl;
#endif
                Neuron<Function> neuron = make_neuron<Function>();
                neuron.setValue(inputs[i]);
                inputLayer.push_back(neuron);
            }
        }

        void createSynapses()
        {
            if (hiddenLayers.size() > 0)
            {
                // First layer
                for (std::size_t i = 0; i < inputLayer.size(); ++i)
                {
                    for(std::size_t j = 0; j < hiddenLayers[0].size(); ++j)
                    {
#ifdef SYNAPSIS
                        std::cout << "Linking : inputlayer[" << i <<"] with hiddenLayers[0][" << j << "]." << std::endl;
#endif
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
#ifdef SYNAPSIS
                        std::cout << "Linking : hiddenLayers[" << i <<"][" << j << "] with hiddenLayers[" << i+1 << "][" << k << "]." << std::endl;
#endif
                            auto synapsis = std::make_shared<Synapsis>();
                            inputLayer[j].addOutput(synapsis);
                            hiddenLayers[i+1][k].addInput(synapsis);
                        }
                    }
                }

                // Last layer
                for (std::size_t i = 0; i < hiddenLayers[hiddenLayers.size()-1].size(); ++i)
                {
#ifdef SYNAPSIS
                        std::cout << "Linking : hiddenLayers[" << hiddenLayers.size()-1 <<"]["<< i << "] with output." << std::endl;
#endif
                    auto synapsis = std::make_shared<Synapsis>();
                    hiddenLayers[hiddenLayers.size()-1][i].addOutput(synapsis);
                    output.addInput(synapsis);
                }
            }
            else
            {
                // Synapses between input and output
                for (std::size_t i = 0; i < inputLayer.size(); ++i)
                {
#ifdef SYNAPSIS
                    std::cout << "Linking : inputLayer[" << i << "] with output." << std::endl;
#endif
                    auto synapsis = std::make_shared<Synapsis>();
                    inputLayer[i].addOutput(synapsis);
                    output.addInput(synapsis);
                }
            }
        }

        double predict(const std::vector<double> & input)
        {
#ifdef PREDICT
            std::cout << "Starting the prediction." << std::endl;
#endif
            assert(input.size() == inputLayer.size());
            for (std::size_t i = 0; i < input.size(); ++i)
            {
#ifdef PREDICT
            std::cout << "Setting inputLayer[" << i << "] to " << input[i] << "." << std::endl;
#endif
                inputLayer[i].setValue(input[i]);
            }
#ifdef PREDICT
            std::cout << "Updating inputLayer." << std::endl;
#endif
            inputLayer.updateNeurons();

            for (std::size_t i = 0; i < hiddenLayers.size(); ++i)
            {
#ifdef PREDICT
            std::cout << "Updating neurons in hiddenLayers[" << i << "]." << std::endl;
#endif

                hiddenLayers[i].updateNeurons();
            }

#ifdef PREDICT
            std::cout << "Updating output neuron." << std::endl;
#endif

            output.update();
#ifdef PREDICT
            std::cout << "Predicted value is: " << output.getResult() << "." << std::endl;
#endif
            return output.getResult();
        }

        double propagate(double target)
        {
            double error = output.getResult() - target;
            // Propagation of error to input target is useless, as it has no input synapses
            for (std::size_t i = 0; i < hiddenLayers.size(); ++i)
            {
#ifdef PROPAGATE
                std::cout << "Propagating error to hiddenLayers[" << i << "]." << std::endl;
#endif
                hiddenLayers[i].propagate(error);
            }
#ifdef PROPAGATE
                std::cout << "Propagating error to output." << std::endl;
#endif
            output.updateSynapses(error);

            return error;
        }

        double train(const std::vector<double> & input, double target)
        {
            predict(input);
            return propagate(target);
        }

        void print() const
        {
            if (hiddenLayers.size() > 0)
            {
                std::cout << "Synapses between inputLayer and hiddenLayers[0]:" << std::endl;
                inputLayer.print();

                for (std::size_t i = 0; i < hiddenLayers.size()-1; ++i)
                {
                    std::cout << "Synapses between hiddenLayers[" << i << "] and hiddenLayers[" << i+1 << "]:" << std::endl;
                    hiddenLayers[i].print();
                }

                std::cout << "Synapses between hiddenLayers[" << hiddenLayers.size()-1 << "] and ouput neuron:" << std::endl;
                hiddenLayers[hiddenLayers.size()-1].print();
            }
            else
            {
                std::cout << "Synapses between inputLayer and output:" << std::endl;
                inputLayer.print();
            }

            std::cout << "Output neuron bias: " << output.getBias() << std::endl;
        }

    private:
        std::vector<ezn::Layer<Function>> hiddenLayers;
        Layer<Function> inputLayer;
        Neuron<Function> output;
};

}

#endif // NETWORK_HPP
