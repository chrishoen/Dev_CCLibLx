#pragma once

/*==============================================================================
==============================================================================*/

namespace CC
{

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Constants.

// Maximum tree node level.
static const int cMaxLevelDepth = 20;

// operator new[] adds extra bytes to store number of elements.
static const int cNewArrayExtraMemory = 4;

/*
#ifndef _MSC_VER
#define NOEXCEPT noexcept(true)
#else
#define NOEXCEPT
#endif
*/

#define NOEXCEPT noexcept(true)
//******************************************************************************
//******************************************************************************
//******************************************************************************
} //namespace

