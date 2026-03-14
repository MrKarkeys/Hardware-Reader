#ifndef HARDWARE_READER_HPP
#define HARDWARE_READER_HPP

#include <string>
#include <vector>

namespace hardware_fetch {
class HardwareReader {
  private:
    std::string user_name;
    std::string host_name;
    std::string os_name;
    std::string device_type;
    std::string kernel_type;
    std::string uptime;
    std::string shell_type;
    std::string desktop_env;
    std::string terminal;
    std::string cpu_model;
    std::vector<std::string> gpu_model;
    std::string memory;

    long total_ram;
    long used_ram;

    void get_user_name();
    void get_host_name();
    void get_os_name();
    void get_device_type();
    void get_kernel_type();
    void get_uptime();
    void get_shell_type();
    void get_desktop_env();
    void get_terminal();
    void get_cpu_model();
    void get_gpu_model();
    void get_memory();

  public:
    HardwareReader();

    void display_hardware_info();
};
}  // namespace hardware_fetch

#endif