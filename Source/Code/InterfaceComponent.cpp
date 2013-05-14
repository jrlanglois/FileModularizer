#include "InterfaceComponent.h"

InterfaceComponent::InterfaceComponent() :
    numConfigItems (0)
{
    addAndMakeVisible (btnGenerate = new juce::TextButton ("btnGenerate"));
    btnGenerate->setTooltip ("Generates a module at a specified folder");
    btnGenerate->setButtonText ("Generate");
    btnGenerate->addListener (this);
    btnGenerate->setColour (juce::TextButton::buttonColourId, juce::Colour (0xff3dc40d));
    btnGenerate->setColour (juce::TextButton::buttonOnColourId, juce::Colour (0xff007a06));

    addAndMakeVisible (btnBrowse = new juce::TextButton ("btnBrowse"));
    btnBrowse->setButtonText ("Browse folder...");
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

    addAndMakeVisible (fileListBox = new juce::ListBox ("fileListBox", this));
    fileListBox->setColour (juce::ListBox::outlineColourId, juce::Colours::white.withAlpha (0.5f));
    fileListBox->setColour (juce::ListBox::textColourId, juce::Colours::white);
    fileListBox->setColour (juce::ListBox::backgroundColourId, juce::Colours::darkgrey.brighter().withAlpha (0.5f));
    fileListBox->setMultipleSelectionEnabled (true);

    addAndMakeVisible (grpClassConfiguration = new juce::GroupComponent ("grpClassConfiguration", "Module Class Configuration"));
    grpClassConfiguration->setColour (juce::GroupComponent::ColourIds::textColourId, juce::Colours::white);

    lblFileName = addLabel ("Module Name:", "Will be the name for header and CPP files");
    lblHeaderGuard = addLabel ("Header Guard:", "Will be the header file\'s include guard.");
    lblNamespace = addLabel ("Namespace:", "Optional. Will wrap the headers and source files with the namespace name.");
    lblDestinationFolder = addLabel ("Destination Folder:", "Folder that will contain the newly created module file for the listed files.");

    txtSourceFileFolder = addTextEditor();
    txtModuleFilename = addTextEditor ("MyModule");
    txtHeaderGuard = addTextEditor ("MYMODULE_H");
    txtNamespace = addTextEditor();
    txtDestinationFolder = addTextEditor();

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
    const int margin = 8;
    const int halfMargin = margin / 2;
    const int marginDouble = margin * 2;
    const int buttonWidth = (margin * margin) * 2;
    const int itemHeight = margin * 3;

    btnRefresh->setBounds (getWidth() - margin - buttonWidth, margin, buttonWidth, itemHeight);
    btnBrowse->setBounds (btnRefresh->getX() - buttonWidth, margin, buttonWidth, itemHeight);
    txtSourceFileFolder->setBounds (margin, margin, btnBrowse->getX() - marginDouble, itemHeight);

    {
        const int height = marginDouble + (itemHeight * numConfigItems) + (margin * numConfigItems);

        grpClassConfiguration->setBounds (margin, getHeight() - height - margin, getWidth() - marginDouble, height);
    }

    {
        const int width = getLargestTextWidth() + margin;

        lblFileName->setBounds (marginDouble, grpClassConfiguration->getY() + halfMargin + marginDouble, width, itemHeight);
        lblHeaderGuard->setBounds (marginDouble, lblFileName->getBottom() + halfMargin, width, itemHeight);
        lblNamespace->setBounds (marginDouble, lblHeaderGuard->getBottom() + halfMargin, width, itemHeight);
        lblDestinationFolder->setBounds (marginDouble, lblNamespace->getBottom() + halfMargin, width, itemHeight);
    }

    {
        const int x = lblFileName->getRight() + margin;
        const int boxWidth = grpClassConfiguration->getRight() - x - marginDouble;

        txtModuleFilename->setBounds (x, lblFileName->getY(), boxWidth, itemHeight);
        txtHeaderGuard->setBounds (x, lblHeaderGuard->getY(), boxWidth, itemHeight);
        txtNamespace->setBounds (x, lblNamespace->getY(), boxWidth, itemHeight);

        const int lastItemY = lblDestinationFolder->getY();

        txtDestinationFolder->setBounds (x, lastItemY, boxWidth - margin - buttonWidth, itemHeight);
        btnGenerate->setBounds (txtDestinationFolder->getRight() + margin, lastItemY, buttonWidth, itemHeight);
    }

    fileListBox->setBounds (margin,
                            txtSourceFileFolder->getBottom() + margin,
                            getWidth() - marginDouble,
                            grpClassConfiguration->getY() - txtSourceFileFolder->getBottom() - marginDouble);
}

