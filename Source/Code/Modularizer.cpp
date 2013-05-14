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
        juce::Array<juce::File> f;
        folder.findChildFiles (f, juce::File::findFiles, searchRecursively);

        juce::StringArray paths;

        for (int i = 0; i < f.size(); ++i)
            paths.add (f.getReference (i).getFullPathName());

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
                          const juce::String& headerGuard,
                          const juce::String& desiredNamespace)
{
    if (files.size() > 0
        && sourceFolderToRemove.isNotEmpty()
        && moduleName.isNotEmpty()
        && headerGuard.isNotEmpty())
    {
        const juce::String moduleNameToUse (moduleName.trim());
        const juce::String headerGuardToUse (headerGuard.toUpperCase().trim());
        const juce::String namespaceToUse (desiredNamespace.trim());

        const juce::File moduleHeader (destinationFolder.getFullPathName() + "/" + moduleNameToUse + ".h");
        moduleHeader.deleteFile();
        moduleHeader.create();

        const bool endsWithSlash = sourceFolderToRemove.endsWith ("/") || sourceFolderToRemove.endsWith ("\\");
        const int numPathCharsToRemove = sourceFolderToRemove.length() - (endsWithSlash ? 1 : 0);

        const int numSpaces = desiredNamespace.isEmpty() ? 0 : 4;
        juce::String spacer (juce::String::empty);

        for (int i = numSpaces; --i >= 0;)
            spacer += " ";

        { //Write the header file contents:
            juce::String data (juce::String::empty);

            data << "#ifndef " << headerGuardToUse << juce::newLine;
            data << "#define " << headerGuardToUse << juce::newLine;
            data << juce::newLine;
            data << juce::newLine;
            data << juce::newLine;

            if (namespaceToUse.isNotEmpty())
            {
                data << "namespace " << namespaceToUse << juce::newLine;
                data << "{" << juce::newLine;
            }

            for (int i = 0; i < files.size(); ++i)
            {
                const juce::File file (files[i]);

                if (file.hasFileExtension (".h"))
                {
                    juce::ScopedPointer<juce::FileInputStream> guardFinder (file.createInputStream());
                    jassert (guardFinder != nullptr);

                    juce::String code = guardFinder->readString();
                    const int startIndex = code.indexOf ("#ifndef ") + 8;
                    const int endIndex = startIndex + code.substring (startIndex).indexOf ("\n");

                    const juce::String guard = code.substring (startIndex, endIndex)
                                                   .trim()
                                                   .removeCharacters (juce::newLine)
                                                   .removeCharacters ("\n")
                                                   .removeCharacters ("\r");

                    juce::String fileShort = file.getFullPathName()
                                                 .replaceCharacters ("\\", "/")
                                                 .replaceSection (0, numPathCharsToRemove, juce::String::empty);

                    if (fileShort.startsWith ("/"))
                        fileShort = fileShort.substring (1);

                    data << spacer << "#ifndef " << guard << juce::newLine;
                    data << spacer <<"    #include \"" << fileShort << juce::newLine;
                    data << spacer <<"#endif " << guard << juce::newLine;

                    if (i != (files.size() - 1))
                        data << juce::newLine;
                }
            }

            if (namespaceToUse.isNotEmpty())
                data << "}" << juce::newLine;

            data << juce::newLine;
            data << "#endif //" << headerGuardToUse;

            juce::ScopedPointer<juce::FileOutputStream> stream (moduleHeader.createOutputStream());
            stream->writeText (data, false, false);
        }

        const juce::File moduleCPP (destinationFolder.getFullPathName() + "/" + moduleNameToUse + ".cpp");
        moduleCPP.deleteFile();
        moduleCPP.create();

        { //Write the CPP file contents:
            juce::String data (juce::String::empty);

            data << "#include \"" << moduleNameToUse << ".h\"" << juce::newLine;
            data << juce::newLine;

            if (namespaceToUse.isNotEmpty())
            {
                data << "namespace " << namespaceToUse << juce::newLine;
                data << "{" << juce::newLine;
            }

            for (int i = 0; i < files.size(); ++i)
            {
                const juce::File file (files[i]);

                if (file.hasFileExtension (".c;.cpp;.hpp"))
                {
                    juce::String fileShort = file.getFullPathName()
                                                 .replaceCharacters ("\\", "/")
                                                 .replaceSection (0, numPathCharsToRemove, juce::String::empty);

                    if (fileShort.startsWith ("/"))
                        fileShort = fileShort.substring (1);

                    data << spacer << "#include \"" << fileShort << "\"";

                    if (i != (files.size() - 1))
                        data << juce::newLine;
                }
            }

            if (namespaceToUse.isNotEmpty())
                data << "}";

            juce::ScopedPointer<juce::FileOutputStream> stream (moduleCPP.createOutputStream());
            stream->writeText (data, false, false);
        }
    }
}