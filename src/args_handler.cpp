#include <args_handler.hpp>
#include <args/args.hxx>
#include <config_handler.hpp>

int handle_args(int argc, const char* argv[])
{
    args::ArgumentParser            parser("Watchman", "A developer friendly program that runs command on fs event");
    args::HelpFlag                  help(parser, "help", "Displays this help menu", { "h", "help" });
    args::ValueFlag<std::string>    config_file(parser, "config", "Specify the config file. Default is watchman.config.json", { 'c', "config" });
    args::Flag                      ignore_log(parser, "ilog", "Ignore if the creation of log file fails", { 'i', "ilog" });

    try
    {
        parser.ParseCLI(argc, argv);
    }
    catch(args::Help&)
    {
        std::cout<<parser;
        return 0;
    }
    catch(args::ParseError &e)
    {
        std::cerr<<e.what()<<std::endl;
        std::cerr<<parser<<std::endl;
        return 1;
    }
    catch(args::ValidationError &e)
    {
        std::cerr<<e.what()<<std::endl;
        std::cerr<<parser<<std::endl;
        return 1;
    }

    if(args::get(config_file) != "")
    {
        return handle_config(args::get(config_file));
    }
    return handle_config("watchman.config.json");
}