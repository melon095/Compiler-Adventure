#pragma once

#include <cstdlib>
#include <iostream>

#define TRY(expr)                                                                                                      \
	({                                                                                                                 \
		auto&& ___temp___ = (expr);                                                                                    \
		if(!___temp___.ok()) [[unlikely]]                                                                              \
			return ___temp___;                                                                                         \
	})

#define TRY_RET(expr)                                                                                                  \
	({                                                                                                                 \
		auto&& ___temp___ = (expr);                                                                                    \
		if(!___temp___.ok()) [[unlikely]]                                                                              \
			return ___temp___.status();                                                                                \
		*___temp___;                                                                                                   \
	})

#define MUST(expr)                                                                                                     \
	if(auto status = (expr); !status.ok())                                                                             \
	{                                                                                                                  \
		std::cerr << "\033[1;31m[MUST FAILED] " << status.message() << "\033[0m" << std::endl;                         \
                                                                                                                       \
		std::exit(1);                                                                                                  \
	}

#define NOT_REACHABLE()                                                                                                \
	std::cerr << "\033[1;31m[NOT REACHABLE] " << __FILE__ << ":" << __LINE__ << "\033[0m" << std::endl;
