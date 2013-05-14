#ifndef JRL_INTERFACE_COMPONENT_H
#define JRL_INTERFACE_COMPONENT_H

#include "Modularizer.h"

class InterfaceComponent : public juce::Component,
                           public juce::ListBoxModel,
                           public juce::Button::Listener
{
public:
    /**
    * Constructor
    */
    InterfaceComponent();

    /**
    * Destructor
    */
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

private:
    //==============================================================================
    juce::StringArray files;

    //==============================================================================
    juce::ScopedPointer<juce::GroupComponent> grpClassConfiguration;
    juce::ScopedPointer<juce::ListBox> fileListBox;
    juce::ScopedPointer<juce::TextButton> btnGenerate;
    juce::ScopedPointer<juce::TextButton> btnBrowse;
    juce::ScopedPointer<juce::TextButton> btnRefresh;
    juce::ScopedPointer<juce::Label> lblFileName;
    juce::ScopedPointer<juce::Label> lblHeaderGuard;
    juce::ScopedPointer<juce::Label> lblNamespace;
    juce::ScopedPointer<juce::Label> lblDestinationFolder;
    juce::ScopedPointer<juce::TextEditor> txtSourceFileFolder;
    juce::ScopedPointer<juce::TextEditor> txtModuleFilename;
    juce::ScopedPointer<juce::TextEditor> txtHeaderGuard;
    juce::ScopedPointer<juce::TextEditor> txtNamespace;
    juce::ScopedPointer<juce::TextEditor> txtDestinationFolder;

    int numConfigItems;
    juce::Array<juce::Label*> labelList;

    //==============================================================================
    juce::Label* addLabel (const juce::String& text = juce::String::empty,
                           const juce::String& tooltip = juce::String::empty);

    juce::TextEditor* addTextEditor (const juce::String& text = juce::String::empty);

    int getLargestTextWidth() const;

    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (InterfaceComponent)
};

#endif //JRL_INTERFACE_COMPONENT_H