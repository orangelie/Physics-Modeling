/*
* 
* < cppStdUtil.h >
* Github: https://github.com/orangelie
* // Copyright (C) 2022 by orangelie, Inc. All right reserved.
* // MIT License
* 
*/





#pragma once

#include <iostream>
#include <fstream>

#include <exception>
#include <memory>

#include <string>
#include <vector>
#include <tuple>
#include <unordered_map>
#include <array>

#define THROW(e) { \
	std::string __err_message = std::string(__FILE__) + ": " + e; \
	throw std::runtime_error(__err_message); \
} \

#define HR(e) { \
	if(FAILED(e)) { \
		std::string __err_message = std::string(__FILE__) + std::to_string(__LINE__) + std::string(__FUNCTION__); \
		throw std::runtime_error(__err_message); \
	} \
} \


namespace orangelie {
	namespace CppStdUtil {
		template <class _Tp>
		_Tp& unmove(_Tp&& ____TpTp__) {
			return ____TpTp__;
		}
	}
}