//
// Created by Joaquin Bejar Garcia on 19/10/23.
//

#include "simple_config/config.h"

namespace simple_config {

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
