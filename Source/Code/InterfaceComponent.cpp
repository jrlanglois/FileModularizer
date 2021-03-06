#include "InterfaceComponent.h"

InterfaceComponent::InterfaceComponent()
{
    btnGenerate.setButtonText ("Generate");
    btnGenerate.setTooltip ("Generates a module at a specified folder");
    btnGenerate.addListener (this);
    btnGenerate.setColour (juce::TextButton::buttonColourId, juce::Colour (0xff3dc40d));
    btnGenerate.setColour (juce::TextButton::buttonOnColourId, juce::Colour (0xff007a06));

    btnBrowse.setButtonText ("Browse folder...");
    btnBrowse.setConnectedEdges (juce::Button::ConnectedOnRight);
    btnBrowse.setColour (juce::TextButton::buttonColourId, juce::Colour (0xffbababa));
    btnBrowse.setColour (juce::TextButton::buttonOnColourId, juce::Colour (0xff909090));
    btnBrowse.addListener (this);

    btnRefresh.setButtonText ("Refresh");
    btnRefresh.setConnectedEdges (juce::Button::ConnectedOnLeft);
    btnRefresh.setColour (juce::TextButton::buttonColourId, juce::Colour (0xffbababa));
    btnRefresh.setColour (juce::TextButton::buttonOnColourId, juce::Colour (0xff909090));
    btnRefresh.addListener (this);

    fileListBox.setColour (juce::ListBox::outlineColourId, juce::Colours::white.withAlpha (0.5f));
    fileListBox.setColour (juce::ListBox::textColourId, juce::Colours::white);
    fileListBox.setColour (juce::ListBox::backgroundColourId, juce::Colours::darkgrey.brighter().withAlpha (0.5f));
    fileListBox.setMultipleSelectionEnabled (true);
    fileListBox.setModel (this);

    grpClassConfiguration.setColour (juce::GroupComponent::ColourIds::textColourId, juce::Colours::white);
    grpClassConfiguration.setText ("Module Class Configuration");

    addLabel (lblFileName, "Module Name:", "Will be the name for header and CPP files");
    addLabel (lblHeaderGuard, "Header Guard:", "Will be the header file\'s include guard.");
    addLabel (lblNamespace, "Namespace:", "Optional. Will wrap the headers and source files with the namespace name.");
    addLabel (lblDestinationFolder, "Destination Folder:", "Folder that will contain the newly created module file for the listed files.");

    addTextEditor (txtSourceFileFolder, juce::String::empty, true);
    addTextEditor (txtModuleFilename, "MyModule");
    addTextEditor (txtHeaderGuard, "MYMODULE_H");
    addTextEditor (txtNamespace);
    addTextEditor (txtDestinationFolder);

    addAndMakeVisible (&fileListBox);
    addAndMakeVisible (&grpClassConfiguration);
    addAndMakeVisible (&btnRefresh);
    addAndMakeVisible (&btnGenerate);
    addAndMakeVisible (&btnBrowse);

    setSize (800, 600);
    startTimer (timerIntervalMS);
}

//==============================================================================
void InterfaceComponent::paint (juce::Graphics& g)
{
    g.fillAll (juce::Colours::darkgrey);
}

void InterfaceComponent::resized()
{
    const int numConfigItems = labelList.size();

    const int margin = 8;
    const int halfMargin = margin / 2;
    const int marginDouble = margin * 2;
    const int buttonWidth = (margin * margin) * 2;
    const int itemHeight = margin * 3;

    btnRefresh.setBounds (getWidth() - margin - buttonWidth, margin, buttonWidth, itemHeight);
    btnBrowse.setBounds (btnRefresh.getX() - buttonWidth, margin, buttonWidth, itemHeight);
    txtSourceFileFolder.setBounds (margin, margin, btnBrowse.getX() - marginDouble, itemHeight);

    {
        const int height = marginDouble + (itemHeight * numConfigItems) + (margin * numConfigItems);

        grpClassConfiguration.setBounds (margin, getHeight() - height - margin, getWidth() - marginDouble, height);
    }

    {
        const int width = getLargestTextWidth() + margin;

        lblFileName.setBounds (marginDouble, grpClassConfiguration.getY() + halfMargin + marginDouble, width, itemHeight);
        lblHeaderGuard.setBounds (marginDouble, lblFileName.getBottom() + halfMargin, width, itemHeight);
        lblNamespace.setBounds (marginDouble, lblHeaderGuard.getBottom() + halfMargin, width, itemHeight);
        lblDestinationFolder.setBounds (marginDouble, lblNamespace.getBottom() + halfMargin, width, itemHeight);
    }

    {
        const int x = lblFileName.getRight() + margin;
        const int boxWidth = grpClassConfiguration.getRight() - x - marginDouble;

        txtModuleFilename.setBounds (x, lblFileName.getY(), boxWidth, itemHeight);
        txtHeaderGuard.setBounds (x, lblHeaderGuard.getY(), boxWidth, itemHeight);
        txtNamespace.setBounds (x, lblNamespace.getY(), boxWidth, itemHeight);

        const int lastItemY = lblDestinationFolder.getY();

        txtDestinationFolder.setBounds (x, lastItemY, boxWidth - margin - buttonWidth, itemHeight);
        btnGenerate.setBounds (txtDestinationFolder.getRight() + margin, lastItemY, buttonWidth, itemHeight);
    }

    fileListBox.setBounds (margin,
                            txtSourceFileFolder.getBottom() + margin,
                            getWidth() - marginDouble,
                            grpClassConfiguration.getY() - txtSourceFileFolder.getBottom() - marginDouble);
}

