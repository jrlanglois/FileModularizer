#include "InterfaceComponent.h"

InterfaceComponent::InterfaceComponent()
{
    addAndMakeVisible (grpClassConfiguration = new juce::GroupComponent ("grpClassConfiguration", "Class Configuration"));
    grpClassConfiguration->setColour (juce::GroupComponent::ColourIds::textColourId, juce::Colours::white);

    addAndMakeVisible (fileListBox = new juce::ListBox ("fileListBox", this));
    fileListBox->setColour (juce::ListBox::outlineColourId, juce::Colours::white.withAlpha (0.5f));
    fileListBox->setColour (juce::ListBox::textColourId, juce::Colours::white);
    fileListBox->setColour (juce::ListBox::backgroundColourId, juce::Colours::darkgrey.brighter().withAlpha (0.5f));
    fileListBox->setMultipleSelectionEnabled (true);

    addAndMakeVisible (btnGenerate = new juce::TextButton ("btnGenerate"));
    btnGenerate->setTooltip ("Generates a module at a specified folder");
    btnGenerate->setButtonText ("Generate");
    btnGenerate->addListener (this);
    btnGenerate->setColour (juce::TextButton::buttonColourId, juce::Colour (0xff3dc40d));
    btnGenerate->setColour (juce::TextButton::buttonOnColourId, juce::Colour (0xff007a06));

    addAndMakeVisible (btnBrowse = new juce::TextButton ("btnBrowse"));
    btnBrowse->setButtonText ("Browse...");
    btnBrowse->setConnectedEdges (juce::Button::ConnectedOnRight);
    btnBrowse->setColour (juce::TextButton::buttonColourId, juce::Colour (0xffbababa));
    btnBrowse->setColour (juce::TextButton::buttonOnColourId, juce::Colour (0xff909090));
    btnBrowse->addListener (this);

    addAndMakeVisible (btnRefresh = new juce::TextButton ("btnRefresh"));
    btnRefresh->setButtonText ("Refresh");
    btnRefresh->setConnectedEdges (juce::Button::ConnectedOnLeft);
    btnRefresh->setColour (juce::TextButton::buttonColourId, juce::Colour (0xffbababa));
    btnRefresh->setColour (juce::TextButton::buttonOnColourId, juce::Colour (0xff909090));
    btnRefresh->addListener (this);

    addAndMakeVisible (lblFileName = new juce::Label ("lblFileName", "Module Filename:"));
    lblFileName->setTooltip ("Will be the name for header and CPP files");
    lblFileName->setFont (juce::Font (15.00f, juce::Font::plain));
    lblFileName->setJustificationType (juce::Justification::centredRight);
    lblFileName->setEditable (false, false, false);
    lblFileName->setColour (juce::Label::textColourId, juce::Colours::white);
    lblFileName->setColour (juce::Label::backgroundColourId, juce::Colour (juce::Colours::transparentBlack));

    addAndMakeVisible (lblHeaderGuard = new juce::Label ("lblHeaderGuard", "Header Guard:"));
    lblHeaderGuard->setTooltip ("Will be the header file\'s include guard");
    lblHeaderGuard->setFont (juce::Font (15.00f, juce::Font::plain));
    lblHeaderGuard->setJustificationType (juce::Justification::centredRight);
    lblHeaderGuard->setEditable (false, false, false);
    lblHeaderGuard->setColour (juce::Label::textColourId, juce::Colours::white);
    lblHeaderGuard->setColour (juce::Label::backgroundColourId, juce::Colour (juce::Colours::transparentBlack));

    addAndMakeVisible (lblDestinationFolder = new juce::Label ("lblDestinationFolder", "Destination Folder:"));
    lblDestinationFolder->setTooltip ("Will be the header file\'s include guard");
    lblDestinationFolder->setFont (juce::Font (15.00f, juce::Font::plain));
    lblDestinationFolder->setJustificationType (juce::Justification::centredRight);
    lblDestinationFolder->setEditable (false, false, false);
    lblDestinationFolder->setColour (juce::Label::textColourId, juce::Colours::white);
    lblDestinationFolder->setColour (juce::Label::backgroundColourId, juce::Colour (juce::Colours::transparentBlack));

    addAndMakeVisible (txtSourceFileFolder = new juce::TextEditor ("txtSourceFileFolder"));
    txtSourceFileFolder->setMultiLine (false);
    txtSourceFileFolder->setReturnKeyStartsNewLine (false);
    txtSourceFileFolder->setReadOnly (false);
    txtSourceFileFolder->setScrollbarsShown (true);
    txtSourceFileFolder->setCaretVisible (true);
    txtSourceFileFolder->setPopupMenuEnabled (true);
    txtSourceFileFolder->setText ("");

    addAndMakeVisible (txtModuleFilename = new juce::TextEditor ("txtModuleFilename"));
    txtModuleFilename->setMultiLine (false);
    txtModuleFilename->setReturnKeyStartsNewLine (false);
    txtModuleFilename->setReadOnly (false);
    txtModuleFilename->setScrollbarsShown (true);
    txtModuleFilename->setCaretVisible (true);
    txtModuleFilename->setPopupMenuEnabled (true);
    txtModuleFilename->setText ("MyModule");

    addAndMakeVisible (txtHeaderGuard = new juce::TextEditor ("txtHeaderGuard"));
    txtHeaderGuard->setMultiLine (false);
    txtHeaderGuard->setReturnKeyStartsNewLine (false);
    txtHeaderGuard->setReadOnly (false);
    txtHeaderGuard->setScrollbarsShown (true);
    txtHeaderGuard->setCaretVisible (true);
    txtHeaderGuard->setPopupMenuEnabled (true);
    txtHeaderGuard->setText ("MYCLASS_H");

    addAndMakeVisible (txtDestinationFolder = new juce::TextEditor ("txtDestinationFolder"));
    txtDestinationFolder->setMultiLine (false);
    txtDestinationFolder->setReturnKeyStartsNewLine (false);
    txtDestinationFolder->setReadOnly (false);
    txtDestinationFolder->setScrollbarsShown (true);
    txtDestinationFolder->setCaretVisible (true);
    txtDestinationFolder->setPopupMenuEnabled (true);
    txtDestinationFolder->setText ("");

    setSize (800, 600);
}

