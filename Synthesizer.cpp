#include <iostream>
#include "olcNoiseMaker.h"





double angularVelocity(double dHertz)
{
    return dHertz * 2.0 * 3.14159;
}

constexpr int OSC_SINE = 0;
constexpr int OSC_SQUARE = 1;
constexpr int OSC_TRIANGLE = 2;
constexpr int OSC_SAW_ANALOG = 3;
constexpr int OSC_SAW_DIGITAL = 4;
constexpr int OSC_NOISE = 5;


//oscillator

double oscillator(double dHertz, double dTime, int nType = OSC_SQUARE)
{
    if (nType == OSC_SINE)
    {
        return sin(angularVelocity(dHertz) * dTime);
    }
    else if (nType == OSC_SQUARE)
    {
        return sin(angularVelocity(dHertz) * dTime) > 0.0 ? 0.3 : -0.3;
    }
    else if (nType == OSC_TRIANGLE)
    {
        return asin(sin(angularVelocity(dHertz) * dTime) * (2.0 / 3.14159));
    }
    else if (nType == OSC_SAW_ANALOG)
    {
        double dOutput = 0.0;
        for (double n = 1.0; n < 40.0; n++) {
            dOutput += (sin(n * angularVelocity(dHertz) * dTime)) / n;
        }
        return dOutput * (2.0 / 3.14159);
    }
    else if (nType == OSC_SAW_DIGITAL)
    {
        return (2.0 / 3.14159) * (dHertz * 3.14159 * fmod(dTime, 1.0 / dHertz) - (3.14159 / 2.0));
    }
    else if (nType == OSC_NOISE)
    {
        return 2.0 * ((double)rand() / (double)RAND_MAX) - 1.0;
    }
    else
    {
        return 0.0;
    }
}


//envelopes
struct sEnvelopeADSR 
{
    double dAttackTime;
    double dDecayTime;
    double dSustainAmplitude;
    double dReleaseTime;
    double startAmp;
    double triggerOfftime;
    double triggerOntime;
    bool bnoteOn;

    sEnvelopeADSR()
    {
        dAttackTime = 0.10;
        dDecayTime = 0.01;
        dSustainAmplitude = 0.8;
        dReleaseTime = 0.20;
        startAmp = 1.0 ;
        triggerOfftime = 0.0;
        triggerOntime = 0.0;
        bnoteOn = false;
    }

    void noteOn(double dTimeOn)
    {
        triggerOntime = dTimeOn;
        bnoteOn = true;
    }

    void noteOff(double dTimeoff)
    {
        triggerOfftime = dTimeoff;
        bnoteOn = false;
    }

    double getAmplitude(double dTime)
    {
        double dAmplitude = 0.0;
        double dLifetime = dTime - triggerOntime;

        if (bnoteOn)
        {
            if (dLifetime <= dAttackTime)
            {
                dAmplitude = (dLifetime / dAttackTime) * startAmp;
            }
            if (dLifetime > dAttackTime && dLifetime <= (dAttackTime + dDecayTime))
            {
                // In decay phase - reduce to sustained amplitude
                dAmplitude = ((dLifetime - dAttackTime) / dDecayTime) * (dSustainAmplitude - startAmp) + startAmp;
            }
            if (dLifetime > (dAttackTime + dDecayTime))
            {
                // In sustain phase - dont change until note released
                dAmplitude = dSustainAmplitude;
            }
        }
        else
        {
            // Note has been released, so in release phase
            dAmplitude = ((dTime - triggerOfftime) / dReleaseTime) * (0.0 - dSustainAmplitude) + dSustainAmplitude;
        }
        // Amplitude should not be negative
        return std::abs(dAmplitude);
    }

};

std::atomic<double> dFrequencyOutput = 0.0;
sEnvelopeADSR envelope;

static double dOctaveFrequency = 500.0; //A2
static double d12thOfRoot2 = pow(2.0, 1.0 / 12.0);