void InterfaceComponent::deleteKeyPressed (const int /*lastRowSelected*/)
{
    const juce::SparseSet<int> selectedRows (fileListBox.getSelectedRows());

    for (int i = 0; i < selectedRows.size(); ++i)
    {
        int index = selectedRows[i];

        if (i > 0)
            index -= i;

        files.remove (index);
    }

    fileListBox.setSelectedRows (juce::SparseSet<int>());
    fileListBox.updateContent();
}

void InterfaceComponent::backgroundClicked (const juce::MouseEvent&)
{
    fileListBox.setSelectedRows (juce::SparseSet<int>());
}

void InterfaceComponent::buttonClicked (juce::Button* button)
{
    if (button == &btnBrowse)
    {
        juce::FileChooser chooser ("Select the folder which contains the code files you want to modularise");
        
        if (chooser.browseForDirectory())
        {
            const juce::File folder (chooser.getResult());

            txtSourceFileFolder.setText (folder.getFullPathName().trim(), juce::sendNotification);
            txtDestinationFolder.setText (folder.getParentDirectory().getFullPathName().trim(), juce::sendNotification);

            files = Modulariser (folder, true).getFiles();

            fileListBox.setSelectedRows (juce::SparseSet<int>());
            fileListBox.updateContent();
        }
    }
    else if (button == &btnRefresh)
    {
        refresh();
    }
    else if (button == &btnGenerate)
    {
        const juce::String srcFolder (txtSourceFileFolder.getText().trim());
        const juce::String destFolder (txtDestinationFolder.getText().trim());

        if (srcFolder.isNotEmpty()
            && destFolder.isNotEmpty()
            && juce::File::isAbsolutePath (srcFolder)
            && juce::File::isAbsolutePath (destFolder))
        {
            Modulariser modulariser (files);

            modulariser.saveTo (juce::File (destFolder),
                                srcFolder,
                                txtModuleFilename.getText().trim(),
                                txtHeaderGuard.getText().trim(),
                                txtNamespace.getText().trim());
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
    if (isRowSelected)
    {
        g.fillAll (juce::Colours::white.darker());
        g.setColour (juce::Colours::white.darker().contrasting());
    }
    else
    {
        g.setColour (fileListBox.findColour (juce::ListBox::textColourId));
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

void InterfaceComponent::textEditorTextChanged (juce::TextEditor& editor)
{
    if (&editor == &txtSourceFileFolder)
    {
        stopTimer();
    }
    else
    {
        startTimer (timerIntervalMS);
    }
}

void InterfaceComponent::textEditorFocusLost (juce::TextEditor& editor)
{
    if (&editor == &txtSourceFileFolder)
    {
        startTimer (timerIntervalMS);
    }
}

void InterfaceComponent::timerCallback()
{
    refresh();
}

//==============================================================================
void InterfaceComponent::addLabel (juce::Label& label,
                                   const juce::String& text,
                                   const juce::String& tooltip)
{
    label.setText (text, juce::NotificationType::dontSendNotification);
    label.setTooltip (tooltip);
    label.setFont (juce::Font (15.0f, juce::Font::plain));
    label.setJustificationType (juce::Justification::centredRight);
    label.setEditable (false, false, false);
    label.setColour (juce::Label::textColourId, juce::Colours::white);
    label.setColour (juce::Label::backgroundColourId, juce::Colour (juce::Colours::transparentBlack));

    addAndMakeVisible (&label);
    labelList.add (&label);
}

void InterfaceComponent::addTextEditor (juce::TextEditor& editor,
                                        const juce::String& text,
                                        const bool addListener)
{
    editor.setText (text);
    editor.setMultiLine (false);
    editor.setReturnKeyStartsNewLine (false);
    editor.setReadOnly (false);
    editor.setScrollbarsShown (true);
    editor.setCaretVisible (true);
    editor.setPopupMenuEnabled (true);

    if (addListener)
        editor.addListener (this);

    addAndMakeVisible (&editor);
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

void InterfaceComponent::refresh()
{
    const juce::String folderPath (txtSourceFileFolder.getText().trim());

    if (folderPath.isNotEmpty()
        && juce::File::isAbsolutePath (folderPath))
    {
        const juce::File folder (folderPath);

        if (folder.isDirectory())
        {
            txtDestinationFolder.setText (folder.getParentDirectory().getFullPathName().trim(), juce::sendNotification);

            const juce::StringArray fileList = Modulariser (folder, true).getFiles();

            if (fileList != files)
            {
                files = fileList;
                fileListBox.setSelectedRows (juce::SparseSet<int>());
                fileListBox.updateContent();
            }
        }
    }
}