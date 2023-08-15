#pragma once
#include "mat.h"
#include <string>


bool load(const std::string& filename, mat<uint16_t>& img, uint16_t& maxvalue);

