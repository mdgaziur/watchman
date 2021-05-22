#include <config_handler.hpp>
#include <rapidjson/document.h>
#include <fstream>
#include <sstream>
#include <log/log.hpp>
#include <config.hpp>
#include <watch.hpp>

int handle_config(std::string filename)
{
#ifdef DEBUGMODE
    Log_class logger("debug-log.txt");
#else
    Log_class logger("/tmp/watchman-log.txt");
#endif
    rapidjson::Document document;
    FILE *config_file_fp = fopen(filename.c_str(), "r");
    if(!config_file_fp)
    {
        logger.write(LOG_LEVEL::FATAL, std::string(FAILED_TO_OPEN_CONFIG), true);
    }

    fseek(config_file_fp, 0, SEEK_END);
    size_t config_f_length = ftell(config_file_fp);
    fseek(config_file_fp, 0, SEEK_SET);

    char *buf;
    buf = (char*)malloc(config_f_length + 1);
    if(buf)
    {
        fread(buf, 1, config_f_length, config_file_fp);
    }
    fclose(config_file_fp);
    buf[config_f_length] = '\0';

    document.Parse(buf);

    watchman_config *config = new watchman_config;

    if (document.HasParseError()) {
        logger.write(LOG_LEVEL::FATAL, "Parsing error occured in the config json", true);
    }

    if(document.HasMember("extensions"))
    {
        if(document["extensions"].IsArray()) {
            const rapidjson::Value& extensions_array = document["extensions"];
            for(rapidjson::SizeType i = 0; i < extensions_array.Size(); i++)
            {
                if(!extensions_array[i].IsString())
                    logger.write(LOG_LEVEL::FATAL, "Expected extensions in extensions object to be string", true);
                config->extensions.push_back(extensions_array[i].GetString());
            }
        }
        else {
            logger.write(LOG_LEVEL::FATAL, "'extensions' property should be an array", true);
        }
    }
    
    if(document.HasMember("exclude"))
    {
        if(document["exclude"].IsArray()) {
            const rapidjson::Value& dirs_array = document["exclude"];
            for(rapidjson::SizeType i = 0; i < dirs_array.Size(); i++)
            {
                if(!dirs_array[i].IsString())
                    logger.write(LOG_LEVEL::FATAL, "Expected paths in exclude object to be string", true);
                config->excluded.push_back(dirs_array[i].GetString());
            }
        }
        else {
            logger.write(LOG_LEVEL::FATAL, "'exclude' property should be an array", true);
        }
    }

    if(document.HasMember("command"))
    {
        if (document["command"].IsString())
            config->command = document["command"].GetString();
        else
            logger.write(LOG_LEVEL::FATAL, "'command' property should be a string", true);
    }

    if(document.HasMember("recursive"))
    {   
        if (document["recursive"].IsBool())
           config->recursive = document["recursive"].GetBool();
        else
            logger.write(LOG_LEVEL::FATAL, "'recursive' property should be a bool", true);
    }

    if(document.HasMember("verbose"))
    {   
        if (document["verbose"].IsBool())
           config->verbose = document["verbose"].GetBool();
        else
            logger.write(LOG_LEVEL::FATAL, "'verbose' property should be a bool", true);
    }

    if(document.HasMember("showChangedFileName"))
    {
        if (document["showChangedFileName"].IsBool())
            config->show_changed_file_name = document["showChangedFileName"].GetBool();
        else
            logger.write(LOG_LEVEL::FATAL, "'showChangedFileName' property should be a bool", true);
    }

    if (!(
        config->command.size() > 0
    )) {
        logger.write(LOG_LEVEL::FATAL, std::string("Config file is invalid. Check the docs about how to make config file"), true);
    }

#ifdef DEBUGMODE
    logger.write(LOG_LEVEL::DEBUG, std::string("Config file data: "), true);
    logger.write(LOG_LEVEL::DEBUG, std::string(buf), true);
#endif
    return watch(config);
}