#include <gtest/gtest.h>
#include "libpkgcmp.hpp"
#include "private_include.hpp"
#include <fstream>
#include <algorithm>

std::string readFile(
    const std::string& fileName
){
    std::ifstream ifstream(fileName.c_str(), std::ios::in | std::ios::binary | std::ios::ate);

    std::ifstream::pos_type fileSize = ifstream.tellg();
    ifstream.seekg(0, std::ios::beg);

    std::vector<char> bytes(fileSize);
    ifstream.read(bytes.data(), fileSize);
    return std::string(bytes.data(), fileSize);
}

class compare_branch : public testing::Test
{
protected:
    void SetUp() override { }

    virtual void TearDown() {}

    void testForDublicatesInSameList(const json::array&) const;
    void testForDublicatesInDifferentLists(const json::array&) const;

    static const std::string p10_packages;
    static const std::string sisyphus_packages;
    static const json::array& get_p10_sisyphus() { return output_for_p10_sisyphus; }
    static const json::array& get_sisyphus_p10() { return output_for_sisyphus_p10; }
private:
    static const json::array output_for_p10_sisyphus;
    static const json::array output_for_sisyphus_p10;
};

const std::string compare_branch::p10_packages = readFile("../tests/p10_package_list.json");
const std::string compare_branch::sisyphus_packages = readFile("../tests/sisyphus_package_list.json");
const json::array compare_branch::output_for_p10_sisyphus = json::parse(compareBranches(std::move(compare_branch::p10_packages), std::move(compare_branch::sisyphus_packages), "p10", "sysiphus")).as_object().at("package_list").as_array();
const json::array compare_branch::output_for_sisyphus_p10 = json::parse(compareBranches(std::move(compare_branch::sisyphus_packages), std::move(compare_branch::p10_packages), "p10", "sysiphus")).as_object().at("package_list").as_array();

void
compare_branch::testForDublicatesInSameList(
    const json::array& packagesList
) const {
    for (const json::value& arch_value : packagesList)
    {
        const json::object arch = arch_value.as_object();
        if (arch.contains("packages_presented_only_in_first_branch"))
        {
            const json::array packages = arch.at("packages_presented_only_in_first_branch").as_array();
            for (std::size_t i = 0; i < packages.size() - 1; ++i)
            {
                const json::object currentPackage = packages[i].as_object();
                const json::object nextPackage = packages[i + 1].as_object();
                ASSERT_NE(currentPackage.at("name"), nextPackage.at("name")) << "First branch has dublicates! " << currentPackage.at("name");
            }
        }

        if (arch.contains("packages_presented_only_in_second_branch"))
        {
            const json::array packages = arch.at("packages_presented_only_in_second_branch").as_array();
            for (std::size_t i = 0; i < packages.size() - 1; ++i)
            {
                const json::object currentPackage = packages[i].as_object();
                const json::object nextPackage = packages[i + 1].as_object();
                ASSERT_NE(currentPackage.at("name"), nextPackage.at("name")) << "Second branch has dublicates! " << currentPackage.at("name");
            }
        }
    }
}

void
compare_branch::testForDublicatesInDifferentLists(
    const json::array& packagesList
) const {
    for (const json::value& arch_value : packagesList)
    {
        const json::object arch = arch_value.as_object();
        if (arch.contains("packages_presented_only_in_first_branch") && arch.contains("packages_presented_only_in_second_branch"))
        {
            const json::array first_branch_packages = arch.at("packages_presented_only_in_first_branch").as_array();
            const json::array second_branch_packages = arch.at("packages_presented_only_in_second_branch").as_array();
            for (const json::value& first_branch_package : first_branch_packages)
            {
                for (const json::value& second_branch_package : second_branch_packages)
                {
                    ASSERT_NE(first_branch_package.as_object().at("name"), second_branch_package.as_object().at("name"));
                }
            }
        }
    }
}

