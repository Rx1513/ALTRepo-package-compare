#include "lib.hpp"
#include <iostream>
#include <boost/json/src.hpp>

using namespace boost::json;



// {
//   "request_args": {},
//   "length": 0,
//   "packages": [
//     {
//       "name": "string",
//       "epoch": 0,
//       "version": "string",
//       "release": "string",
//       "arch": "string",
//       "disttag": "string",
//       "buildtime": 0,
//       "source": "string"
//     }
//   ]
// }

// {
//   "first_branch: string",
//   "second_branch: string",
// [
//   "arch: string",
//   "packages_presented_only_in_first_bracnh": {
//      "length": 0,
//      "list": [
//          {
//          "name": "string",
//          "epoch": 0,
//          "version": "string",
//          "release": "string",
//          "arch": "string",
//          "disttag": "string",
//          "buildtime": 0,
//          "source": "string"
//          }
//      ]
//  },
//   "packages_presented_only_in_second_bracnh": {
//      "length": 0,
//      "list": [
//          {
//          "name": "string",
//          "epoch": 0,
//          "version": "string",
//          "release": "string",
//          "arch": "string",
//          "disttag": "string",
//          "buildtime": 0,
//          "source": "string"
//          }
//      ]
//  },
//   "packages_with_version_older_in_first_branch": {
//      "length": 0,
//      "list": [
//          {
//          "name": "string",
//          "epoch": 0,
//          "version": "string",
//          "release": "string",
//          "arch": "string",
//          "disttag": "string",
//          "buildtime": 0,
//          "source": "string"
//          }
//      ]
//  }
// }
// ]
// }

// void
// add_package_to_result_json(object& result_json, object package){
//     array package_array = result_json.at("package_list").as_array();
//     for (auto package_list : package_array)
//     {
//         auto x = package_list.as_object();
//         if(x.at("arch") == package.at("arch")){
//             x.at("packages_presented_only_in_first_bracnh").as_object().at("list").as_array().push_back(package);
//             return;
//         }
//     }
//     package_array.push_back(
//         {
//             {"arch", package.at("arch") },
//             {"packages_presented_only_in_first_bracnh", { "list", {} }}
//         });
// }

std::string compare_branches(
    std::string first_branch_packages, 
    std::string second_branch_packages
    )
{
    array first_branch_json_packages = parse(first_branch_packages).as_object().at("packages").as_array();
    array second_branch_json_packages = parse(second_branch_packages).as_object().at("packages").as_array();
    
    object result_json = { { "package_list", array() } };

    std::size_t last_second_index = 0; 
    for (std::size_t first_package_index = 0; first_package_index < first_branch_json_packages.size(); ++first_package_index)
    {
        object first_package = first_branch_json_packages[first_package_index].as_object();
        bool first_package_not_found = true;
        for (std::size_t second_package_index = last_second_index; second_package_index < second_branch_json_packages.size(); ++second_package_index)
        {

            object second_package = second_branch_json_packages[second_package_index].as_object();

            /*
                Since all packages sorted in ascending order of arch names and then for package name
                we can skip further checks
            */

            if( first_package.at("name").as_string()[0] > second_package.at("name").as_string()[0] ) { break; }

            /*
                if names of packages are equal, then push all second branch packages 
                from last_second_index to current index,
                increase last_index 
                and at the end compare package versions
            */ 

            if( first_package.at("name") == second_package.at("name"))
            {
                first_package_not_found = false;
                for (std::size_t i = last_second_index; i < second_package_index; ++i)
                {
                    // push to packages presented in second branch
                }
                last_second_index = second_package_index + 1;
                // compare versions
                break;
            }
        }
        if (first_package_not_found)
        {
             // push to packages presented in first branch
        }
    }

    std::cout << result_json << std::endl;
    return "";
};