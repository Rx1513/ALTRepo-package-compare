#include <boost/json/src.hpp>

#include <cstdio>
#include <memory>
#include <stdexcept>
#include <array>
#include "rpmvercmp.h"

using namespace boost;

void
pushPackageToGivenList(
    const std::string&& listName,
    json::object& outputJSON,
    const json::object package
);

bool
comparePackageVersion(
    const json::object& firstPackage,
    const json::object& secondPackage
);