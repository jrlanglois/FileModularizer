#ifndef JRL_MODULARIZER_H
#define JRL_MODULARIZER_H

#include "JuceHeader.h"

class Modularizer
{
public:
    Modularizer (const juce::StringArray& files);

    Modularizer (const juce::File& folder,
                 bool searchRecursively);

    ~Modularizer();

    //==============================================================================
    void copyAndfilterBadFiles (const juce::StringArray& files);

    void saveTo (const juce::File& destinationFolder,
                 const juce::String& sourceFolderToRemove,
                 const juce::String& moduleName,
                 const juce::String& headerGuard,
                 const juce::String& desiredNamespace);

    bool containsOldCStyleCodeFiles() const;

    //==============================================================================
    inline juce::StringArray getFiles() const noexcept { return files; }

    juce::String getWildcards() const;

private:
    //==============================================================================
    juce::StringArray files;

    //==============================================================================
    bool isFileValid (const juce::String& file);

    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Modularizer)
};

#endif //JRL_MODULARIZER_H