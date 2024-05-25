#include "lib.hpp"
#include <boost/json/src.hpp>
#include <cstdio>
#include <memory>
#include <stdexcept>
#include <array>


using namespace boost;


std::string
exec(
    const char* cmd
){
    std::array<char, 128> buffer;
    std::string result;
    std::unique_ptr<FILE, decltype(&pclose)> pipe(popen(cmd, "r"), pclose);

    if (!pipe)
    {
        throw std::runtime_error("popen() failed!");
    }

    while (fgets(buffer.data(), buffer.size(), pipe.get()) != nullptr)
    {
        result += buffer.data();
    }

    return result;
}


void
pushPackageToGivenList(
    const std::string&& listName,
    json::object& outputJSON,
    const json::object package
){
    json::array& packageList = outputJSON.at("package_list").as_array();

    for (json::value& packageListObject : packageList)
    {
        json::object& givenPackageList = packageListObject.as_object();

        if(givenPackageList.at("arch") == package.at("arch"))
        {
            if(givenPackageList.find(listName) == givenPackageList.end())
            {
                givenPackageList.emplace( listName, json::array{ package } );
                return;
            }
            givenPackageList.at(listName).as_array().push_back(package);
            return;
        }
    }

    packageList.push_back(
        {
            { "arch", package.at("arch") },
            { listName, json::array{ package } }
        }
    );
}


bool
comparePackageVersion(
    const json::object& firstPackage,
    const json::object& secondPackage
){
    std::string commad = std::string("rpmvercmp ") +  // Utility to compare branches

    std::to_string(firstPackage.at("epoch").as_int64()) + ':'  // First package version
    + json::value_to<std::string>(firstPackage.at("version")) + '-'  
    + json::value_to<std::string>(firstPackage.at("release"))
    
    + ' ' + 

    std::to_string(secondPackage.at("epoch").as_int64()) + ':' // Second package version
    + json::value_to<std::string>(secondPackage.at("version")) + '-' 
    + json::value_to<std::string>(secondPackage.at("release"));

    return std::stoi(exec(commad.c_str())) > 0;
}


std::string
compareBranches(
    
    const std::string&& firstBranchPackages, 
    const std::string&& secondBranchPackages,
    const std::string&& firstBranch,
    const std::string&& secondBranch
){
    // Parsing list of packages fiven by altrepo server
    const json::array firstBranchPackageList = json::parse(firstBranchPackages).as_object().at("packages").as_array();
    const json::array secondBranchPackageList = json::parse(secondBranchPackages).as_object().at("packages").as_array();
    
    // Output JSON
    json::object outputJSON = { 
        { "package_list", json::array() },
        { "first_branch", firstBranch },
        { "second_branch", secondBranch}
    };

    /*
        Index used to skip checking packages that are above first package
        Since all packages are sorted alphabetically it's unnecessary
        to check package above last found package
    */
    std::size_t lastSecondIndex = 0;
    for (
        std::size_t firstPackageIndex = 0;
        firstPackageIndex < firstBranchPackageList.size();
        ++firstPackageIndex
    ){
        const json::object& firstPackage = firstBranchPackageList[firstPackageIndex].as_object();
        
        bool first_package_not_found = true;
        for (
            std::size_t secondPackageIndex = lastSecondIndex;
            secondPackageIndex < secondBranchPackageList.size();
            ++secondPackageIndex
        ){
            const json::object& secondPackage = secondBranchPackageList[secondPackageIndex].as_object();

            /*
                TODO:
                Modify condition for package names
            */

            if ( firstPackage.at("arch").as_string()[0] < secondPackage.at("arch").as_string()[0] ) { break; }

            /*
                if names of packages are equal, then push all second branch packages 
                from last_second_index to current index,
                increase last_index 
                and at the end compare package versions
            */ 

            if(
                firstPackage.at("name") == secondPackage.at("name")
                &&
                firstPackage.at("arch") == secondPackage.at("arch")
            ){
                first_package_not_found = false;
                for (std::size_t i = lastSecondIndex; i < secondPackageIndex; ++i)
                {
                    pushPackageToGivenList(
                        "packages_presented_only_in_second_branch",
                        outputJSON,
                        secondPackage
                    );
                }
                lastSecondIndex = secondPackageIndex + 1;
                if(comparePackageVersion(firstPackage,secondPackage)) {
                    pushPackageToGivenList(
                        "packages_with_bigger_version_in_first_branch",
                        outputJSON,
                        secondPackage
                    );
                };
                break;
            }
        }

        if (first_package_not_found)
        {
            pushPackageToGivenList(
                "packages_presented_only_in_first_branch",
                outputJSON,
                firstPackage
            );
        }
    }

    return serialize(outputJSON);
};