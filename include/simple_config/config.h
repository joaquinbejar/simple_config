//
// Created by Joaquin Bejar Garcia on 19/10/23.
//

#ifndef SIMPLE_CONFIG_CONFIG_H
#define SIMPLE_CONFIG_CONFIG_H

#include <iostream>
#include "nlohmann/json.hpp"
#include <set>
#include <sstream>
#include <common/common.h>

using json = nlohmann::json;

namespace simple_config {


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

        std::string loglevel = common::get_env_variable_string("LOGLEVEL", "info");

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
