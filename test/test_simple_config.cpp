//
// Created by Joaquin Bejar Garcia on 19/10/23.
//

#include <simple_config/config.h>
#include <catch2/catch_test_macros.hpp>

class BasicConfig : public simple_config::Config {
public:

    BasicConfig() = default;

    //move constructor
    BasicConfig(BasicConfig &&other) noexcept {
        if (this != &other) {
            test1 = std::move(other.test1);
            test2 = other.test2;
            other.m_moved = true;
        }
    }

    //move assignment operator
    BasicConfig &operator=(BasicConfig &&other) noexcept {
        if (this != &other) {
            test1 = std::move(other.test1);
            test2 = other.test2;
            other.m_moved = true;
        }
        return *this;
    }

    std::string test1 = simple_config::get_env_variable_string("TEST1", "default");
    int test2 = simple_config::get_env_variable_int("TEST2", 0);

    [[nodiscard]] bool validate() override {
        return true;
    }

    [[nodiscard]] std::string to_string() const override {
        return R"({"BasicConfig":)" + this->get_basic_string() + "}";
    }

    void from_json(const json &j) override {
        test1 = j["test1"];
        test2 = j["test2"];
        *loglevel = j["loglevel"];
    }

    [[nodiscard]] json to_json() const override {
        json j;
        j["test1"] = test1;
        j["test2"] = test2;
        j["loglevel"] = *loglevel;

        return j;
    }

    static int public_get_env_variable_int(std::string const &key, int const &default_value) {
        return simple_config::get_env_variable_int(key, default_value);
    }

    static std::string public_get_env_variable_string(std::string const &key, std::string const &default_value) {
        return simple_config::get_env_variable_string(key, default_value);
    }

    static bool public_get_env_variable_bool(std::string const &key, bool const &default_value) {
        return simple_config::get_env_variable_bool(key, default_value);
    }


};

class BasicConfigWithError : public simple_config::Config {
public:
    std::string test1 = simple_config::get_env_variable_string("TEST1", "default");
    int test2 = simple_config::get_env_variable_int("TEST2", 0);

    [[nodiscard]] bool validate() override {
        return true;
    }

    [[nodiscard]] std::string to_string() const override {
        return this->get_basic_string();
    }

    void from_json(const json &j) override {
        this->test1 = j["test1"];
        this->test2 = j["test2"];
    }

    [[nodiscard]] json to_json() const override {
        json j;
        throw std::runtime_error("Not implemented");
    }

};

// ---------------------------------------------------------------------------------------------------
TEST_CASE("Declare config", "[Config]") {
    BasicConfig config;
    REQUIRE(config.validate());
}

TEST_CASE("Declare config with env", "[Config]") {
    setenv("LOGLEVEL", "error", 1);
    BasicConfig config;
    REQUIRE(config.validate());
    REQUIRE(*config.loglevel == "error");
}

TEST_CASE("Declare config and test ENV Var string", "[Config]") {
    setenv("TEST", "test", 1);
    BasicConfig config;
    REQUIRE(config.validate());
    REQUIRE(simple_config::get_env_variable_string("TEST", "default") == "test");
}

TEST_CASE("Declare config and test ENV Var int", "[Config]") {
    setenv("TEST", "1", 1);
    BasicConfig config;
    REQUIRE(config.validate());
    REQUIRE(simple_config::get_env_variable_int("TEST", 0) == 1);
}

TEST_CASE("Declare config and test ENV Var bool", "[Config]") {
    setenv("TEST", "true", 1);
    BasicConfig config;
    REQUIRE(config.validate());
    REQUIRE(simple_config::get_env_variable_bool("TEST", false) == true);
}

TEST_CASE("Declare config and test ENV Var bool default", "[Config]") {
    unsetenv("TEST");
    BasicConfig config;
    REQUIRE(config.validate());
    REQUIRE(simple_config::get_env_variable_bool("TEST", false) == false);
}

TEST_CASE("Declare config and test ENV Var using with default", "[Config]") {
    unsetenv("TEST1");
    unsetenv("TEST2");
    unsetenv("LOGLEVEL");
    BasicConfig config;
    REQUIRE(config.validate());
    REQUIRE(config.to_string() == R"({"BasicConfig":{"loglevel":"info","test1":"default","test2":0}})");
}

TEST_CASE("Declare config and test ENV Var not using default", "[Config]") {
    setenv("TEST1", "test", 1);
    setenv("TEST2", "12", 1);
    setenv("LOGLEVEL", "info", 1);
    BasicConfig config;
    REQUIRE(config.validate());
    REQUIRE(config.to_string() == R"({"BasicConfig":{"loglevel":"info","test1":"test","test2":12}})");
}

TEST_CASE("Declare config and test ENV Var using default to json", "[Config]") {
    unsetenv("TEST1");
    unsetenv("TEST2");
    unsetenv("LOGLEVEL");
    BasicConfig config;
    json j = config.to_json();
    REQUIRE(config.to_json().dump() == R"({"loglevel":"info","test1":"default","test2":0})");
}

TEST_CASE("Declare config and test ENV Var not using default to json", "[Config]") {
    setenv("TEST1", "test", 1);
    setenv("TEST2", "12", 1);
    setenv("LOGLEVEL", "info", 1);
    BasicConfig config;
    json j = config.to_json();
    REQUIRE(config.to_json().dump() == R"({"loglevel":"info","test1":"test","test2":12})");
}

TEST_CASE("Declare config and raise a json exception", "[Config]") {
    unsetenv("TEST1");
    unsetenv("TEST2");
    BasicConfigWithError config;
    REQUIRE_THROWS(config.to_json());
}

TEST_CASE("Declare move", "[Config]") {
    BasicConfig config;
    BasicConfig config3 = std::move(config);
    REQUIRE(config.is_moved());

    BasicConfig config4;
    BasicConfig config5(std::move(config4));
    REQUIRE(config4.is_moved());
}

TEST_CASE("Config operators std::string and <<", "[Config]") {
    setenv("TEST1", "test", 1);
    setenv("TEST2", "12", 1);
    setenv("LOGLEVEL", "info", 1);
    BasicConfig config;
    auto s = (std::string) config;
    REQUIRE(s == R"({"loglevel":"info","test1":"test","test2":12})");
    std::stringstream ss;
    ss << config;
    REQUIRE(ss.str() == R"({"BasicConfig":{"loglevel":"info","test1":"test","test2":12}})");
}

