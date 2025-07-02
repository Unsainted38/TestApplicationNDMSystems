#include <iostream>
#include <cstdlib>
#include <cstdio>
#include <string>
#include <regex>

std::string exec_command(const std::string &cmd) {
    std::string result;
    char buffer[128];
    FILE *pipe = popen(cmd.c_str(), "r");

    if(!pipe) {
        return "";
    }

    while(fgets(buffer, sizeof(buffer), pipe) != nullptr) {
        result += buffer;
    }

    pclose(pipe);
    return result;
}

std::string get_mac_from_arp(const std::string &ip) {
    std::string arp_output = exec_command("arp -n " + ip);
    std::regex mac_regex("(([0-9a-fA-F]{2}[:-]){5}[0-9a-fA-F]{2})");
    std::smatch match;

    if(std::regex_search(arp_output, match, mac_regex)) {
        return match.str(0);
    }

    return "MAC not found";
}

int main() {


    while(true) {
        std::cout << "Enter IP address (0 for exit:)" << std::endl;
        std::string ip;
        std::cin >> ip;

        if(ip == "0") {
            return 0;
        }

        std::cout << "Pinging " << ip << "..." << std::endl;

        std::string ping_cmd = "ping -c 1 -W 1 " + ip + " > /dev/null 2>&1";
        int ping_result = system(ping_cmd.c_str());

        if(ping_result != 0) {
            std::cerr << "Ping failed or host unreachable." << std::endl;
            continue;
        }

        std::string mac = get_mac_from_arp(ip);

        std::cout << "MAC address: " << mac << std::endl;
    }

    return 0;
}
