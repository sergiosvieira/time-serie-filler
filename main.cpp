#include <iostream>
#include "cxxopts.hpp"
#include "date.h"
#include "fmt/include/fmt/format.h"

using std::cout, std::endl;

int main(int argc, char** argv)
{
    cxxopts::Options options("time-serie-filler",
                             "Time serie tool");
    options.add_options()
        ("s,startdate", "Start date", cxxopts::value<std::string>())
        ("e,enddate", "End date", cxxopts::value<std::string>())
        ("d,days", "Increment days", cxxopts::value<std::string>())
        ("c,const", "Fills time serie with constant value", cxxopts::value<std::string>())
    ;
    auto result = options.parse(argc, argv);
    if (result.arguments().size() <= 1)
    {
        cout << options.help();
        return 0;
    }
    uint days = 0;
    Date start = from_str(result["s"].as<std::string>());
    if (result.count("e") == 0)
    {
        if (result.count("d") == 0)
        {
            cout << options.help();
            return 0;
        }
        days = static_cast<uint>(std::atoi(result["d"].as<std::string>().c_str()));
    }
    else
    {
        Date end = from_str(result["e"].as<std::string>());
        days = diff_date(start, end);
    }
    std::string value = "0.0";
    if (result.count("c"))
        value = result["c"].as<std::string>();
    for (uint i = 0; i < days; ++i)
    {
       cout << fmt::format("{} {}", start, value) << endl;
       start = increment_day(start);
   }
    return 0;
}
