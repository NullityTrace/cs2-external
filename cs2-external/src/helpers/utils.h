#pragma once
#include <windows.h>
#include <string>

#include "vector.h"

class Utils {
private:
public:
	bool is_in_bounds(const Vector3& pos, int width, int heigh);
};

inline Utils utils;