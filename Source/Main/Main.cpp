#include "MainComponent.h"

class FileModularizerApplication : public juce::JUCEApplication
{
public:
    FileModularizerApplication()    { }
    ~FileModularizerApplication()   { }

    //==============================================================================
    void initialise (const juce::String& /*commandLine*/)
    {
        mainWindow = new MainWindow();
    }

    void shutdown()
    {
        mainWindow = nullptr;
    }

    //==============================================================================
    void systemRequestedQuit()
    {
        quit();
    }

    void anotherInstanceStarted (const juce::String& /*commandLine*/)
    {
    }

    //==============================================================================
    const juce::String getApplicationName()     { return ProjectInfo::projectName; }
    const juce::String getApplicationVersion()  { return ProjectInfo::versionString; }
    bool moreThanOneInstanceAllowed()           { return true; }

    //==============================================================================
    class MainWindow : public juce::DocumentWindow
    {
    public:
        MainWindow() : juce::DocumentWindow ("File Modularizer",
                                             juce::Colours::lightgrey,
                                             juce::DocumentWindow::closeButton)
        {
            setContentOwned (new MainComponent(), true);

            centreWithSize (getWidth(), getHeight());
            setVisible (true);
        }

        void closeButtonPressed()
        {
            JUCEApplication::getInstance()->systemRequestedQuit();
        }

    private:
        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainWindow)
    };

private:
    juce::ScopedPointer<MainWindow> mainWindow;
};

//==============================================================================
// This macro generates the main() routine that launches the app.
START_JUCE_APPLICATION (FileModularizerApplication)