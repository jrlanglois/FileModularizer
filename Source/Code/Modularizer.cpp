#include "Modularizer.h"

Modularizer::Modularizer (const juce::StringArray& f)
{
    copyAndfilterBadFiles (f);
}

Modularizer::Modularizer (const juce::File& folder,
                          const bool searchRecursively)
{
    if (folder.isDirectory())
    {
        juce::StringArray paths;
        juce::Array<juce::File> f;
        folder.findChildFiles (f, juce::File::findFiles, searchRecursively);

        for (int i = 0; i < f.size(); ++i)
            paths.add (f[i].getFullPathName());

        copyAndfilterBadFiles (paths);
    }
    else
    {
        jassertfalse;
    }
}

Modularizer::~Modularizer()
{
}

//==============================================================================
juce::String Modularizer::getWildcards() const
{
    return "*.h;*.c;*.cpp;*.hpp";
}

bool Modularizer::isFileValid (const juce::String& f)
{
    const juce::String wildcards (getWildcards().removeCharacters ("*"));

    if (f.isNotEmpty())
        if (juce::File (f).existsAsFile())
            return juce::File (f).hasFileExtension (wildcards);

    return false;
}

bool Modularizer::containsOldCStyleCodeFiles() const
{
    for (int i = files.size(); --i >= 0;)
        if (juce::File (files[i]).hasFileExtension (".c"))
            return true;

    return false;
}

//==============================================================================
void Modularizer::copyAndfilterBadFiles (const juce::StringArray& f)
{
    files = f;
    files.removeDuplicates (true);
    files.removeEmptyStrings();

    for (int i = 0; i < files.size(); ++i)
    {
        if (! isFileValid (files[i]))
        {
            files.remove (i);
            --i;
        }
    }

    jassert (files.size() > 0);
}

void Modularizer::saveTo (const juce::File& folder,
                          const juce::String& moduleName,
                          const juce::String& headerGuard)
{
    if (files.size() > 0
        && moduleName.isNotEmpty()
        && headerGuard.isNotEmpty())
    {
        const bool hasOldStyle = containsOldCStyleCodeFiles();
    }
}