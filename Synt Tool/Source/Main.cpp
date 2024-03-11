#include <JuceHeader.h>
#include "MainComponent.h"


//IMPORTANT: in deze class hoef je niks te doen behalve als je wilt kunnen met startup en shutdown
//==============================================================================
class SyntToolApplication  : public juce::JUCEApplication
{
public:
    //==============================================================================
    SyntToolApplication() {}

    const juce::String getApplicationName() override       { return ProjectInfo::projectName; }
    const juce::String getApplicationVersion() override    { return ProjectInfo::versionString; }
    bool moreThanOneInstanceAllowed() override             { return true; }

    //==============================================================================
    void initialise (const juce::String& commandLine) override
    {
        // Voeg hier je initialisatie code toe

        mainWindow.reset (new MainWindow (getApplicationName()));
    }

    void shutdown() override
    {
        // Voeg hier je Shutdown code toe

        mainWindow = nullptr; // (deletes our window)
    }

    //==============================================================================
    void systemRequestedQuit() override
    {
        // Dit wordt gecalled als de app wordt gevraagt af te sluiten: je deze request negeren
        // en de app door laten runnen, of je kan quit() callen om de app te laten sluiten.
        quit();
    }

    void anotherInstanceStarted (const juce::String& commandLine) override
    {

        //dit wordt gecalled als de app all een instance heeft runnen.
    }
    

    //==========================================================
    /*
        deze class zet de main window op
    */
    class MainWindow    : public juce::DocumentWindow
    {
    public:
        MainWindow (juce::String name)
            : DocumentWindow (name,
                              juce::Desktop::getInstance().getDefaultLookAndFeel()
                                                          .findColour (juce::ResizableWindow::backgroundColourId),
                              DocumentWindow::allButtons)
        {
            setUsingNativeTitleBar (true);
            setContentOwned (new MainComponent(), true);

           #if JUCE_IOS || JUCE_ANDROID
            setFullScreen (true);
           #else
            setResizable (true, true);
            centreWithSize (getWidth(), getHeight());
           #endif

            setVisible (true);
        }

        void closeButtonPressed() override
        {
            /*
                dit wordt geroepen als de applicatie sluit. voor nu laten we de applicatie sluiten.
            */
            JUCEApplication::getInstance()->systemRequestedQuit();
        }

        /* Note: Be careful if you override any DocumentWindow methods - the base
           class uses a lot of them, so by overriding you might break its functionality.
           It's best to do all your work in your content component instead, but if
           you really have to override any DocumentWindow methods, make sure your
           subclass also calls the superclass's method.
        */

    private:
        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainWindow)
    };

private:
    std::unique_ptr<MainWindow> mainWindow;
};

//==============================================================================
// This macro generates the main() routine that launches the app.
START_JUCE_APPLICATION (SyntToolApplication)
