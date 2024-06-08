#include <iostream>
#include <vector>
#include <string>
#include "mega_io.h"

int show_help(const char *argv0)
{
    std::cout<<"Usage: \n"
                <<"  "<<argv0<<" spinup disk_id disk_id ...\n"
                <<"  "<<argv0<<" spindown disk_id disk_id ...\n"
                <<"Exmaple: "<<argv0<<" spindown 0\n";
    return 1;
}

int main(int argc, char **argv)
{
    if (argc < 3)
        return show_help(argv[0]);
    
    std::string op{argv[1]};
    if (op != "spinup" && op != "spindown")
        return show_help(argv[0]);

    std::vector<unsigned int> disk_list;
    for (int j=2; j<argc; j++)
        disk_list.push_back(std::stoul(argv[j]));
    
    MegaIO cli;
    if (!cli.open())
        return 2;
    for (size_t i=0; i < disk_list.size(); i++) {
        if (op == "spinup" && !cli.disk_spinup(disk_list[i]))
            continue;
        if (op == "spindown" && !cli.disk_spindown(disk_list[i]))
            continue;
    }
    cli.close();
    return 0;
}
