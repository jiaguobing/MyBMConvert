/*
  Copyright 1999-2006 ImageMagick Studio LLC, a non-profit organization
  dedicated to making software imaging solutions freely available.

  You may not use this file except in compliance with the License.
  obtain a copy of the License at

    http://www.imagemagick.org/script/license.php

  Unless required by applicable law or agreed to in writing, software
  distributed under the License is distributed on an "AS IS" BASIS,
  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  See the License for the specific language governing permissions and
  limitations under the License.

  MagickCore quantum inline methods.
*/
#ifndef _MAGICKCORE_QUANTUM_H
#define _MAGICKCORE_QUANTUM_H

#if defined(__cplusplus) || defined(c_plusplus)
extern "C" {
#endif

static inline unsigned short RoundToMap(const MagickRealType value)
{
  if (value < 0.0)
    return(0UL);
  if (value >= (MagickRealType) MaxMap)
    return((unsigned short) MaxMap);
  return((unsigned short) (value+0.5));
}

static inline Quantum RoundToQuantum(const MagickRealType value)
{
  if (value < 0.0)
    return(0);
  if (value >= (MagickRealType) QuantumRange)
    return(QuantumRange);
  return((Quantum) (value+0.5));
}

static inline Quantum ScaleAnyToQuantum(const unsigned long x,
  const unsigned long scale)
{
  return((Quantum) (((MagickRealType) QuantumRange*x)/scale+0.5));
}

static inline unsigned long ScaleQuantumToAny(const Quantum x,
  const unsigned long scale)
{
  return((unsigned long) (((MagickRealType) scale*x)/QuantumRange+0.5));
}

#if (QuantumDepth == 8)
static inline Quantum ScaleCharToQuantum(const unsigned char value)
{
  return((Quantum) value);
}

static inline Quantum ScaleLongToQuantum(const unsigned long value)
{
  return((Quantum) (value/16843009UL));
}

static inline Quantum ScaleMapToQuantum(const unsigned long value)
{
  return((Quantum) value);
}

static inline unsigned long ScaleQuantum(const unsigned long quantum)
{
  return((unsigned long) quantum);
}

static inline unsigned char ScaleQuantumToChar(const Quantum quantum)
{
  return((unsigned char) quantum);
}

static inline unsigned long ScaleQuantumToLong(const Quantum quantum)
{
  return(16843009UL*quantum);
}

static inline unsigned long ScaleQuantumToMap(const Quantum quantum)
{
  return((unsigned long) quantum);
}

static inline unsigned short ScaleQuantumToShort(const Quantum quantum)
{
  return((unsigned short) (257*quantum));
}

static inline Quantum ScaleShortToQuantum(const unsigned short value)
{
  return((Quantum) (value/257));
}

static inline unsigned long ScaleToQuantum(const unsigned long value)
{
  return((unsigned long) value);
}
#elif (QuantumDepth == 16)
static inline Quantum ScaleCharToQuantum(const unsigned char value)
{
  return((Quantum) (257*value));
}

static inline Quantum ScaleLongToQuantum(const unsigned long value)
{
  return((Quantum) (value/65537UL));
}

static inline Quantum ScaleMapToQuantum(const unsigned long value)
{
  return((Quantum) value);
}

static inline unsigned long ScaleQuantum(const unsigned long quantum)
{
  return(quantum/257UL);
}

static inline unsigned char ScaleQuantumToChar(const Quantum quantum)
{
  return((unsigned char) (quantum/257UL));
}

static inline unsigned long ScaleQuantumToLong(const Quantum quantum)
{
  return(65537UL*quantum);
}

static inline unsigned long ScaleQuantumToMap(const Quantum quantum)
{
  return((unsigned long) quantum);
}

static inline unsigned short ScaleQuantumToShort(const Quantum quantum)
{
  return((unsigned short) quantum);
}

static inline Quantum ScaleShortToQuantum(const unsigned short value)
{
  return((Quantum) value);
}

static inline unsigned long ScaleToQuantum(const unsigned long value)
{
  return(257UL*value);
}
#elif (QuantumDepth == 32)
static inline Quantum ScaleCharToQuantum(const unsigned char value)
{
  return((Quantum) (16843009UL*value));
}

static inline Quantum ScaleLongToQuantum(const unsigned long value)
{
  return((Quantum) value);
}

static inline Quantum ScaleMapToQuantum(const unsigned long value)
{
  return((Quantum) (65537UL*value));
}

static inline unsigned long ScaleQuantum(const unsigned long quantum)
{
  return((unsigned long) (quantum/16843009UL));
}

static inline unsigned char ScaleQuantumToChar(const Quantum quantum)
{
  return((unsigned char) (quantum/16843009UL));
}

static inline unsigned long ScaleQuantumToLong(const Quantum quantum)
{
  return((unsigned long) quantum);
}

static inline unsigned long ScaleQuantumToMap(const Quantum quantum)
{
  return((unsigned long) (quantum/65537UL));
}

static inline unsigned short ScaleQuantumToShort(const Quantum quantum)
{
  return((unsigned short) (quantum/65537UL));
}

static inline Quantum ScaleShortToQuantum(const unsigned short value)
{
  return((Quantum) (65537UL*value));
}

static inline unsigned long ScaleToQuantum(const unsigned long value)
{
  return((unsigned long) (16843009UL*value));
}
#elif (QuantumDepth == 64)
static inline Quantum ScaleCharToQuantum(const unsigned char value)
{
  return((Quantum) (MagickULLConstant(71777214294589695)*value));
}

static inline Quantum ScaleLongToQuantum(const unsigned long value)
{
  return((Quantum)  4294967295UL*value);
}

static inline Quantum ScaleMapToQuantum(const unsigned long value)
{
  return((Quantum) (MagickULLConstant(281479271612415)*value));
}

static inline unsigned long ScaleQuantum(const unsigned long long quantum)
{
  return((unsigned long) (quantum/MagickULLConstant(71777214294589695)));
}

static inline unsigned char ScaleQuantumToChar(const Quantum quantum)
{
  return((unsigned char) (quantum/MagickULLConstant(71777214294589695));
}

static inline unsigned long ScaleQuantumToLong(const Quantum quantum)
{
  return((unsigned long) (quantum/MagickULLConstant(4294967297)));
}

static inline unsigned long ScaleQuantumToMap(const Quantum quantum)
{
  return((unsigned long) (quantum/MagickULLConstant(281479271612415)));
}

static inline unsigned short ScaleQuantumToShort(const Quantum quantum)
{
  return((unsigned short) (quantum/MagickULLConstant(281479271612415)));
}

static inline Quantum ScaleShortToQuantum(const unsigned short value)
{
  return((Quantum) (MagickULLConstant(281479271612415)*value));
}

static inline unsigned long long ScaleToQuantum(const unsigned long value)
{
  return(MagickULLConstant(71777214294589695)*value);
}
#endif

#if defined(__cplusplus) || defined(c_plusplus)
}
#endif

#endif
