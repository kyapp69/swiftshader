//===- subzero/crosstest/test_cast_main.cpp - Driver for tests ------------===//
//
//                        The Subzero Code Generator
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
// Driver for crosstesting cast operations.
//
//===----------------------------------------------------------------------===//

/* crosstest.py --test=test_cast.cpp --test=test_cast_to_u1.ll \
   --driver=test_cast_main.cpp --prefix=Subzero_ --output=test_cast */

#include <cstring>
#include <iostream>
#include <stdint.h>

// Include test_cast.h twice - once normally, and once within the
// Subzero_ namespace, corresponding to the llc and Subzero translated
// object files, respectively.
#include "test_cast.h"
namespace Subzero_ {
#include "test_cast.h"
}

#define XSTR(s) STR(s)
#define STR(s) #s
#define COMPARE(Func, FromCName, ToCName, Input, FromString)                   \
  do {                                                                         \
    ToCName ResultSz, ResultLlc;                                               \
    ResultLlc = Func<FromCName, ToCName>(Input);                               \
    ResultSz = Subzero_::Func<FromCName, ToCName>(Input);                      \
    ++TotalTests;                                                              \
    if (!memcmp(&ResultLlc, &ResultSz, sizeof(ToCName))) {                     \
      ++Passes;                                                                \
    } else {                                                                   \
      ++Failures;                                                              \
      std::cout << std::fixed << XSTR(Func) << "<" << FromString               \
                << ", " XSTR(ToCName) ">(" << Input << "): ";                  \
      if (sizeof(ToCName) == 1)                                                \
        std::cout << "sz=" << (int)ResultSz << " llc=" << (int)ResultLlc;      \
      else                                                                     \
        std::cout << "sz=" << ResultSz << " llc=" << ResultLlc;                \
      std::cout << "\n";                                                       \
    }                                                                          \
  } while (0)

template <typename FromType>
void testValue(FromType Val, size_t &TotalTests, size_t &Passes,
               size_t &Failures, const char *FromTypeString) {
  COMPARE(cast, FromType, bool, Val, FromTypeString);
  COMPARE(cast, FromType, uint8_t, Val, FromTypeString);
  COMPARE(cast, FromType, myint8_t, Val, FromTypeString);
  COMPARE(cast, FromType, uint16_t, Val, FromTypeString);
  COMPARE(cast, FromType, int16_t, Val, FromTypeString);
  COMPARE(cast, FromType, uint32_t, Val, FromTypeString);
  COMPARE(cast, FromType, int32_t, Val, FromTypeString);
  COMPARE(cast, FromType, uint64_t, Val, FromTypeString);
  COMPARE(cast, FromType, int64_t, Val, FromTypeString);
  COMPARE(cast, FromType, float, Val, FromTypeString);
  COMPARE(cast, FromType, double, Val, FromTypeString);
}

