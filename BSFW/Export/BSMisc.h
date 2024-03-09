#ifndef __BS_MISC_H__
#define __BS_MISC_H__

#include <functional>

#ifdef _WIN32
#define BSFW_DECL_EXPORT __declspec(dllexport)
#define BSFW_DECL_IMPORT __declspec(dllimport)
#else
#define BSFW_DECL_EXPORT
#define BSFW_DECL_IMPORT
#endif // __WIN32

# if defined(BSFW_EXPORTS)
#  define BSFW_LIB_EXPORT BSFW_DECL_EXPORT
# else
#  define BSFW_LIB_EXPORT BSFW_DECL_IMPORT
# endif

namespace BSFW
{
	typedef std::function<void()> VOID_FUNC_VOID;
};

#endif