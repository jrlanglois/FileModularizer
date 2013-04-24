#include "InterfaceComponent.h"

InterfaceComponent::InterfaceComponent()
{
    addAndMakeVisible (grpClassConfiguration = new GroupComponent ("grpClassConfiguration", "Class Configuration"));
    grpClassConfiguration->setColour (GroupComponent::ColourIds::textColourId, Colours::white);

    addAndMakeVisible (fileListBox = new ListBox ("fileListBox", this));
    fileListBox->setColour (ListBox::outlineColourId, Colours::white.withAlpha (0.5f));
    fileListBox->setColour (ListBox::textColourId, Colours::white);
    fileListBox->setColour (ListBox::backgroundColourId, Colours::darkgrey.brighter().withAlpha (0.5f));

    addAndMakeVisible (btnGenerate = new TextButton ("btnGenerate"));
    btnGenerate->setTooltip ("Generates a module at a specified folder");
    btnGenerate->setButtonText ("Generate");
    btnGenerate->addListener (this);
    btnGenerate->setColour (TextButton::buttonColourId, Colour (0xff3dc40d));
    btnGenerate->setColour (TextButton::buttonOnColourId, Colour (0xff007a06));

    addAndMakeVisible (btnBrowse = new TextButton ("btnBrowse"));
    btnBrowse->setButtonText ("Browse...");
    btnBrowse->setConnectedEdges (Button::ConnectedOnLeft | Button::ConnectedOnRight);
    btnBrowse->addListener (this);
    btnBrowse->setColour (TextButton::buttonColourId, Colour (0xffbababa));
    btnBrowse->setColour (TextButton::buttonOnColourId, Colour (0xff909090));

    addAndMakeVisible (lblFileName = new Label ("lblFileName", "Module Filename:"));
    lblFileName->setTooltip ("Will be the name for header and CPP files");
    lblFileName->setFont (Font (15.00f, Font::plain));
    lblFileName->setJustificationType (Justification::centredRight);
    lblFileName->setEditable (false, false, false);
    lblFileName->setColour (Label::textColourId, Colours::white);
    lblFileName->setColour (Label::backgroundColourId, Colour (juce::Colours::transparentBlack));

    addAndMakeVisible (lblHeaderGuard = new Label ("lblHeaderGuard", "Header Guard:"));
    lblHeaderGuard->setTooltip ("Will be the header file\'s include guard");
    lblHeaderGuard->setFont (Font (15.00f, Font::plain));
    lblHeaderGuard->setJustificationType (Justification::centredRight);
    lblHeaderGuard->setEditable (false, false, false);
    lblHeaderGuard->setColour (Label::textColourId, Colours::white);
    lblHeaderGuard->setColour (Label::backgroundColourId, Colour (juce::Colours::transparentBlack));

    addAndMakeVisible (lblDestinationFolder = new Label ("lblDestinationFolder", "Destination Folder:"));
    lblDestinationFolder->setTooltip ("Will be the header file\'s include guard");
    lblDestinationFolder->setFont (Font (15.00f, Font::plain));
    lblDestinationFolder->setJustificationType (Justification::centredRight);
    lblDestinationFolder->setEditable (false, false, false);
    lblDestinationFolder->setColour (Label::textColourId, Colours::white);
    lblDestinationFolder->setColour (Label::backgroundColourId, Colour (juce::Colours::transparentBlack));

    addAndMakeVisible (txtSourceFileFolder = new TextEditor ("txtSourceFileFolder"));
    txtSourceFileFolder->setMultiLine (false);
    txtSourceFileFolder->setReturnKeyStartsNewLine (false);
    txtSourceFileFolder->setReadOnly (false);
    txtSourceFileFolder->setScrollbarsShown (true);
    txtSourceFileFolder->setCaretVisible (true);
    txtSourceFileFolder->setPopupMenuEnabled (true);
    txtSourceFileFolder->setText ("C:/Users/UUO/Documents/Work/noteloop/Source/Modules/Applications/");

    Modularizer mod (juce::File ("C:/Users/UUO/Documents/Work/noteloop/Source/Modules/Applications/"), true);
    files = mod.getFiles();

    addAndMakeVisible (txtModuleFilename = new TextEditor ("txtModuleFilename"));
    txtModuleFilename->setMultiLine (false);
    txtModuleFilename->setReturnKeyStartsNewLine (false);
    txtModuleFilename->setReadOnly (false);
    txtModuleFilename->setScrollbarsShown (true);
    txtModuleFilename->setCaretVisible (true);
    txtModuleFilename->setPopupMenuEnabled (true);
    txtModuleFilename->setText ("MyModule");

    addAndMakeVisible (txtHeaderGuard = new TextEditor ("txtHeaderGuard"));
    txtHeaderGuard->setMultiLine (false);
    txtHeaderGuard->setReturnKeyStartsNewLine (false);
    txtHeaderGuard->setReadOnly (false);
    txtHeaderGuard->setScrollbarsShown (true);
    txtHeaderGuard->setCaretVisible (true);
    txtHeaderGuard->setPopupMenuEnabled (true);
    txtHeaderGuard->setText ("MYCLASS_H");

    addAndMakeVisible (txtDestinationFolder = new TextEditor ("txtDestinationFolder"));
    txtDestinationFolder->setMultiLine (false);
    txtDestinationFolder->setReturnKeyStartsNewLine (false);
    txtDestinationFolder->setReadOnly (false);
    txtDestinationFolder->setScrollbarsShown (true);
    txtDestinationFolder->setCaretVisible (true);
    txtDestinationFolder->setPopupMenuEnabled (true);
    txtDestinationFolder->setText ("C:/");

    setSize (800, 600);
}

