#include "synth.h"

Synth::Synth()
{
    key = utils::itof(0);
    pitch = 0.0;
    unisonAmount = 0.0;
    squareAmount = 0.0;
}

Synth::~Synth()
{

}

void Synth::setKey(const StkFloat &key)
{
    this->key = key;
}

void Synth::setPitch(const StkFloat &pitch)
{
    this->pitch = pitch;
}

void Synth::setUnisonAmount(const StkFloat &unisonAmount)
{
    this->unisonAmount = unisonAmount;
    unisonFreqs.clear();

    foreach (double d, utils::unisonDetune(key, unisonAmount, waves.size()))
    {
        unisonFreqs << d;
    }
}

void Synth::setSquareAmount(const StkFloat &squareAmount)
{
    this->squareAmount = squareAmount;
}

void Synth::setVolume(const StkFloat &value)
{
    volume = value;
}

void Synth::setUnisonCount(const unsigned int &count)
{
    clear();
    unisonFreqs.clear();

    for (unsigned int i=0; i < count; i++)
    {
        waves.push_back(new SineWave());
    }

    if (count > 0)
    {
        setUnisonAmount(unisonAmount);
    }
}

void Synth::clear()
{
    qDeleteAll(waves);
    waves.clear();
}

StkFloat Synth::tick()
{
    StkFloat val = 0;
    for (int i=0; i < waves.size(); i++)
    {
        waves[i]->setFrequency(key + unisonFreqs[i] + pitch);

        StkFloat tick = waves[i]->tick();
        if (squareAmount > 0.0)
        {
            if (squareAmount < 1.0)
            {
                tick = utils::distort(tick, squareAmount);
            }

            else
            {
                tick = utils::square(tick);
            }
        }

        val += tick;
    }

    return utils::scaleVolume(val, volume);
}
