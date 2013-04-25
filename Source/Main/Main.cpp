#include "MainComponent.h"

class FileModularizerApplication : public JUCEApplication
{
public:
    FileModularizerApplication()    { }
    ~FileModularizerApplication()   { }

    //==============================================================================
    void initialise (const String& /*commandLine*/)
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

    void anotherInstanceStarted (const String& /*commandLine*/)
    {
    }

    //==============================================================================
    const String getApplicationName()       { return ProjectInfo::projectName; }
    const String getApplicationVersion()    { return ProjectInfo::versionString; }
    bool moreThanOneInstanceAllowed()       { return true; }

    //==============================================================================
    class MainWindow : public DocumentWindow
    {
    public:
        MainWindow() : DocumentWindow ("File Modularizer",
                                       Colours::lightgrey,
                                       DocumentWindow::closeButton)
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
    ScopedPointer<MainWindow> mainWindow;
};

//==============================================================================
// This macro generates the main() routine that launches the app.
START_JUCE_APPLICATION (FileModularizerApplication)