#include "MainComponent.h"
#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif


//==============================================================================
MainComponent::MainComponent()
{
    // Frequency Slider
    frequencySlider.setRange(20.0, 20000.0); // Bruikbare frequentiebereik
    frequencySlider.setValue(440.0); // Standaard A4
    frequencySlider.addListener(this); // Voeg deze klasse toe als een listener
    addAndMakeVisible(frequencySlider);

    frequencyLabel.setText("Frequency", juce::dontSendNotification);
    frequencyLabel.attachToComponent(&frequencySlider, false);
    addAndMakeVisible(frequencyLabel);

    // Make sure you set the size of the component after
    // you add any child components.
    setSize (800, 600);

    // Some platforms require permissions to open input channels so request that here
    if (juce::RuntimePermissions::isRequired (juce::RuntimePermissions::recordAudio)
        && ! juce::RuntimePermissions::isGranted (juce::RuntimePermissions::recordAudio))
    {
        juce::RuntimePermissions::request (juce::RuntimePermissions::recordAudio,
                                           [&] (bool granted) { setAudioChannels (granted ? 2 : 0, 2); });
    }
    else
    {
        // Specify the number of input and output channels that we want to open
        setAudioChannels (2, 2);
    }

    //init van de wave selector
    waveSelector.addItem("Sine Wave", sineWave);
    waveSelector.addItem("Saw Wave", sawWave);
    waveSelector.addItem("Square Wave", squareWave);
    waveSelector.addItem("Triangle Wave", triangleWave);
    waveSelector.onChange = [this] { waveChanged(); };
    waveSelector.setSelectedId(sineWave); // Standaardinstelling
    addAndMakeVisible(waveSelector);
}

MainComponent::~MainComponent()
{

    // This shuts down the audio device and clears the audio source.
    shutdownAudio();
}

void MainComponent::sliderValueChanged(juce::Slider* slider) {
    if (slider == &frequencySlider) {
        frequency = frequencySlider.getValue();
        updateAngleDelta();
    }

}
void MainComponent::updateAngleDelta() {
    double cyclesPerSample = frequency / currentSampleRate;
    angleDelta = cyclesPerSample * 2.0 * juce::MathConstants<double>::pi;
}

//==============================================================================
void MainComponent::prepareToPlay (int samplesPerBlockExpected, double sampleRate)
{
    // This function will be called when the audio device is started, or when
    // its settings (i.e. sample rate, block size, etc) are changed.

    // You can use this function to initialise any resources you might need,
    // but be careful - it will be called on the audio thread, not the GUI thread.

    // For more details, see the help for AudioProcessor::prepareToPlay()

    currentSampleRate = sampleRate;
    updateAngleDelta();
}

void MainComponent::getNextAudioBlock (const juce::AudioSourceChannelInfo& bufferToFill)
{
    // Your audio-processing code goes here!

    // For more details, see the help for AudioProcessor::getNextAudioBlock()

    // Right now we are not producing any data, in which case we need to clear the buffer
    // (to prevent the output of random noise)
    bufferToFill.clearActiveBufferRegion();

    auto* leftChannel = bufferToFill.buffer->getWritePointer(0, bufferToFill.startSample);
    auto* rightChannel = bufferToFill.buffer->getWritePointer(1, bufferToFill.startSample);
	bufferToFill.clearActiveBufferRegion();

    //verschillende waves
    bufferToFill.clearActiveBufferRegion();
    //auto* leftChannel = bufferToFill.buffer->getWritePointer(0, bufferToFill.startSample);
    //auto* rightChannel = bufferToFill.buffer->getWritePointer(1, bufferToFill.startSample);

    for (auto sample = 0; sample < bufferToFill.numSamples; ++sample) {
        float currentSample = 0.0f;
        switch (waveSelector.getSelectedId()) {
        case sineWave:
            currentSample = (float)std::sin(currentAngle);
            break;
        case sawWave:
            currentSample = 2.0f * (currentAngle / (2.0f * M_PI)) - 1.0f;
            currentSample -= floor(currentSample);
            break;
        case squareWave:
            currentSample = std::sin(currentAngle) > 0 ? 1.0f : -1.0f;
            break;
        case triangleWave:
            currentSample = asin(std::sin(currentAngle)) * (2.0f / M_PI);
            break;
        default:
            break;
        }
        currentAngle += angleDelta;
        if (currentAngle >= 2.0 * M_PI) currentAngle -= 2.0 * M_PI; // Zorg ervoor dat currentAngle binnen een cyclus blijft

        leftChannel[sample] = currentSample;
        rightChannel[sample] = currentSample;
    }
}

void MainComponent::releaseResources()
{
    // This will be called when the audio device stops, or when it is being
    // restarted due to a setting change.

    // For more details, see the help for AudioProcessor::releaseResources()
}

//==============================================================================
void MainComponent::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));

    // Set the font size and draw text to the screen
    g.setFont(15.0f);
    g.setColour(juce::Colours::white);
    g.drawText("Synthesizer Controls", 10, 10, 300, 20, juce::Justification::left);
    // You can add your drawing code here!

}

void MainComponent::resized()
{
    // This is called when the MainContentComponent is resized.
    // If you add any child components, this is where you should
    // update their positions.

    auto area = getLocalBounds(); // Verkrijg de totale grootte van je component
    // Stel een gedeelte van de area in voor de frequency slider en label
    auto frequencyArea = area.removeFromTop(200);
	frequencyLabel.setBounds(frequencyArea.removeFromTop(180));
    frequencySlider.setBounds(frequencyArea);

    //wave selector laten zien
    waveSelector.setBounds(getLocalBounds().removeFromTop(100).reduced(10));

}

void MainComponent::waveChanged() {
    auto selectedWave = waveSelector.getSelectedId();
    // Je kunt hier de logica toevoegen om te wisselen tussen golfvormen.
    // Dit kan bijvoorbeeld het instellen van een variabele zijn die gebruikt wordt in getNextAudioBlock.
}
