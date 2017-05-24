#ifndef NEURON_HPP
#define NEURON_HPP

#include <random>
#include <memory>
#include <cassert>
#include <iostream>

//#define UPDATE
//#define PROPAGATE

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
        Synapsis() : weight(random() - 0.5), input(0){};

        double getOutput() const
        {
            return input*weight;
        }

        double getWeight() const
        {
            return weight;
        }

        double getValue() const
        {
            return input;
        }

        void setInput(double input)
        {
#ifdef UPDATE
            std::cout << "Updating the value in the synapsis: " << input << "." << std::endl;
#endif
            this->input = input;
#ifdef UPDATE
            std::cout << "Result is: " << input << "*" << weight << " = " << getOutput() << "." << std::endl;
#endif
        }

        void updateWeight(double deltaOutput)
        {
#ifdef PROPAGATE
                    std::cout << "Previous weight: " << weight << "." << std::endl;
                    std::cout << "Delta weight: " << deltaOutput << "*" << input << " = " << deltaOutput*input << "." << std::endl;
#endif

            weight -= deltaOutput*input*(1 + (random() + random()));
#ifdef PROPAGATE
                    std::cout << "New weight: " << weight << "." << std::endl;
#endif
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
        Neuron(const Function & f) : input{}, output{}, value(), bias(random()), result(), function(f) {};

        void setValue(double value)
        {
            this->value = value;
            if (input.size() > 0)
            {
                result = function(value + bias);
            }
            else
            {
                result = value;
            }
        }

        double getValue() const
        {
            return value;
        }

        double getBias() const
        {
            return bias;
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
#ifdef UPDATE
            std::cout << "Updating the neuron." << std::endl;
#endif
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
                    sum += output[i]->getWeight()*function.prime(output[i]->getValue());
                }
            }
            else
            {
                sum = function.prime(value);
            }
#ifdef PROPAGATE
            std::cout << "Value of sum: " << sum << "." << std::endl;
#endif
            for (std::size_t i = 0; i < input.size(); ++i)
                    {
                input[i]->updateWeight(deltaOutput*sum);
            }
            bias -= 0.01*deltaOutput;
            //std::cout << deltaOutput << std::endl;
        }

        void printWeight(std::size_t n) const
        {
            for (std::size_t i = 0; i < output.size(); ++i)
            {
                std::cout << "Synapsis between neuron " << n << " and neuron " << i << ": " << output[i]->getWeight() << "." << std::endl;
            }
        }

        void printBias() const
        {
            std::cout << "Bias for this neuron: " << getBias() << "." << std::endl;
        }

    private:
        static Random random;
        std::vector<std::shared_ptr<Synapsis>> input;
        std::vector<std::shared_ptr<Synapsis>> output;
        double value;
        double bias;
        double result;
        Function function;

        void updateValue()
        {
#ifdef UPDATE
            std::cout << "Updating the value of the neuron." << std::endl;
            std::cout << "Value was: " << value << ", result was: " << result << "." << std::endl;
#endif
            if (input.size() > 0)
            {
                value = std::accumulate(input.begin(), input.end(), 0.0,
                                        [](double current, const std::shared_ptr<Synapsis> & s){
                                            return current + s->getOutput();
                                        });
                result = function(value);
            }
            else
            {
                result = value;
            }
#ifdef UPDATE
            std::cout << "Value is: " << value << ", result is: " << result << "." << std::endl;
#endif
        }

        void updateOutputs()
        {
#ifdef UPDATE
            std::cout << "Updating the next synapsises" << std::endl;
#endif
            for (std::size_t i = 0; i < output.size(); ++i)
            {
#ifdef UPDATE
                std::cout << "Updating synapsis " << i << " with result " << result << "." << std::endl;
#endif
                output[i]->setInput(result);
            }
        }

};

template<typename Function>
Random Neuron<Function>::random{};

template<typename Function, typename... T>
Neuron<Function> make_neuron(T... param)
{
    return Neuron<Function>(Function(param...));
}

}

#endif // NEURON_HPP

