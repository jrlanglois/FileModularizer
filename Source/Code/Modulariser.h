#ifndef JRL_MODULARISER_H
#define JRL_MODULARISER_H

#include "JuceHeader.h"

class Modulariser
{
public:
    Modulariser (const juce::StringArray& files);

    Modulariser (const juce::File& folder,
                 bool searchRecursively);

    ~Modulariser();

    //==============================================================================
    void copyAndfilterBadFiles (const juce::StringArray& files);

    void saveTo (const juce::File& destinationFolder,
                 const juce::String& sourceFolderToRemove,
                 const juce::String& moduleName,
                 const juce::String& headerGuard,
                 const juce::String& desiredNamespace);

    bool containsOldCStyleCodeFiles() const;

    //==============================================================================
    inline const juce::StringArray& getFiles() const noexcept { return files; }

    juce::String getHeaderWildcards() const noexcept;
    juce::String getCPPWildcards() const noexcept;
    juce::String getWildcards() const noexcept;

private:
    //==============================================================================
    juce::StringArray files;

    //==============================================================================
    bool isFileValid (const juce::String& file) const;

    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Modulariser)
};

#endif //JRL_MODULARISER_H