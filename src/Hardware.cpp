#include "Hardware.hpp"

#include <limits.h>       // Required for HOST_NAME_MAX
#include <pwd.h>          // define passwd struct
#include <sys/sysinfo.h>  // For uptime
#include <sys/types.h>    // prereq for pwd.h
#include <sys/utsname.h>  // For for uname()
#include <unistd.h>  // For gethostname() and getlogin(), Access to POSIX OS API functions

#include <cstdint>
#include <cstdio>  //popen and pclose
#include <cstdlib>
#include <format>
#include <fstream>  // For reading files like /proc/cpuinfo
#include <iostream>
#include <memory>  // unique_pointer
#include <string>
#include <vector>

namespace hardware_fetch {
HardwareReader::HardwareReader() {
    get_user_name();
    get_host_name();
    get_os_name();
    get_device_type();
    get_kernel_type();
    get_uptime();
    get_shell_type();
    get_desktop_env();
    get_terminal();
    get_cpu_model();
    get_gpu_model();
    get_memory();
}

// Display: Prints the gathered data
void HardwareReader::display_hardware_info() {
    std::cout << "--- Hardware Info ---" << std::endl;
    std::cout << "User:       " << user_name << std::endl;
    std::cout << "Host:       " << host_name << std::endl;
    std::cout << "OS:         " << os_name << std::endl;
    std::cout << "Device:     " << device_type << std::endl;
    std::cout << "Kernel:     " << kernel_type << std::endl;
    std::cout << "Uptime:     " << uptime << std::endl;
    std::cout << "Shell       " << shell_type << std::endl;
    std::cout << "DE:         " << desktop_env << std::endl;
    std::cout << "Terminal    " << terminal << std::endl;
    std::cout << "CPU:        " << cpu_model << std::endl;

    uint8_t gpu_count = 0;
    for (const auto& gpu : gpu_model) {
        std::cout << std::format("GPU {}:      ", gpu_count);
        std::cout << gpu << std::endl;
        gpu_count++;
    }

    std::cout << "Memory:     " << memory << std::endl;

    std::cout << std::endl;
}

void HardwareReader::get_user_name() {
    struct passwd* pw = getpwuid(getuid());
    if (pw) {
        user_name = std::string(pw->pw_name);
    } else {
        user_name = "unknown";
    }
}

void HardwareReader::get_host_name() {
    char hostname[HOST_NAME_MAX];
    if (gethostname(hostname, HOST_NAME_MAX) == 0) {
        host_name = std::string(hostname);
    } else {
        host_name = "unknown";
    }
}

void HardwareReader::get_os_name() {
    std::ifstream file("/etc/os-release");
    os_name = "unknown";
    std::string line;

    if (file.is_open()) {
        while (std::getline(file, line)) {
            std::string name_find = "PRETTY_NAME=";
            if (line.find(name_find) == 0) {
                size_t start = line.find('"') + 1;
                size_t end = line.find_last_of('"');
                os_name = line.substr(start, end - start);
            }
        }
    }
}

void HardwareReader::get_device_type() {
    std::string vendor = "";
    std::string prod_name = "";
    device_type = "unknown";

    std::ifstream vendor_file("/sys/class/dmi/id/sys_vendor");
    if (vendor_file.is_open()) {
        std::getline(vendor_file, vendor);
    }

    std::ifstream prod_name_file("/sys/class/dmi/id/product_name");
    if (prod_name_file.is_open()) {
        std::getline(prod_name_file, prod_name);
    }

    device_type = vendor + " " + prod_name;

    if (device_type.find("Inc.") != std::string::npos) {
        device_type.erase(device_type.find("Inc."), 5);
    }
}

void HardwareReader::get_kernel_type() {
    struct utsname info_buffer;
    kernel_type = "unknown";

    if (uname(&info_buffer) == 0) {
        kernel_type = std::string(info_buffer.release) + " " +
                      std::string(info_buffer.version);
    }
}

void HardwareReader::get_uptime() {
    struct sysinfo info;
    if (sysinfo(&info) == 0) {
        long seconds = info.uptime;

        int days = seconds / 86400;
        int hours = (seconds % 86400) / 3600;
        int minutes = (seconds % 3600) / 60;
        seconds = seconds % 60;

        uptime = std::to_string(days) + " days " + std::to_string(hours) +
                 " hours " + std::to_string(minutes) + " mins " +
                 std::to_string(seconds) + " seconds ";
    } else {
        uptime = "unknown";
    }
}

void HardwareReader::get_shell_type() {
    const char* shell_path = std::getenv("SHELL");

    if (shell_path != nullptr) {
        std::string full_path(shell_path);
        size_t last_slash = full_path.find_last_of('/');
        if (last_slash != std::string::npos) {
            shell_type = full_path.substr(last_slash + 1);
        } else {
            shell_type = full_path;
        }
    } else {
        shell_type = "Unknown";
    }
}

void HardwareReader::get_desktop_env() {
    const char* de = std::getenv("XDG_CURRENT_DESKTOP");

    if (de != nullptr) {
        desktop_env = std::string(de);
        size_t last_colon = desktop_env.find_last_of(':');
        if (last_colon != std::string::npos) {
            desktop_env = desktop_env.substr(last_colon + 1);
        }
    } else {
        desktop_env = "unknown";
    }
}

void HardwareReader::get_terminal() {
    const char* term = std::getenv("TERM_PROGRAM");
    if (term != nullptr) {
        terminal = term;
    } else {
        terminal = "unknown";
    }
}

void HardwareReader::get_cpu_model() {
    std::ifstream cpu_file("/proc/cpuinfo");
    std::string line;

    if (cpu_file.is_open()) {
        while (std::getline(cpu_file, line)) {
            if (line.find("model name") != std::string::npos) {
                size_t colon_pos = line.find(':');
                if (colon_pos != std::string::npos) {
                    cpu_model = line.substr(colon_pos + 2);
                    break;
                }
            }
        }
    } else {
        cpu_model = "unknown";
    }
}

// credits to https://github.com/dylanaraps/neofetch/blob/master/neofetch
void HardwareReader::get_gpu_model() {
    const std::string command =
        R"(lspci -mm | awk -F '"|" "|\\(' '/"Display|"3D|"VGA/ { a[$0] = $1 " " $3 " " ($(NF-1) ~ /^$|^Device [[:xdigit:]]+$/ ? $4 : $(NF-1)) } END { for (i in a) { if (!seen[a[i]]++) { sub("^[^ ]+ ", "", a[i]); print a[i] }}}')";

    std::unique_ptr<FILE, decltype(&pclose)> pipe(
        popen(command.c_str(), "r"),
        pclose);  // smart pointer use pclose to close properly

    if (!pipe) {
        gpu_model = {"error"};
        return;
    }

    char buffer[256];
    gpu_model.clear();

    while (fgets(buffer, sizeof(buffer), pipe.get()) != nullptr) {
        std::string line = buffer;
        line.erase(line.find_last_not_of("\n") + 1);
        gpu_model.push_back(line.substr(1));
    }

    if (gpu_model.empty()) {
        gpu_model = {"unknown"};
    }
}

void HardwareReader::get_memory() {
    struct sysinfo info;

    if (sysinfo(&info) == 0) {
        unsigned long total_mib = (info.totalram * info.mem_unit) / 1024 / 1024;
        memory = std::to_string(total_mib) + " MiB";
    } else {
        memory = "unknown";
    }
}

}  // namespace hardware_fetch
