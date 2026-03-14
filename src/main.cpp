#include "Hardware.hpp"

int main() {
    hardware_fetch::HardwareReader reader;
    reader.display_hardware_info();
    return 0;
}