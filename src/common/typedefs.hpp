#ifndef TYPEDEFS_H
#define TYPEDEFS_H

#include <cstddef>
#include <memory>

namespace splicpp
{
	typedef unsigned char byte;
	typedef unsigned int uint;
	typedef unsigned long ulong;
	
	typedef size_t stid; //Symbol Type Identifier (Lexer / Parser)
	typedef size_t rid; //Rule Identifier
	typedef size_t stateid; //State Identifier
	typedef size_t sid; //Symbol Identifier (Variables, Functions)
	
	template <typename T>
	using s_ptr = std::shared_ptr<T>;
	
	template <typename T>
	using cs_ptr = std::shared_ptr<const T>;
}

#endif