InterfaceComponent::~InterfaceComponent()
{
    grpClassConfiguration = nullptr;
    btnGenerate = nullptr;
    btnBrowse = nullptr;
    btnRefresh = nullptr;
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
void InterfaceComponent::paint (juce::Graphics& g)
{
    g.fillAll (juce::Colours::darkgrey);
}

void InterfaceComponent::resized()
{
    grpClassConfiguration->setBounds (8, 448, 784, 144);
    fileListBox->setBounds (8, 40, 784, 384);
    btnGenerate->setBounds (632, 552, 150, 24);
    btnBrowse->setBounds (640, 8, 75, 24);
    btnRefresh->setBounds (715, 8, 75, 24);
    lblFileName->setBounds (24, 472, 144, 24);
    lblHeaderGuard->setBounds (24, 512, 144, 24);
    lblDestinationFolder->setBounds (24, 552, 144, 24);
    txtSourceFileFolder->setBounds (8, 8, 632, 24);
    txtModuleFilename->setBounds (168, 472, 608, 24);
    txtHeaderGuard->setBounds (170, 513, 608, 24);
    txtDestinationFolder->setBounds (173, 553, 451, 24);
}

void InterfaceComponent::deleteKeyPressed (const int /*lastRowSelected*/)
{
    juce::SparseSet<int> selectedRows (fileListBox->getSelectedRows());

    for (int i = 0; i < selectedRows.size(); ++i)
    {
        int index = selectedRows[i];

        if (i > 0)
            index -= i;

        files.remove (index);
    }

    fileListBox->setSelectedRows (juce::SparseSet<int>());
    fileListBox->updateContent();
}

void InterfaceComponent::backgroundClicked()
{
    fileListBox->setSelectedRows (juce::SparseSet<int>());
}

void InterfaceComponent::buttonClicked (juce::Button* buttonThatWasClicked)
{
    if (buttonThatWasClicked == btnGenerate)
    {
        Modularizer modularizer (files);

        modularizer.saveTo (juce::File (txtDestinationFolder->getText().trim()),
                            txtSourceFileFolder->getText().trim(),
                            txtModuleFilename->getText().trim(),
                            txtHeaderGuard->getText().trim());
    }
    else if (buttonThatWasClicked == btnBrowse)
    {
        juce::FileChooser chooser ("Select a folder");
        
        if (chooser.browseForDirectory())
        {
            const juce::File folder (chooser.getResult());

            txtSourceFileFolder->setText (folder.getFullPathName().trim(), juce::sendNotification);
            txtDestinationFolder->setText (folder.getParentDirectory().getFullPathName().trim(), juce::sendNotification);

            Modularizer modularizer (folder, true);
            files = modularizer.getFiles();

            fileListBox->setSelectedRows (juce::SparseSet<int>());
            fileListBox->updateContent();
        }
    }
    else if (buttonThatWasClicked == btnRefresh)
    {
        const juce::File folder (txtSourceFileFolder->getText().trim());

        if (folder.isDirectory())
        {
            txtDestinationFolder->setText (folder.getParentDirectory().getFullPathName().trim(), juce::sendNotification);

            Modularizer modularizer (folder, true);
            files = modularizer.getFiles();

            fileListBox->setSelectedRows (juce::SparseSet<int>());
            fileListBox->updateContent();
        }
    }
}

int InterfaceComponent::getNumRows()
{
    return files.size();
}

void InterfaceComponent::paintListBoxItem (const int rowNumber,
                                           juce::Graphics& g,
                                           const int width, const int height,
                                           const bool isRowSelected)
{
    const juce::Colour textColour (fileListBox->findColour (juce::ListBox::textColourId));

    if (isRowSelected)
    {
        g.fillAll (juce::Colours::white.darker());
        g.setColour (juce::Colours::white.darker().contrasting());
    }
    else
    {
        g.setColour (textColour);
    }

    g.setFont (height * 0.7f);

    g.drawText (files[rowNumber],
                5, 0, width, height,
                juce::Justification::centredLeft, false);

    const float h = (float) height;
    const float w = (float) width;
    const float offset = 5.0f;
    const float thickness = 0.25f;

    g.setColour (juce::Colours::lightgrey);
    g.drawLine (offset, h - thickness,
                w - (offset * 2.0f), h - thickness,
                thickness);
}