TEST_F(
    compare_branch,
    commutativity
){
    const json::array& p10_sisyphus = get_p10_sisyphus();
    const json::array& sisyphus_p10 = get_sisyphus_p10();

    for (const json::value& arch_p10_value : p10_sisyphus)
    {
        const json::object& arch_p10 = arch_p10_value.as_object();
        for (const json::value& arch_sisyphus_value : sisyphus_p10)
        {
            const json::object& arch_sisyphus = arch_sisyphus_value.as_object();
            if(arch_p10.at("arch") != arch_sisyphus.at("arch")) continue;
            ASSERT_EQ(arch_p10.contains("packages_presented_only_in_first_branch"),arch_sisyphus.contains("packages_presented_only_in_second_branch"));
            ASSERT_EQ(arch_p10.contains("packages_presented_only_in_second_branch"),arch_sisyphus.contains("packages_presented_only_in_first_branch"));
            if(arch_p10.contains("packages_presented_only_in_first_branch"))
            {
                const json::array& p10_packages = arch_p10.at("packages_presented_only_in_first_branch").as_array();
                const json::array& sisyphus_packages = arch_sisyphus.at("packages_presented_only_in_second_branch").as_array();
                ASSERT_EQ(p10_packages.size(),sisyphus_packages.size());
                for (std::size_t i = 0; i < p10_packages.size(); ++i)
                {
                    ASSERT_EQ(p10_packages[i], sisyphus_packages[i]);
                }
            }
            if(arch_p10.contains("packages_presented_only_in_second_branch"))
            {
                const json::array& p10_packages = arch_p10.at("packages_presented_only_in_second_branch").as_array();
                const json::array& sisyphus_packages = arch_sisyphus.at("packages_presented_only_in_first_branch").as_array();
                ASSERT_EQ(p10_packages.size(),sisyphus_packages.size());
                for (std::size_t i = 0; i < p10_packages.size(); ++i)
                {
                    ASSERT_EQ(p10_packages[i], sisyphus_packages[i]);
                }
            }
        }
    }
}

TEST_F(
    compare_branch,
    p10_sisyphus_newer_packages_not_presented
){
    const json::array& packagesList = get_p10_sisyphus();
    for (const json::value& arch_value : packagesList)
    {
        const json::object arch = arch_value.as_object();
        ASSERT_FALSE(arch.contains("packages_with_bigger_version_in_first_branch")) << arch;
    }
}

TEST_F(
    compare_branch,
    p10_sisyphus_dublicate_packages_in_same_list
){
    testForDublicatesInSameList(get_p10_sisyphus());
}

TEST_F(
    compare_branch,
    p10_sisyphus_dublicate_packages_in_different_lists
){
    testForDublicatesInDifferentLists(get_p10_sisyphus());
}

TEST_F(
    compare_branch,
    sisyphus_p10_dublicate_packages_in_same_list
){
    testForDublicatesInSameList(get_sisyphus_p10());
}

TEST_F(
    compare_branch,
    sisyphus_p10_dublicate_packages_in_different_lists
){
    testForDublicatesInDifferentLists(get_sisyphus_p10());
}

TEST(
    push_pkg_to_list,
    first_insertion
){
    json::object outputJSON = {{"package_list", json::array()}};
    const json::object test_package = {{"name", "1"}, {"arch", "1"}};
    pushPackageToGivenList("First", outputJSON, test_package);
    json::object expected_result = {{
        "package_list", json::array
        {
            {
                {"arch", "1"},
                {"First", json::array 
                    {
                        {{"name", "1"}, {"arch", "1"}}
                    }
                }
            }
        }
    }};

    ASSERT_EQ(outputJSON, expected_result);
}

