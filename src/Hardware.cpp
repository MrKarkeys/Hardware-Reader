#include "Hardware.hpp"

#include <limits.h>       // Required for HOST_NAME_MAX
#include <pwd.h>          // define passwd struct
#include <sys/sysinfo.h>  // For uptime
#include <sys/types.h>    // prereq for pwd.h
#include <sys/utsname.h>  // For for uname()
#include <unistd.h>  // For gethostname() and getlogin(), Access to POSIX OS API functions

#include <cstdint>
#include <format>
#include <fstream>  // For reading files like /proc/cpuinfo
#include <iostream>
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
    get_screen_res();
    get_desktop_env();
    get_window_manager();
    get_window_manager_theme();
    get_terminal();
    get_cpu_model();
    get_gpu_model();
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
    std::cout << "Screen      " << screen_res << std::endl;
    std::cout << "DE:         " << desktop_env << std::endl;
    std::cout << "WM:         " << window_manager << std::endl;
    std::cout << "WM Theme    " << window_manager_theme << std::endl;
    std::cout << "Terminal    " << terminal << std::endl;
    std::cout << "CPU:        " << cpu_model << std::endl;

    uint8_t gpu_count = 0;
    for (const auto& gpu : gpu_model) {
        std::cout << std::format("GPU {}:      ", gpu_count);
        std::cout << gpu << std::endl;
        gpu_count++;
    }
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

void HardwareReader::get_shell_type() { shell_type = "unknown"; }

void HardwareReader::get_screen_res() { screen_res = "unknown"; }

void HardwareReader::get_desktop_env() { desktop_env = "unknown"; }

void HardwareReader::get_window_manager() { window_manager = "unknown"; }

void HardwareReader::get_window_manager_theme() {
    window_manager_theme = "unknown";
}

void HardwareReader::get_terminal() { terminal = "unknown"; }

void HardwareReader::get_cpu_model() { cpu_model = "unknown"; }

void HardwareReader::get_gpu_model() { gpu_model = {"unknown"}; }

}  // namespace hardware_fetch