//generate soundwave
double makenoise(double dTime)
{
    auto dOutput = envelope.getAmplitude(dTime) * ( 
        + 1.0 * oscillator(dFrequencyOutput * 0.5, dTime, OSC_SQUARE) 
        + 1.0 * oscillator(dFrequencyOutput, dTime, OSC_SAW_ANALOG)
        );
    return dOutput * 0.4; //master volume
}
    


int main(int argc, char *argv[])
{
    std::wcout << "synth programs" << std::endl;

    std::wcout << std::endl <<
        "|   | Q |   |   | W | | E |   |   | R | | T | | Y |   |   |" << std::endl <<
        "|   |   |   |   |   | |   |   |   |   | |   | |   |   |   |" << std::endl <<
        "|   |___|   |   |___| |___|   |   |___| |___| |___|   |   |__" << std::endl <<
        "|     |     |     |     |     |     |     |     |     |     |" << std::endl <<
        "|  1  |  2  |  3  |  4  |  5  |  6  |  7  |  8  |  9  |  0  |" << std::endl <<
        "|_____|_____|_____|_____|_____|_____|_____|_____|_____|_____|" << std::endl << endl;

    //get all sound hardware
    std::vector<wstring> devices = olcNoiseMaker<short>::Enumerate();


    //display findings
    for (auto d : devices)
    {
        std::wcout << "found output devices !" << d << std::endl;
    }
    
    //create sound machine
    olcNoiseMaker<short> sound(devices[0], 44100, 1, 8, 512);

    //link noise function with sound machine
    sound.SetUserFunction(makenoise);

    while (1)
    {
        //add a keyboard
        //c major scale
        //key = 0,2,4,5,7,9,11,12

        if (GetAsyncKeyState('1') & 0x8000)
        {
            dFrequencyOutput = dOctaveFrequency * pow(d12thOfRoot2, 0);
        }
        else if (GetAsyncKeyState('Q') & 0x8000)
        {
            dFrequencyOutput = dOctaveFrequency * pow(d12thOfRoot2, 1);
        }
        else if (GetAsyncKeyState('2') & 0x8000)
        {
            dFrequencyOutput = dOctaveFrequency * pow(d12thOfRoot2, 2);
        }
        else if (GetAsyncKeyState('W') & 0x8000)
        {
            dFrequencyOutput = dOctaveFrequency * pow(d12thOfRoot2, 3);
        }
        else if (GetAsyncKeyState('3') & 0x8000)
        {
            dFrequencyOutput = dOctaveFrequency * pow(d12thOfRoot2, 4);
        }
        else if (GetAsyncKeyState('4') & 0x8000)
        {
            dFrequencyOutput = dOctaveFrequency * pow(d12thOfRoot2, 5);
        }
        else if (GetAsyncKeyState('E') & 0x8000)
        {
            dFrequencyOutput = dOctaveFrequency * pow(d12thOfRoot2, 6);
        }
        else if (GetAsyncKeyState('5') & 0x8000)
        {
            dFrequencyOutput = dOctaveFrequency * pow(d12thOfRoot2, 7);
        }
        else if (GetAsyncKeyState('R') & 0x8000)
        {
            dFrequencyOutput = dOctaveFrequency * pow(d12thOfRoot2, 8);
        }
        else if (GetAsyncKeyState('6') & 0x8000)
        {
            dFrequencyOutput = dOctaveFrequency * pow(d12thOfRoot2, 9);
        }
        else if (GetAsyncKeyState('T') & 0x8000)
        {
            dFrequencyOutput = dOctaveFrequency * pow(d12thOfRoot2, 10);
        }
        else if (GetAsyncKeyState('7') & 0x8000)
        {
            dFrequencyOutput = dOctaveFrequency * pow(d12thOfRoot2, 11);
        }
        else if (GetAsyncKeyState('8') & 0x8000)
        {
            dFrequencyOutput = dOctaveFrequency * pow(d12thOfRoot2, 12);
        }
        else
        {
            dFrequencyOutput = 0.0f;
        }

    }
    
    
    
    return 0;
}



