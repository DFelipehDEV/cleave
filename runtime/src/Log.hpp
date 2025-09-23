#pragma once
#include <iostream>

#define LOG_DEBUG(msg) std::cout << "[DEBUG] " << msg << std::endl
#define LOG_INFO(msg)  std::cout << "[INFO] " << msg << std::endl
#define LOG_WARN(msg) std::cerr << "[WARNING] " << msg << std::endl
#define LOG_ERROR(msg) std::cerr << "[ERROR] " << msg << std::endl