InterfaceComponent::~InterfaceComponent()
{
    grpClassConfiguration = nullptr;
    btnGenerate = nullptr;
    btnBrowse = nullptr;
    txtSourceFileFolder = nullptr;
    txtHeaderGuard = nullptr;
    txtModuleFilename = nullptr;
    txtDestinationFolder = nullptr;
    lblFileName = nullptr;
    lblHeaderGuard = nullptr;
    fileListBox = nullptr;
    lblDestinationFolder = nullptr;
}

//==============================================================================
void InterfaceComponent::paint (Graphics& g)
{
    g.fillAll (Colours::darkgrey);
}

void InterfaceComponent::resized()
{
    grpClassConfiguration->setBounds (8, 448, 784, 144);
    fileListBox->setBounds (8, 40, 784, 384);
    btnGenerate->setBounds (632, 552, 150, 24);
    btnBrowse->setBounds (640, 8, 150, 24);
    lblFileName->setBounds (24, 472, 144, 24);
    lblHeaderGuard->setBounds (24, 512, 144, 24);
    lblDestinationFolder->setBounds (24, 552, 144, 24);
    txtSourceFileFolder->setBounds (8, 8, 632, 24);
    txtModuleFilename->setBounds (168, 472, 608, 24);
    txtHeaderGuard->setBounds (170, 513, 608, 24);
    txtDestinationFolder->setBounds (173, 553, 451, 24);
}

void InterfaceComponent::buttonClicked (Button* buttonThatWasClicked)
{
    if (buttonThatWasClicked == btnGenerate)
    {
        Modularizer modularizer (files);

        modularizer.saveTo (juce::File (lblDestinationFolder->getText().trim()),
                            lblFileName->getText().trim(),
                            lblHeaderGuard->getText().trim());
    }
    else if (buttonThatWasClicked == btnBrowse)
    {
        juce::FileChooser chooser ("Select a folder");
        
        if (chooser.browseForDirectory())
        {
            const juce::File folder (chooser.getResult());

            txtSourceFileFolder->setText (folder.getFullPathName().trim(), juce::sendNotification);

            Modularizer modularizer (folder, true);
            files = modularizer.getFiles();

            resized();
        }
    }
}

int InterfaceComponent::getNumRows()
{
    return files.size();
}

void InterfaceComponent::paintListBoxItem (const int rowNumber,
                                           Graphics& g,
                                           const int width, const int height,
                                           const bool isRowSelected)
{
    const juce::Colour textColour (fileListBox->findColour (ListBox::textColourId));

    if (isRowSelected)
    {
        g.setColour (textColour.darker().darker());
        g.fillAll (juce::Colours::white.darker());
    }
    else
    {
        g.setColour (textColour);
    }

    g.setFont (height * 0.7f);

    g.drawText (files[rowNumber],
                5, 0, width, height,
                juce::Justification::centredLeft, false);
}