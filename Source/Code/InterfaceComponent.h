#ifndef JRL_INTERFACE_COMPONENT_H
#define JRL_INTERFACE_COMPONENT_H

#include "Modulariser.h"

class InterfaceComponent : public juce::Component,
                           private juce::ListBoxModel,
                           private juce::Button::Listener,
                           private juce::TextEditor::Listener,
                           private juce::Timer
{
public:
    /** Constructor */
    InterfaceComponent();

    /** Destructor */
    ~InterfaceComponent();

    //==============================================================================
    /** @internal */
    void paint (juce::Graphics& g);
    /** @internal */
    void resized();
    /** @internal */
    void deleteKeyPressed (int lastRowSelected);
    /** @internal */
    void backgroundClicked();
    /** @internal */
    void buttonClicked (juce::Button* buttonThatWasClicked);
    /** @internal */
    int getNumRows();
    /** @internal */
    void paintListBoxItem (int rowNumber,
                           juce::Graphics& g,
                           int width, int height,
                           bool rowIsSelected);
    /** @internal */
    void textEditorTextChanged (juce::TextEditor& editor);
    /** @internal */
    void textEditorFocusLost (juce::TextEditor& editor);
    /** @internal */
    void timerCallback();

    //==============================================================================
    static const int timerIntervalMS = 8000;

private:
    //==============================================================================
    juce::StringArray files;

    //==============================================================================
    juce::GroupComponent grpClassConfiguration;
    juce::ListBox fileListBox;
    juce::TextButton btnGenerate;
    juce::TextButton btnBrowse;
    juce::TextButton btnRefresh;
    juce::Label lblFileName;
    juce::Label lblHeaderGuard;
    juce::Label lblNamespace;
    juce::Label lblDestinationFolder;
    juce::TextEditor txtSourceFileFolder;
    juce::TextEditor txtModuleFilename;
    juce::TextEditor txtHeaderGuard;
    juce::TextEditor txtNamespace;
    juce::TextEditor txtDestinationFolder;

    juce::Array<juce::Label*> labelList;

    //==============================================================================
    void addLabel (juce::Label& label,
                   const juce::String& text,
                   const juce::String& tooltip);

    void addTextEditor (juce::TextEditor& editor,
                        const juce::String& text = juce::String::empty,
                        bool addListener = false);

    int getLargestTextWidth() const;

    void refresh();

    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (InterfaceComponent)
};

#endif //JRL_INTERFACE_COMPONENT_H