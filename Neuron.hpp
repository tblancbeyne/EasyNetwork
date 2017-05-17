#ifndef NEURON_HPP
#define NEURON_HPP

#include <random>
#include <memory>
#include <cassert>

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
        Synapsis() : weight(random()), value(0){};

        double getOutput() const
        {
            return value;
        }

        void setInput(double input)
        {
            value = weight*input;
        }

        void updateWeight(double deltaOutput)
        {
            weight -= deltaOutput*weight;
        }

    private:
        static Random random;
        double weight;
        double value;
};

template<typename Function>
class Neuron
{
    public:
        Neuron(const Function & f) : input{}, output{}, value(0), function(f) {};

        void setValue(double value)
        {
            this->value = value;
        }

        double getValue() const
        {
            return value;
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

    private:
        std::vector<std::shared_ptr<Synapsis>> input;
        std::vector<std::shared_ptr<Synapsis>> output;
        double value;
        Function function;

        void updateValue()
        {
            value = std::accumulate(input.begin(), input.end(), 0, [](double current, const Synapsis & s){
                return current + s.getOutput();
            });
        }

        void updateOutputs()
        {
            for (std::size_t i = 0; i < output.size(); ++i)
                output[i]->setInput(value);
        }

};

template<typename Function, typename... T>
Neuron<Function> make_neuron(T... param)
{
    return Neuron<Function>(Function(param...));
}

}

#endif // NEURON_HPP

