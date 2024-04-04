#pragma once

#ifndef MACRO_HELPER
#define MACRO_HELPER

#define _DLL __declspec(dllexport)

// https://stackoverflow.com/questions/5222008/calling-a-function-immediately-before-main
#define BOOST_PP_CAT(a, b) BOOST_PP_CAT_I(a, b)
#define BOOST_PP_CAT_I(a, b) BOOST_PP_CAT_II(~, a ## b)
#define BOOST_PP_CAT_II(p, res) res

// This avoids repeating the BOOST_PP_CAT 5X
#define STATIC_EXECUTE \
  STATIC_EXECUTE_I(BOOST_PP_CAT(__StaticExecute__, __LINE__))

// This is the meat, a static instance of a class whose constructor runs your code
#define STATIC_EXECUTE_I(uniq_name)               \
static struct uniq_name {                         \
  uniq_name();                                    \
} BOOST_PP_CAT(uniq_name, __var);                 \
uniq_name::uniq_name() 

#endif