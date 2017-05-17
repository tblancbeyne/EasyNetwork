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
        Neuron(const Function & f) : function(f) {}
        void setValue(double value)
        {
            assert(input.size() == 0);
            this->value = value;
        }

        void updateValue()
        {
            value = std::accumulate(input.begin(), input.end(), 0, [](double current, const Synapsis & s){
                return current + s.getOutput();
            });
        }

    private:
        std::vector<std::shared_ptr< Synapsis>> input;
        std::vector<std::shared_ptr< Synapsis>> output;
        double value;
        Function function;
};

template<typename Function>
Neuron<Function> make_neuron(const Function & f)
{
    return Neuron<Function>(f);
}

}

#endif // NEURON_HPP

