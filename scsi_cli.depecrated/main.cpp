#include <iostream>
#include <vector>
#include <string>
#include "mega_io.h"

int show_help(const char *argv0)
{
    std::cout<<"Usage: \n"
                <<"  "<<argv0<<" spinup [--hba=0] disk_id disk_id ...\n"
                <<"  "<<argv0<<" spindown [--hba=0] disk_id disk_id ...\n"
                <<"Exmaple: "<<argv0<<" spindown 0\n"
                <<"About --hba: \n"
                  "using 'dmesg|grep scsi' to check hba no if more than one scsi device on system.\n"
                  "  [    2.033346] scsi host0: ahci\n"
                  "  [   11.017557] scsi host6: Avago SAS based MegaRAID driver\n"
                  "In this case we use --hba=6";
    return 1;
}

int main(int argc, char **argv)
{
    if (argc < 3)
        return show_help(argv[0]);
    
    std::string op{argv[1]};
    if (op != "spinup" && op != "spindown")
        return show_help(argv[0]);

    unsigned int m_hba = 0;
    std::vector<unsigned int> disk_list;
    for (int j=2; j<argc; j++){
        std::string ss{argv[j]};
        std::string ss_hba = "--hba=";
        if (ss.size() > ss_hba.size() && ss.substr(0, ss_hba.size()) == ss_hba)
            m_hba = std::stoul(ss.substr(ss_hba.size()));
        else
            disk_list.push_back(std::stoul(argv[j]));
    }
    
    MegaIO cli;
    if (!cli.open())
        return 2;
    for (size_t i=0; i < disk_list.size(); i++) {
        if (op == "spinup" && !cli.disk_spinup(m_hba, disk_list[i]))
            continue;
        if (op == "spindown" && !cli.disk_spindown(m_hba, disk_list[i]))
            continue;
    }
    cli.close();
    return 0;
}
