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
            juce::String data;

            data << "#ifndef " << headerGuard << "\r\n";
            data << "#define " << headerGuard << "\r\n";
            data << "\r\n";
            data << "\r\n";
            data << "\r\n";

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

                    juce::String guard = code.substring (startIndex, endIndex)
                                             .trim()
                                             .removeCharacters ("\r\n")
                                             .removeCharacters ("\n")
                                             .removeCharacters ("\r");

                    juce::String fileShort = file.getFullPathName()
                                                 .replaceCharacters ("\\", "/")
                                                 .replaceSection (0, sourceFolderToRemove.length(), juce::String::empty);

                    data << "#ifndef " << guard << "\r\n";
                    data << "    #include \"" << fileShort << "\"\r\n";
                    data << "#endif " << guard << "\r\n";
                    data << "\r\n";
                }
            }

            data << "#endif //" << headerGuard;

            juce::ScopedPointer<juce::FileOutputStream> stream = moduleHeader.createOutputStream();
            stream->writeText (data, false, false);
        }

        juce::File moduleCPP (destinationFolder.getFullPathName() + "/" + moduleName + ".cpp");
        moduleCPP.deleteFile();
        moduleCPP.create();

        { //Write the CPP file contents:
            juce::String data;

            data << "#include \"" << moduleName << ".h\"" << "\r\n";
            data << "\r\n";

            for (int i = 0; i < files.size(); ++i)
            {
                juce::File file (files[i]);

                if (file.hasFileExtension (".c;.cpp;.hpp"))
                {
                    juce::String fileShort = file.getFullPathName()
                                                 .replaceCharacters ("\\", "/")
                                                 .replaceSection (0, sourceFolderToRemove.length(), juce::String::empty);

                    data << "#include \"" << fileShort << "\"";
                    data << "\r\n";
                }
            }

            juce::ScopedPointer<juce::FileOutputStream> stream = moduleCPP.createOutputStream();
            stream->writeText (data, false, false);
        }
    }
}