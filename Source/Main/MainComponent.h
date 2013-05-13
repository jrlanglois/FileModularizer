#ifndef JRL_MAIN_COMPONENT_H
#define JRL_MAIN_COMPONENT_H

#include "../Code/InterfaceComponent.h"

class MainComponent : public juce::Component
{
public:
    MainComponent();
    
    ~MainComponent();

    //==============================================================================
    /** @internal */
    void paint (juce::Graphics& g);
    /** @internal */
    void resized();

private:
    //==============================================================================
    InterfaceComponent component;
    
    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainComponent)
};

#endif //JRL_MAIN_COMPONENT_H