void InterfaceComponent::deleteKeyPressed (const int /*lastRowSelected*/)
{
    const juce::SparseSet<int> selectedRows (fileListBox->getSelectedRows());

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
    if (buttonThatWasClicked == btnBrowse)
    {
        juce::FileChooser chooser ("Select the folder which contains the code files you want to modularize");
        
        if (chooser.browseForDirectory())
        {
            const juce::File folder (chooser.getResult());

            txtSourceFileFolder->setText (folder.getFullPathName().trim(), juce::sendNotification);
            txtDestinationFolder->setText (folder.getParentDirectory().getFullPathName().trim(), juce::sendNotification);

            files = Modularizer (folder, true).getFiles();

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

            files = Modularizer (folder, true).getFiles();

            fileListBox->setSelectedRows (juce::SparseSet<int>());
            fileListBox->updateContent();
        }
    }
    else if (buttonThatWasClicked == btnGenerate)
    {
        const juce::String srcFolder = txtSourceFileFolder->getText().trim();
        const juce::String destFolder = txtDestinationFolder->getText().trim();

        if (srcFolder.isNotEmpty()
            && destFolder.isNotEmpty()
            && juce::File::isAbsolutePath (srcFolder)
            && juce::File::isAbsolutePath (destFolder))
        {
            Modularizer modularizer (files);

            modularizer.saveTo (juce::File (destFolder),
                                srcFolder,
                                txtModuleFilename->getText().trim(),
                                txtHeaderGuard->getText().trim(),
                                txtNamespace->getText().trim());
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

//==============================================================================
juce::Label* InterfaceComponent::addLabel (const juce::String& text,
                                           const juce::String& tooltip)
{
    juce::Label* label = new juce::Label();
    label->setText (text, juce::NotificationType::dontSendNotification);
    label->setTooltip (tooltip);
    label->setFont (juce::Font (15.0f, juce::Font::plain));
    label->setJustificationType (juce::Justification::centredRight);
    label->setEditable (false, false, false);
    label->setColour (juce::Label::textColourId, juce::Colours::white);
    label->setColour (juce::Label::backgroundColourId, juce::Colour (juce::Colours::transparentBlack));
    addAndMakeVisible (label);

    ++numConfigItems;
    labelList.add (label);

    return label;
}

juce::TextEditor* InterfaceComponent::addTextEditor (const juce::String& text)
{
    juce::TextEditor* ed = new juce::TextEditor();
    ed->setText (text);
    ed->setMultiLine (false);
    ed->setReturnKeyStartsNewLine (false);
    ed->setReadOnly (false);
    ed->setScrollbarsShown (true);
    ed->setCaretVisible (true);
    ed->setPopupMenuEnabled (true);
    addAndMakeVisible (ed);

    return ed;
}

int InterfaceComponent::getLargestTextWidth() const
{
    int largestWidth = 0;

    for (int i = labelList.size(); --i >= 0;)
    {
        const juce::Label* label = labelList.getUnchecked (i);
        const int width = label->getFont().getStringWidth (label->getText());

        if (width > largestWidth)
            largestWidth = width;
    }

    return largestWidth;
}