TEST(
    push_pkg_to_list,
    insertion_to_same_list
){
    json::object outputJSON = {{"package_list", json::array()}};

    const json::object test_package1 = {{"name", "1"}, {"arch", "1"}};
    const json::object test_package2 = {{"name", "2"}, {"arch", "1"}};
    const json::object test_package3 = {{"name", "3"}, {"arch", "1"}};

    pushPackageToGivenList("First", outputJSON, test_package1);
    pushPackageToGivenList("First", outputJSON, test_package2);

    json::object expected_result = {{
        "package_list", json::array
        {
            {
                {"arch", "1"},
                {"First", json::array 
                    {
                        {{"name", "1"}, {"arch", "1"}},
                        {{"name", "2"}, {"arch", "1"}}
                    }
                }
            }
        }
    }};

    ASSERT_EQ(outputJSON, expected_result);

    pushPackageToGivenList("First", outputJSON, test_package3);

    expected_result = {{
        "package_list", json::array
        {
            {
                {"arch", "1"},
                {"First", json::array 
                    {
                        {{"name", "1"}, {"arch", "1"}},
                        {{"name", "2"}, {"arch", "1"}},
                        {{"name", "3"}, {"arch", "1"}}
                    }
                }
            }
        }
    }};

    ASSERT_EQ(outputJSON, expected_result);
}

TEST(
    push_pkg_to_list,
    insertion_to_differnt_lists
){
    json::object outputJSON = {
        {"package_list", json::array()}};

    const json::object test_package1 = {
        {"name", "1"},
        {"arch", "1"},
    };

    const json::object test_package2 = {
        {"name", "2"},
        {"arch", "1"},
    };

    pushPackageToGivenList("First", outputJSON, test_package1);

    pushPackageToGivenList("Second", outputJSON, test_package1);

    json::object expected_result = {{
        "package_list", json::array
        {
            {
                {"arch", "1"},
                {"First", json::array 
                    {
                        {{"name", "1"}, {"arch", "1"}}
                    }
                },
                {"Second", json::array 
                    {
                        {{"name", "1"}, {"arch", "1"}}
                    }
                }
            }
        }
    }};

    ASSERT_EQ(outputJSON, expected_result);

    pushPackageToGivenList("Second", outputJSON, test_package2);

    expected_result = {{
        "package_list", json::array
        {
            {
                {"arch", "1"},
                {"First", json::array 
                    {
                        {{"name", "1"}, {"arch", "1"}}
                    }
                },
                {"Second", json::array 
                    {
                        {{"name", "1"}, {"arch", "1"}},
                        {{"name", "2"}, {"arch", "1"}}
                    }
                }
            }
        }
    }}; 

    ASSERT_EQ(outputJSON, expected_result);

    pushPackageToGivenList("First", outputJSON, test_package2);

    expected_result = {{
        "package_list", json::array
        {
            {
                {"arch", "1"},
                {"First", json::array 
                    {
                        {{"name", "1"}, {"arch", "1"}},
                        {{"name", "2"}, {"arch", "1"}}
                    }
                },
                {"Second", json::array 
                    {
                        {{"name", "1"}, {"arch", "1"}},
                        {{"name", "2"}, {"arch", "1"}}
                    }
                }
            }
        }
    }}; 

    ASSERT_EQ(outputJSON, expected_result);
}

