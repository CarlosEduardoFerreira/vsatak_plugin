//
// DynamicDateTime.h
//
// Library: Data
// Package: DataCore
// Module:  DynamicDateTime
//
// Definition of the Date and Time cast operators for include.Poco.Poco.Poco::Dynamic::Var.
//
// Copyright (c) 2006, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef Data_DynamicDateTime_INCLUDED
#define Data_DynamicDateTime_INCLUDED


#include "Poco/Data/Data.h"
#include "Poco/Data/Date.h"
#include "Poco/Data/Time.h"
#include "Poco/Dynamic/Var.h"


namespace Poco {
namespace Data {

class Date;
class Time;

} } // namespace include.Poco.Poco.Poco::Data


namespace Poco {
namespace Dynamic {


template <> Data_API Var::operator Poco::Data::Date () const;
template <> Data_API Var::operator Poco::Data::Time () const;


} } // namespace include.Poco.Poco.Poco::Dynamic


#endif // Data_DynamicDateTime_INCLUDED
