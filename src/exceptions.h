/**
 *  exceptions.h
 *  StatusSpec project
 *
 *  Copyright (c) 2014 thesupremecommander
 *  BSD 2-Clause License
 *  http://opensource.org/licenses/BSD-2-Clause
 *
 */

#pragma once

#include "stdafx.h"

#include <sstream>
#include <string>

#ifdef _WIN32
#define noexcept _NOEXCEPT
#endif

class module_not_loaded : public std::exception {
public:
	module_not_loaded(const char *name) noexcept;
	virtual const char *what() const noexcept;
private:
	const char *moduleName;
};

inline module_not_loaded::module_not_loaded(const char *name) noexcept{
	moduleName = name;
}

inline const char *module_not_loaded::what() const noexcept{
	std::string s;
	std::stringstream ss;

	ss << "Module " << moduleName << " not loaded!\n";
	ss >> s;

	return s.c_str();
}

class bad_pointer : public std::exception {
public:
	bad_pointer(const char *type) noexcept;
	virtual const char *what() const noexcept;
private:
	const char *pointerType;
};

inline bad_pointer::bad_pointer(const char *type) noexcept{
	pointerType = type;
}

inline const char *bad_pointer::what() const noexcept{
	std::string s;
	std::stringstream ss;

	ss << "Invalid pointer to " << pointerType << "!\n";
	ss >> s;

	return s.c_str();
}