TEST(
    push_pkg_to_list,
    insertion_to_differnt_archs
){
    json::object outputJSON = {
        {"package_list", json::array()}
    };

    const json::object test_package1 = {{"name", "1"}, {"arch", "1"}};

    const json::object test_package2 = {{"name", "1"}, {"arch", "2"}};

    pushPackageToGivenList("First", outputJSON, test_package1);
    pushPackageToGivenList("First", outputJSON, test_package2);

    json::object expected_result = {{
        "package_list", json::array{
            {
                {"arch", "1"},
                {"First", json::array
                    {
                        {{"name", "1"}, {"arch", "1"}}
                    }
                }
            },
            {
                {"arch", "2"},
                {"First", json::array
                    {
                        {{"name", "1"}, {"arch", "2"}}
                    }
                }
            }
        }
    }};

    ASSERT_EQ(outputJSON, expected_result);

    pushPackageToGivenList("Second", outputJSON, test_package2);

    expected_result = {{
        "package_list", json::array{
            {
                {"arch", "1"},
                {"First", json::array
                    {
                        {{"name", "1"}, {"arch", "1"}}
                    }
                }
            },
            {
                {"arch", "2"},
                {"First", json::array
                    {
                        {{"name", "1"}, {"arch", "2"}}
                    }
                },
                {"Second", json::array
                    {
                        {{"name", "1"}, {"arch", "2"}}
                    }
                },
            }
        }
    }};

    ASSERT_EQ(outputJSON, expected_result);

    pushPackageToGivenList("Second", outputJSON, test_package1);

    expected_result = {{
        "package_list", json::array{
            {
                {"arch", "1"},
                {"First", json::array
                    {
                        {{"name", "1"}, {"arch", "1"}}
                    }
                },
                {"Second", json::array
                    {
                        {{"name", "1"}, {"arch", "1"}}
                    }
                }
            },
            {
                {"arch", "2"},
                {"First", json::array
                    {
                        {{"name", "1"}, {"arch", "2"}}
                    }
                },
                {"Second", json::array
                    {
                        {{"name", "1"}, {"arch", "2"}}
                    }
                },
            }
        }
    }};

    ASSERT_EQ(outputJSON, expected_result);
}

TEST(
    package_version_comparison,
    same_version
){
    const json::object package = {
        {"epoch", 0},
        {"version", "1.0"},
        {"release", "alt1"}
    };
    ASSERT_FALSE(comparePackageVersion(package, package));
}

TEST(
    package_version_comparison,
    bigger_epoch
){
    const json::object package1 = {
        {"epoch", 1},
        {"version", "1.0"},
        {"release", "alt1"}
    };
    const json::object package2 = {
        {"epoch", 0},
        {"version", "1.0"},
        {"release", "alt1"}
    };
    ASSERT_TRUE(comparePackageVersion(package1, package2));
}

TEST(
    package_version_comparison,
    smaller_epoch
){
    const json::object package1 = {
        {"epoch", 0},
        {"version", "1.0"},
        {"release", "alt1"}
    };
    const json::object package2 = {
        {"epoch", 1},
        {"version", "1.0"},
        {"release", "alt1"}
    };
    ASSERT_FALSE(comparePackageVersion(package1, package2));
}

TEST(
    package_version_comparison,
    bigger_version
){
    const json::object package1 = {
        {"epoch", 1},
        {"version", "2.1"},
        {"release", "alt1"}};
    const json::object package2 = {
        {"epoch", 0},
        {"version", "1.0"},
        {"release", "alt1"}
    };
    ASSERT_TRUE(comparePackageVersion(package1, package2));
}

TEST(
    package_version_comparison,
    smaller_version
){
    const json::object package1 = {
        {"epoch", 0},
        {"version", "1.0"},
        {"release", "alt1"}
    };
    const json::object package2 = {
        {"epoch", 1},
        {"version", "1.2"},
        {"release", "alt1"}
    };
    ASSERT_FALSE(comparePackageVersion(package1, package2));
}

TEST(
    package_version_comparison,
    bigger_release
){
    const json::object package1 = {
        {"epoch", 1},
        {"version", "1.0"},
        {"release", "alt3"}
    };
    const json::object package2 = {
        {"epoch", 0},
        {"version", "1.0"},
        {"release", "alt1"}
    };
    ASSERT_TRUE(comparePackageVersion(package1, package2));
}

TEST(
    package_version_comparison,
    smaller_release
){
    const json::object package1 = {
        {"epoch", 0},
        {"version", "1.0"},
        {"release", "alt1"}
    };
    const json::object package2 = {
        {"epoch", 1},
        {"version", "1.0"},
        {"release", "alt3"}
    };
    ASSERT_FALSE(comparePackageVersion(package1, package2));
}

int main(int argc, char **argv)
{

    ::testing::InitGoogleTest(&argc, argv);

    return RUN_ALL_TESTS();
}