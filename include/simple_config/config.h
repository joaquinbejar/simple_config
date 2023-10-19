//
// Created by Joaquin Bejar Garcia on 19/10/23.
//

#ifndef SIMPLE_CONFIG_CONFIG_H
#define SIMPLE_CONFIG_CONFIG_H

#include <iostream>
#include "nlohmann/json.hpp"
#include <set>
#include <sstream>

using json = nlohmann::json;

namespace simple_config {

    std::string get_env_variable_string(std::string const &key, std::string const &default_value);

    std::set<std::string> get_env_variable_set_string(std::string const &key, std::string const &default_value);

    int get_env_variable_int(std::string const &key, int const &default_value);

    unsigned long long get_env_variable_long(std::string const &key, int const &default_value);

    bool get_env_variable_bool(std::string const &key, bool const &default_value);

    class ConfigException : public std::exception {
    public:
        const std::string &message;

        explicit ConfigException(std::string const &message) : message(message) {}

        [[nodiscard]] const char *what() const noexcept override {
            return message.c_str();
        }
    };

    class Config {
    public:

        std::shared_ptr<std::string> loglevel = std::make_shared<std::string>(
                get_env_variable_string("LOGLEVEL", "info"));

        [[maybe_unused]] [[nodiscard]] virtual bool validate() = 0;

        [[nodiscard]] virtual std::string to_string() const = 0;

        [[nodiscard]] virtual json to_json() const = 0;

        virtual void from_json(const json &j) = 0;

        [[nodiscard]] virtual bool is_moved() const final;

        virtual explicit operator std::string() const final;

        friend std::ostream &operator<<(std::ostream &os, const Config &config);


    protected:
        bool m_moved = false;

        [[nodiscard]] virtual std::string get_basic_string() const final;

    };


}

#endif //SIMPLE_CONFIG_CONFIG_H
