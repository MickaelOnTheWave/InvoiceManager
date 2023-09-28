#ifndef COMMANDLINEMANAGER_H
#define COMMANDLINEMANAGER_H

#include <list>
#include <map>
#include <string>

/// TODO Manage short commands, unix-style
class CommandLineManager
{
public:
    CommandLineManager(int argc, char* argv[]);
    CommandLineManager(const std::map<std::string, std::string>& parameters);

    // TODO Put a "mandatory" value for each param
    // TODO Put a field just for param value.
    void AddParameter(const std::string& parameter, const std::string& description);

    bool HasParameter(const std::string& parameter) const;

    std::string GetParameterValue(const std::string& parameter) const;
    unsigned long GetParameterValueAsUnsignedLong(const std::string& parameter);

    bool HasUnknownParameters(void) const;

    /**
     * @brief Shows command usage information if unknown parameters are found.
     * @return false if all parameters are known.
     */
    bool HandleUnknownParameters();

    /**
     * @brief Shows version and copyright information on version command.
     * @return false if command was not version.
     */
    bool HandleVersionCommand();

    /**
     * @brief Shows command usage information on help command.
     * @return false if command was not help.
     */
    bool HandleHelpCommand();

    void ShowUsageInformation();

    void EnableHelpCommand();

    void EnableVersionCommand(const std::string& appName, const std::string& appVersion,
                              const std::string& author, const std::string& copyright);


private:
    typedef std::list<std::pair<std::string, std::string> > ParameterList;

    void BuildParametersMap(const int argc, char* argv[]);

    void BuildParametersMap(const int argc, wchar_t* argv[]);

    bool IsInKnownList(const std::string& parameter) const;

    void ShowVersionInformation();

    void UpdateParameterColumnSize(const ParameterList& parameters);

    void ShowParamUsage(const std::string& param, const std::string& description);

    static std::string Spaces(const int spaceCount);

    std::map<std::string, std::string> options;
    ParameterList knownParameters;
    bool usingHelpCommand;
    bool usingVersionCommand;
    std::string applicationName;
    std::string applicationVersion;
    std::string authorName;
    std::string copyrightInfo;
    size_t parameterColumnSize;
};

#endif // COMMANDLINEMANAGER_H
