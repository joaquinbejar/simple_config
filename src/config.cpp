//
// Created by Joaquin Bejar Garcia on 19/10/23.
//

#include "simple_config/config.h"

namespace simple_config {

    std::string get_env_variable_string(std::string const &key, std::string const &default_value) {
        char *c_queue = getenv(key.c_str());
        if (c_queue != nullptr) {
            return c_queue;
        } else {
            return default_value;
        }
    }

    std::set<std::string> get_env_variable_set_string(std::string const &key, std::string const &default_value) {
        std::string env_value;
        char *c_queue = getenv(key.c_str());
        if (c_queue != nullptr) {
            env_value = c_queue;
        } else {
            env_value = default_value;
        }

        // Use a stringstream to split the string
        std::stringstream ss(env_value);
        std::string token;
        std::set<std::string> result;
        while (std::getline(ss, token, ' ')) {
            // Further split token by comma
            std::stringstream inner_ss(token);
            std::string inner_token;
            while (std::getline(inner_ss, inner_token, ',')) {
                result.insert(inner_token);
            }
        }

        return result;
    }


    int get_env_variable_int(std::string const &key, int const &default_value) {
        char *c_queue = getenv(key.c_str());
        if (c_queue != nullptr) {
            try {
                return std::stoi(c_queue);
            } catch (std::invalid_argument &e) {
                return default_value;
            }
        } else {
            return default_value;
        }
    }

    bool get_env_variable_bool(const std::string &key, const bool &default_value) {
        char *c_queue = getenv(key.c_str());
        if (c_queue != nullptr && std::string(c_queue) == "true") {
            return true;
        } else if (c_queue != nullptr && std::string(c_queue) == "false") {
            return false;
        } else {
            return default_value;
        }
    }

    unsigned long long get_env_variable_long(const std::string &key, const int &default_value) {
        char *c_queue = getenv(key.c_str());
        if (c_queue != nullptr) {
            try {
                return std::stoull(c_queue);
            } catch (std::invalid_argument &e) {
                return default_value;
            }
        } else {
            return default_value;
        }
    }


    bool Config::is_moved() const {
        return m_moved;
    }

    std::string Config::get_basic_string() const {
        std::string str = this->to_json().dump();
        str.erase(std::remove(str.begin(), str.end(), ' '), str.end());
        return str;
    }

    Config::operator std::string() const {
        return this->get_basic_string();
    }

    std::ostream &operator<<(std::ostream &os, const Config &config) {
        os << config.to_string();
        return os;
    }


}
