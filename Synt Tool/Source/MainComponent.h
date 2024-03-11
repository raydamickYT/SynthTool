#pragma once

#include <JuceHeader.h>

//==============================================================================
/*
    This component lives inside our window, and this is where you should put all
    your controls and content.
*/
class MainComponent  : public juce::AudioAppComponent,
	public juce::Slider::Listener // maak je component een slider Listener
{
public:
    //==============================================================================
    MainComponent();
    ~MainComponent() override;

    //==============================================================================
    void prepareToPlay(int samplesPerBlockExpected, double sampleRate) override;
    void getNextAudioBlock(const juce::AudioSourceChannelInfo& bufferToFill) override;
    void releaseResources() override;

    //==============================================================================
    void paint (juce::Graphics& g) override;
    void resized() override;

    //==============================================================================
    void sliderValueChanged(juce::Slider* slider) override; // Implementeer deze functie om slider veranderingen te behandelen


private:
    //==============================================================================
    double currentAngle = 0.0, angleDelta = 0.0;


    double currentSampleRate = 0.0; //de frequency wordt gebruikt voor de hoogte van de klank
    int frequency = 440;
    juce::Slider frequencySlider;
    juce::Label frequencyLabel = "frequency";

    void updateAngleDelta(); // Functie om angleDelta te updaten gebaseerd op de huidige frequency en sampleRate

    juce::ComboBox waveSelector;
    enum WaveForms {
        sineWave = 1,
        sawWave,
        squareWave,
        triangleWave
    };
    void waveChanged();

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainComponent)
};
