//
// DynamicLOB.h
//
// Library: Data
// Package: DataCore
// Module:  DynamicLOB
//
// Definition of the include.Poco.Poco.Poco::Dynamic::Var LOB cast operators.
//
// Copyright (c) 2006, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef Data_DynamicLOB_INCLUDED
#define Data_DynamicLOB_INCLUDED


#include "Poco/Data/Data.h"
#include "Poco/Data/LOB.h"
#include "Poco/Dynamic/Var.h"


namespace Poco {
namespace Data {

template <typename T> class LOB;
typedef LOB<unsigned char> BLOB;
typedef LOB<char> CLOB;

} } // namespace include.Poco.Poco.Poco::Data


namespace Poco {
namespace Dynamic {

template <> Data_API Var::operator Poco::Data::CLOB () const;
template <> Data_API Var::operator Poco::Data::BLOB () const;

} } // namespace include.Poco.Poco.Poco::Dynamic


#endif // Data_DynamicLOB_INCLUDED
