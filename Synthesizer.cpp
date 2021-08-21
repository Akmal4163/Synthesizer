// Synthesizer.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include "olcNoiseMaker.h"



std::atomic<double> dFrequencyOutput = 0.0;


double makeChordSineWave(double dTime)
{
    double dOutput = 1.0 * sin(dFrequencyOutput * 2.0 * 3.14159 * dTime) + sin((dFrequencyOutput + 20.0) * 2.0 * 3.14159 * dTime);

    return dOutput;

}

double makeNoiseSineWave(double dTime)
{
    double dOutput = 1.0 * sin(dFrequencyOutput * 2.0 * 3.14159 * dTime);

        return dOutput;
       
}

double makeChordSquareWave(double dTime)
{
    double dOutput = 1.0 * sin(dFrequencyOutput * 2.0 * 3.14159 * dTime) + sin((dFrequencyOutput + 20.0) * 2.0 * 3.14159 * dTime);

    if (dOutput > 0.0)
    {
        return 0.3;
    }
    else
    {
        return -0.3;
    }
}

double makeNoiseSquareWave(double dTime)
{
    double dOutput = 1.0 * sin(dFrequencyOutput * 2.0 * 3.14159 * dTime);

    if (dOutput > 0.0)
    {
        return 0.3;
    }
    else
    {
        return -0.3;
    }
}
    


int main()
{
    std::wcout << "synth programs" << std::endl;

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

    int pValue;
    std::cout << "choose a wave, 1. sine wave 2. square wave\n";
    std::cin >> pValue;
    if (pValue == 1)
    {
        std::cout << "sine wave\n";
        int pValueValue;
        std::cout << "1. chords 2. single tone\n";
        std::cin >> pValueValue;
        if (pValueValue == 1)
        {
            sound.SetUserFunction(makeChordSineWave);
        }
        if (pValueValue == 2)
        {
            sound.SetUserFunction(makeNoiseSineWave);
        }
        
    }
    else if (pValue == 2)
    {
        std::cout << "square wave\n";
        int pValueValue;
        std::cout << "1. chords 2. single tone\n";
        std::cin >> pValueValue;
        if (pValueValue == 1)
        {
            sound.SetUserFunction(makeChordSquareWave);
        }
        if (pValueValue == 2)
        {
            sound.SetUserFunction(makeNoiseSquareWave);
        }
        
    }
    else
    {
        std::cout << "error not found\n";
    }
           
    

    double dOctaveFrequency = 300.0; //A2
    double d12thOfRoot2 = pow(2.0, 1.0 / 12.0);

    while (1)
    {
        //add a keyboard
        //c major scale
        //key = 0,2,4,5,7,9,11,12

        if (GetAsyncKeyState('1') & 0x8000)
        {
            dFrequencyOutput = dOctaveFrequency * pow(d12thOfRoot2, 0);
        }
        else if (GetAsyncKeyState('2') & 0x8000)
        {
            dFrequencyOutput = dOctaveFrequency * pow(d12thOfRoot2, 2);
        }
        else if (GetAsyncKeyState('3') & 0x8000)
        {
            dFrequencyOutput = dOctaveFrequency * pow(d12thOfRoot2, 4);
        }
        else if (GetAsyncKeyState('4') & 0x8000)
        {
            dFrequencyOutput = dOctaveFrequency * pow(d12thOfRoot2, 5);
        }
        else if (GetAsyncKeyState('5') & 0x8000)
        {
            dFrequencyOutput = dOctaveFrequency * pow(d12thOfRoot2, 7);
        }
        else if (GetAsyncKeyState('6') & 0x8000)
        {
            dFrequencyOutput = dOctaveFrequency * pow(d12thOfRoot2, 9);
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



