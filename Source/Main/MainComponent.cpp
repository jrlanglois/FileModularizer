#include "MainComponent.h"

MainComponent::MainComponent()
{
    setSize (component.getWidth(), component.getHeight());
    addAndMakeVisible (&component);
}

MainComponent::~MainComponent()
{
}

//==============================================================================
void MainComponent::paint (Graphics& /*g*/)
{
}

void MainComponent::resized()
{
    component.setTopLeftPosition (0, 0);
}