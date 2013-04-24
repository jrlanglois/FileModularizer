#ifndef JRL_INTERFACE_COMPONENT_H
#define JRL_INTERFACE_COMPONENT_H

#include "Modularizer.h"

class InterfaceComponent : public juce::Component,
                           public juce::ListBoxModel,
                           public juce::Button::Listener
{
public:
    InterfaceComponent();

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
    ScopedPointer<GroupComponent> grpClassConfiguration;
    ScopedPointer<ListBox> fileListBox;
    ScopedPointer<TextButton> btnGenerate;
    ScopedPointer<TextButton> btnBrowse;
    ScopedPointer<Label> lblFileName;
    ScopedPointer<Label> lblHeaderGuard;
    ScopedPointer<Label> lblDestinationFolder;
    ScopedPointer<TextEditor> txtSourceFileFolder;
    ScopedPointer<TextEditor> txtModuleFilename;
    ScopedPointer<TextEditor> txtHeaderGuard;
    ScopedPointer<TextEditor> txtDestinationFolder;

    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (InterfaceComponent)
};

#endif //JRL_INTERFACE_COMPONENT_H