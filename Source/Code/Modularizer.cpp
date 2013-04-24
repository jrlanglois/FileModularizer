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

void Modularizer::saveTo (const juce::File& destinationFolder,
                          const juce::String& sourceFolderToRemove,
                          const juce::String& moduleName,
                          const juce::String& headerGuard)
{
    if (files.size() > 0
        && sourceFolderToRemove.isNotEmpty()
        && moduleName.isNotEmpty()
        && headerGuard.isNotEmpty())
    {
        juce::File moduleHeader (destinationFolder.getFullPathName() + "/" + moduleName + ".h");
        moduleHeader.deleteFile();
        moduleHeader.create();

        { //Write the header file contents:
            juce::ScopedPointer<juce::FileOutputStream> stream = moduleHeader.createOutputStream();

            stream->writeString ("#ifndef " + headerGuard + "\r\n");
            stream->writeString ("#define " + headerGuard + "\r\n");
            stream->writeString ("\r\n");
            stream->writeString ("\r\n");
            stream->writeString ("\r\n");

            for (int i = 0; i < files.size(); ++i)
            {
                juce::File file (files[i]);

                if (file.hasFileExtension (".h"))
                {
                    juce::ScopedPointer<juce::FileInputStream> guardFinder = file.createInputStream();
                    jassert (guardFinder != nullptr);

                    juce::String code = guardFinder->readString();
                    const int startIndex = code.indexOf ("#ifndef ") + 8;
                    const int endIndex = startIndex + code.substring (startIndex).indexOf ("\n");

                    juce::String guard (code.substring (startIndex, endIndex));
                    guard.trim();
                    guard.removeCharacters ("\n");
                    guard.removeCharacters ("\r");

                    juce::String fileShort = file.getFullPathName()
                                             .replaceCharacters ("\\", "/")
                                             .replaceSection (0, sourceFolderToRemove.length(), juce::String::empty);

                    stream->writeString ("#ifndef " + guard + "\r\n");
                    stream->writeString ("    #include " + fileShort + "\r\n");
                    stream->writeString ("#endif " + guard + "\r\n");
                    stream->writeString ("\r\n");
                }
            }

            stream->writeString ("#endif //" + headerGuard);
        }

        juce::File moduleCPP (destinationFolder.getFullPathName() + "/" + moduleName + ".cpp");
        moduleCPP.deleteFile();
        moduleCPP.create();

        { //Write the CPP file contents:
            juce::ScopedPointer<juce::FileOutputStream> stream = moduleCPP.createOutputStream();

            stream->writeString ("#include \"" + moduleName + ".h\"" + "\r\n");
        }
    }
}