#ifndef NEURON_HPP
#define NEURON_HPP

#include <random>
#include <memory>
#include <cassert>
#include <iostream>

namespace ezn
{

class Random
{
    public:
        Random() : rd(), gen(rd()), dis(0,1) {};
        double operator()()
        {
            return dis(gen);
        }

    private:
        std::random_device rd;
        std::mt19937 gen;
        std::uniform_real_distribution<> dis;
};

class Synapsis
{
    public:
        Synapsis() : weight(random()), input(0){};

        double getOutput() const
        {
            return input*weight;
        }

        double getWeight() const
        {
            return weight;
        }

        void setInput(double input)
        {
            this->input = input;
        }

        void updateWeight(double deltaOutput)
        {
            weight -= deltaOutput*input;
        }

    private:
        static Random random;
        double weight;
        double input;
};

template<typename Function>
class Neuron
{
    public:
        Neuron(const Function & f) : input{}, output{}, value(0), result(0.5), function(f) {};

        void setValue(double value)
        {
            this->value = value;
            result = function(value);
        }

        double getValue() const
        {
            return value;
        }

        double getResult() const
        {
            return result;
        }

        void addInput(const std::shared_ptr<Synapsis> & input)
        {
            this->input.push_back(input);
        }

        void addOutput(const std::shared_ptr<Synapsis> & output)
        {
            this->output.push_back(output);
        }

        void update()
        {
            updateValue();
            updateOutputs();
        }

        void updateSynapsises(double deltaOutput)
        {
            double sum;
            if (output.size() > 0)
            {
                for (std::size_t i = 0; i < output.size(); ++i)
                {
                    sum += output[i]->getWeight()*function.prime(value);
                }
            }
            else
            {
              sum = function.prime(value);
            }
            for (std::size_t i = 0; i < input.size(); ++i)
            {
                input[i]->updateWeight(deltaOutput*sum);
            }
       }

    private:
        std::vector<std::shared_ptr<Synapsis>> input;
        std::vector<std::shared_ptr<Synapsis>> output;
        double value;
        double result;
        Function function;

        void updateValue()
        {
            value = std::accumulate(input.begin(), input.end(), 0, [](double current, const std::shared_ptr<Synapsis> & s){
                return current + s->getOutput();
            });
            result = function(value);
        }

        void updateOutputs()
        {
            for (std::size_t i = 0; i < output.size(); ++i)
                output[i]->setInput(result);
        }

};

template<typename Function, typename... T>
Neuron<Function> make_neuron(T... param)
{
    return Neuron<Function>(Function(param...));
}

}

#endif // NEURON_HPP

