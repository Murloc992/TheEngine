#ifndef _Precomp_H_
#define _Precomp_H_

#ifdef _CRT_SECURE_NO_WARNINGS
#undef _CRT_SECURE_NO_WARNINGS
#endif

#include "Config.h"

// C/C++ SUITE INCLUDES
//#include <stdio.h>
#include <iostream>
#include <cstdio>
#include <stdint.h>
#include <map>
#include <unordered_map>
#include <vector>
#include <memory>
#include <thread>
#include <atomic>
#include <condition_variable>
#include <mutex>
#include <random>
#include <filesystem>

#include <string.h>
#include <string>
#include <stdarg.h>

// OPENGL RELATED LIBS
#include "glad/glad.h"
#include "GLFW/glfw3.h"

#include "glm.hpp"
#include "gtx/norm.hpp"
#include "gtc/matrix_transform.hpp"
#include "gtc/matrix_inverse.hpp"
#include "gtc/matrix_access.hpp"
#include "gtx/rotate_vector.hpp"
#include "gtx/matrix_decompose.hpp"

#include "gtc/type_ptr.hpp"

// OTHER LIBRARIES
#include "physfs.h"
#include "tinyxml2.h"
#include "sigc++/sigc++.h"

// PROJECT INCLUDES
#include "utility/Bit.h"
#include "utility/Helpers.h"

// BOOST INCLUDES
// jk, don't include boost here. pch size too big.

#endif
