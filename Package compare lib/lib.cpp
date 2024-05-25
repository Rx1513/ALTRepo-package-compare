#include "lib.hpp"
#include <iostream>
#include <boost/json/src.hpp>
#include <cstdio>
#include <iostream>
#include <memory>
#include <stdexcept>
#include <string>
#include <array>

std::string exec(const char* cmd) {
    std::array<char, 128> buffer;
    std::string result;
    std::unique_ptr<FILE, decltype(&pclose)> pipe(popen(cmd, "r"), pclose);
    if (!pipe) {
        throw std::runtime_error("popen() failed!");
    }
    while (fgets(buffer.data(), buffer.size(), pipe.get()) != nullptr) {
        result += buffer.data();
    }
    return result;
}

using namespace boost::json;

void push_package_to_given_list(std::string list_name, object& result_json, object package){
    array& package_array = result_json.at("package_list").as_array();
    for (value& package_list_json_object : package_array)
    {
        object package_list = package_list_json_object.as_object();
        if(package_list.at("arch") == package.at("arch")){
            package_list.at(list_name).as_array().push_back(package);
            return;
        }
    }
    package_array.push_back(
        {
            { "arch", package.at("arch") },
            { list_name, array{ package } }
        });
}

bool compare_version(const object& first_package,const object& second_package)
{
    std::string commad = std::string("rpmvercmp ") + 
    std::to_string(first_package.at("epoch").as_int64()) + ':'  + value_to<std::string>(first_package.at("version")) + '-'  + value_to<std::string>(first_package.at("release"))
    + ' ' + 
    std::to_string(second_package.at("epoch").as_int64()) + ':'  + value_to<std::string>(second_package.at("version")) + '-'  + value_to<std::string>(second_package.at("release"));
    return std::stoi(exec(commad.c_str())) > 0;
}

std::string compare_branches(
    std::string first_branch_packages, 
    std::string second_branch_packages,
    std::string first_branch,
    std::string second_branch
    )
{
    array first_branch_json_packages = parse(first_branch_packages).as_object().at("packages").as_array();
    array second_branch_json_packages = parse(second_branch_packages).as_object().at("packages").as_array();
    
    object result_json = { 
        { "package_list", array() },
        { "first_branch", first_branch },
        { "second_branch", second_branch}
    };
    std::size_t last_second_index = 0; 
    
    for (std::size_t first_package_index = 0; first_package_index < first_branch_json_packages.size(); ++first_package_index)
    {
        object& first_package = first_branch_json_packages[first_package_index].as_object();
        bool first_package_not_found = true;
        for (std::size_t second_package_index = last_second_index; second_package_index < second_branch_json_packages.size(); ++second_package_index)
        {
            object& second_package = second_branch_json_packages[second_package_index].as_object();

            /*
                TODO:
                Modify condition for names
            */
            if ( first_package.at("arch").as_string()[0] < second_package.at("arch").as_string()[0] ) { break; }

            /*
                if names of packages are equal, then push all second branch packages 
                from last_second_index to current index,
                increase last_index 
                and at the end compare package versions
            */ 

            if( first_package.at("name") == second_package.at("name") && first_package.at("arch") == second_package.at("arch"))
            {
                first_package_not_found = false;
                for (std::size_t i = last_second_index; i < second_package_index; ++i)
                {
                    push_package_to_given_list("packages_presented_only_in_second_branch",result_json,second_package);
                }
                last_second_index = second_package_index + 1;
                if(compare_version(first_package,second_package)) {
                    push_package_to_given_list("packages_with_bigger_version_in_first_branch",result_json,second_package);
                };
                // compare versions
                break;
            }
        }

        if (first_package_not_found)
        {
            push_package_to_given_list("packages_presented_only_in_first_branch",result_json,first_package);
        }
    }

    std::cout << result_json << std::endl;
    return "";
};