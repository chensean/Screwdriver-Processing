#ifndef SCREWDRIVER_CORE_HPP
#define SCREWDRIVER_CORE_HPP

#if defined(_MSC_VER)
# pragma once
#endif

#include <boost/config.hpp>

#if defined(SCREWDRIVER_CORE_LINK_DYNAMIC)
# if defined(SCREWDRIVER_CORE_COMPILATION)
#   define SCREWDRIVER_CORE_API BOOST_SYMBOL_EXPORT
# else
#   define SCREWDRIVER_CORE_API BOOST_SYMBOL_IMPORT
# endif
#else
# define SCREWDRIVER_CORE_API
#endif
#endif // SCREWDRIVER_CORE_HPP
