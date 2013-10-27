/**********************************************************\ 
 
 Auto-generated Factory.cpp
 
 This file contains the auto-generated factory methods 
 for the WebrtcEngine project
 
\**********************************************************/

#include "FactoryBase.h"
#include "WebrtcEngine.h"
#include "WebrtcRenderer.h"
#include "SystemHelpers.h"
#include <boost/make_shared.hpp>
#include <boost/filesystem.hpp>

class PluginFactory : public FB::FactoryBase
{
public:
    ///////////////////////////////////////////////////////////////////////////////
    /// @fn FB::PluginCorePtr createPlugin(const std::string& mimetype)
    ///
    /// @brief  Creates a plugin object matching the provided mimetype
    ///         If mimetype is empty, returns the default plugin
    ///////////////////////////////////////////////////////////////////////////////
    FB::PluginCorePtr createPlugin(const std::string& mimetype)
    {
        if (!mimetype.compare("application/x-webrtcrenderer")) {
            return boost::make_shared<WebrtcRenderer>(mimetype);
        }else {
            return boost::make_shared<WebrtcEngine>(mimetype);
        }
    }
    
    ///////////////////////////////////////////////////////////////////////////////
    /// @see FB::FactoryBase::globalPluginInitialize
    ///////////////////////////////////////////////////////////////////////////////
    void globalPluginInitialize()
    {
        WebrtcEngine::StaticInitialize();
    }
    
    ///////////////////////////////////////////////////////////////////////////////
    /// @see FB::FactoryBase::globalPluginDeinitialize
    ///////////////////////////////////////////////////////////////////////////////
    void globalPluginDeinitialize()
    {
        WebrtcEngine::StaticDeinitialize();
    }
	
    ///////////////////////////////////////////////////////////////////////////////
    /// @fn void getLoggingMethods(FB::Log::LogMethodList& outMethods)
    ///
    /// @brief Configures plugin logging functionality
    ///
    ///////////////////////////////////////////////////////////////////////////////    
    void getLoggingMethods(FB::Log::LogMethodList& outMethods)
    {
        return;

        boost::filesystem::path appDataPath = FB::System::getLocalAppDataPath("WebrtcEngine");
        boost::filesystem::path logDirPath = appDataPath / "logs";
        
        if(false == boost::filesystem::exists(logDirPath))
        {
            boost::filesystem::create_directories(logDirPath);
        }
        
        if(true == boost::filesystem::exists(logDirPath) && 
           true == boost::filesystem::is_directory(logDirPath))
        {
            time_t timeInSeconds = time(NULL);
            boost::thread::id threadId = boost::this_thread::get_id();
            boost::filesystem::path logFilePath;
            std::stringstream sstrm;
            
            sstrm << "WebrtcEngine_" << threadId << "_" << timeInSeconds << ".log";
            logFilePath = logDirPath / sstrm.str();
            outMethods.push_back(std::make_pair(FB::Log::LogMethod_File, logFilePath.string()));
        }
        
        outMethods.push_back(std::make_pair(FB::Log::LogMethod_Console, std::string()));        
    }
    
    ///////////////////////////////////////////////////////////////////////////////
    /// @fn FB::LogLogLevel getLogLevel()
    ///
    /// @brief Gets log level for the plugin
    ///
    ///////////////////////////////////////////////////////////////////////////////    
    FB::Log::LogLevel getLogLevel()
    {
        return FB::Log::LogLevel_Debug;
    }	
};

///////////////////////////////////////////////////////////////////////////////
/// @fn getFactoryInstance()
///
/// @brief  Returns the factory instance for this plugin module
///////////////////////////////////////////////////////////////////////////////
FB::FactoryBasePtr getFactoryInstance()
{
    static boost::shared_ptr<PluginFactory> factory = boost::make_shared<PluginFactory>();
    return factory;
}