int main(int argc, char **argv) {
  size_t TotalTests = 0;
  size_t Passes = 0;
  size_t Failures = 0;

  volatile bool ValsUi1[] = { false, true };
  static const size_t NumValsUi1 = sizeof(ValsUi1) / sizeof(*ValsUi1);
  volatile uint8_t ValsUi8[] = { 0, 1, 0x7e, 0x7f, 0x80, 0x81, 0xfe, 0xff };
  static const size_t NumValsUi8 = sizeof(ValsUi8) / sizeof(*ValsUi8);

  volatile myint8_t ValsSi8[] = { 0, 1, 0x7e, 0x7f, 0x80, 0x81, 0xfe, 0xff };
  static const size_t NumValsSi8 = sizeof(ValsSi8) / sizeof(*ValsSi8);

  volatile uint16_t ValsUi16[] = { 0,      1,      0x7e,   0x7f,   0x80,
                                   0x81,   0xfe,   0xff,   0x7ffe, 0x7fff,
                                   0x8000, 0x8001, 0xfffe, 0xffff };
  static const size_t NumValsUi16 = sizeof(ValsUi16) / sizeof(*ValsUi16);

  volatile int16_t ValsSi16[] = { 0,      1,      0x7e,   0x7f,   0x80,
                                  0x81,   0xfe,   0xff,   0x7ffe, 0x7fff,
                                  0x8000, 0x8001, 0xfffe, 0xffff };
  static const size_t NumValsSi16 = sizeof(ValsSi16) / sizeof(*ValsSi16);

  volatile size_t ValsUi32[] = {
    0,          1,          0x7e,       0x7f,       0x80,
    0x81,       0xfe,       0xff,       0x7ffe,     0x7fff,
    0x8000,     0x8001,     0xfffe,     0xffff,     0x7ffffffe,
    0x7fffffff, 0x80000000, 0x80000001, 0xfffffffe, 0xffffffff
  };
  static const size_t NumValsUi32 = sizeof(ValsUi32) / sizeof(*ValsUi32);

  volatile size_t ValsSi32[] = {
    0,          1,          0x7e,       0x7f,       0x80,
    0x81,       0xfe,       0xff,       0x7ffe,     0x7fff,
    0x8000,     0x8001,     0xfffe,     0xffff,     0x7ffffffe,
    0x7fffffff, 0x80000000, 0x80000001, 0xfffffffe, 0xffffffff
  };
  static const size_t NumValsSi32 = sizeof(ValsSi32) / sizeof(*ValsSi32);

  volatile uint64_t ValsUi64[] = {
    0,                     1,                     0x7e,
    0x7f,                  0x80,                  0x81,
    0xfe,                  0xff,                  0x7ffe,
    0x7fff,                0x8000,                0x8001,
    0xfffe,                0xffff,                0x7ffffffe,
    0x7fffffff,            0x80000000,            0x80000001,
    0xfffffffe,            0xffffffff,            0x100000000ull,
    0x100000001ull,        0x7ffffffffffffffeull, 0x7fffffffffffffffull,
    0x8000000000000000ull, 0x8000000000000001ull, 0xfffffffffffffffeull,
    0xffffffffffffffffull
  };
  static const size_t NumValsUi64 = sizeof(ValsUi64) / sizeof(*ValsUi64);

  volatile int64_t ValsSi64[] = {
    0,                    1,                    0x7e,
    0x7f,                 0x80,                 0x81,
    0xfe,                 0xff,                 0x7ffe,
    0x7fff,               0x8000,               0x8001,
    0xfffe,               0xffff,               0x7ffffffe,
    0x7fffffff,           0x80000000,           0x80000001,
    0xfffffffe,           0xffffffff,           0x100000000ll,
    0x100000001ll,        0x7ffffffffffffffell, 0x7fffffffffffffffll,
    0x8000000000000000ll, 0x8000000000000001ll, 0xfffffffffffffffell,
    0xffffffffffffffffll
  };
  static const size_t NumValsSi64 = sizeof(ValsSi64) / sizeof(*ValsSi64);

  volatile float ValsF32[] = {
    0,                    1,                    1.4,
    1.5,                  1.6,                  -1.4,
    -1.5,                 -1.6,                 0x7e,
    0x7f,                 0x80,                 0x81,
    0xfe,                 0xff,                 0x7ffe,
    0x7fff,               0x8000,               0x8001,
    0xfffe,               0xffff,               0x7ffffffe,
    0x7fffffff,           0x80000000,           0x80000001,
    0xfffffffe,           0xffffffff,           0x100000000ll,
    0x100000001ll,        0x7ffffffffffffffell, 0x7fffffffffffffffll,
    0x8000000000000000ll, 0x8000000000000001ll, 0xfffffffffffffffell,
    0xffffffffffffffffll
  };
  static const size_t NumValsF32 = sizeof(ValsF32) / sizeof(*ValsF32);

  volatile double ValsF64[] = {
    0,                    1,                    1.4,
    1.5,                  1.6,                  -1.4,
    -1.5,                 -1.6,                 0x7e,
    0x7f,                 0x80,                 0x81,
    0xfe,                 0xff,                 0x7ffe,
    0x7fff,               0x8000,               0x8001,
    0xfffe,               0xffff,               0x7ffffffe,
    0x7fffffff,           0x80000000,           0x80000001,
    0xfffffffe,           0xffffffff,           0x100000000ll,
    0x100000001ll,        0x7ffffffffffffffell, 0x7fffffffffffffffll,
    0x8000000000000000ll, 0x8000000000000001ll, 0xfffffffffffffffell,
    0xffffffffffffffffll
  };
  static const size_t NumValsF64 = sizeof(ValsF64) / sizeof(*ValsF64);

  for (size_t i = 0; i < NumValsUi1; ++i) {
    bool Val = ValsUi1[i];
    testValue<bool>(Val, TotalTests, Passes, Failures, "bool");
  }
  for (size_t i = 0; i < NumValsUi8; ++i) {
    uint8_t Val = ValsUi8[i];
    testValue<uint8_t>(Val, TotalTests, Passes, Failures, "uint8_t");
  }
  for (size_t i = 0; i < NumValsSi8; ++i) {
    myint8_t Val = ValsSi8[i];
    testValue<myint8_t>(Val, TotalTests, Passes, Failures, "int8_t");
  }
  for (size_t i = 0; i < NumValsUi16; ++i) {
    uint16_t Val = ValsUi16[i];
    testValue<uint16_t>(Val, TotalTests, Passes, Failures, "uint16_t");
  }
  for (size_t i = 0; i < NumValsSi16; ++i) {
    int16_t Val = ValsSi16[i];
    testValue<int16_t>(Val, TotalTests, Passes, Failures, "int16_t");
  }
  for (size_t i = 0; i < NumValsUi32; ++i) {
    uint32_t Val = ValsUi32[i];
    testValue<uint32_t>(Val, TotalTests, Passes, Failures, "uint32_t");
    COMPARE(castBits, uint32_t, float, Val, "uint32_t");
  }
  for (size_t i = 0; i < NumValsSi32; ++i) {
    int32_t Val = ValsSi32[i];
    testValue<int32_t>(Val, TotalTests, Passes, Failures, "int32_t");
  }
  for (size_t i = 0; i < NumValsUi64; ++i) {
    uint64_t Val = ValsUi64[i];
    testValue<uint64_t>(Val, TotalTests, Passes, Failures, "uint64_t");
    COMPARE(castBits, uint64_t, double, Val, "uint64_t");
  }
  for (size_t i = 0; i < NumValsSi64; ++i) {
    int64_t Val = ValsSi64[i];
    testValue<int64_t>(Val, TotalTests, Passes, Failures, "int64_t");
  }
  for (size_t i = 0; i < NumValsF32; ++i) {
    for (unsigned j = 0; j < 2; ++j) {
      float Val = ValsF32[i];
      if (j > 0)
        Val = -Val;
      testValue<float>(Val, TotalTests, Passes, Failures, "float");
      COMPARE(castBits, float, uint32_t, Val, "float");
    }
  }
  for (size_t i = 0; i < NumValsF64; ++i) {
    for (unsigned j = 0; j < 2; ++j) {
      double Val = ValsF64[i];
      if (j > 0)
        Val = -Val;
      testValue<double>(Val, TotalTests, Passes, Failures, "double");
      COMPARE(castBits, double, uint64_t, Val, "double");
    }
  }

  std::cout << "TotalTests=" << TotalTests << " Passes=" << Passes
            << " Failures=" << Failures << "\n";
  return Failures;
}
