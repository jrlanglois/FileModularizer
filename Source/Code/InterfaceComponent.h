#ifndef JRL_INTERFACE_COMPONENT_H
#define JRL_INTERFACE_COMPONENT_H

#include "Modularizer.h"

class InterfaceComponent : public Component,
                           public Button::Listener,
                           public ListBoxModel
{
public:
    InterfaceComponent();
    ~InterfaceComponent();

    //==============================================================================
    /** @internal */
    void paint (Graphics& g);
    /** @internal */
    void resized();
    /** @internal */
    void buttonClicked (Button* buttonThatWasClicked);
    /** @internal */
    int getNumRows();
    /** @internal */
    void paintListBoxItem (int rowNumber,
                           Graphics& g,
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