#ifndef _Precomp_H_
#define _Precomp_H_

#ifdef _CRT_SECURE_NO_WARNINGS
  #undef _CRT_SECURE_NO_WARNINGS
#endif

#include "Config.h"

// C/C++ SUITE INCLUDES
// #include <stdio.h>
#include <stdint.h>
#include <atomic>
#include <condition_variable>
#include <cstdio>
#include <filesystem>
#include <iostream>
#include <map>
#include <memory>
#include <mutex>
#include <random>
#include <thread>
#include <unordered_map>
#include <vector>

#include <stdarg.h>
#include <string.h>
#include <string>

// OPENGL RELATED LIBS
#include "glad/glad.h"

#include "GLFW/glfw3.h"

#include "glm.hpp"
#include "gtc/matrix_access.hpp"
#include "gtc/matrix_inverse.hpp"
#include "gtc/matrix_transform.hpp"
#include "gtx/matrix_decompose.hpp"
#include "gtx/norm.hpp"
#include "gtx/rotate_vector.hpp"

#include "gtc/type_ptr.hpp"

// OTHER LIBRARIES
#include "physfs.h"
#include "sigc++/sigc++.h"
#include "tinyxml2.h"

// PROJECT INCLUDES
#include "utility/Bit.h"
#include "utility/Helpers.h"

// BOOST INCLUDES
// jk, don't include boost here. pch size too big.

#endif
