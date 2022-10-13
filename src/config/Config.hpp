#ifndef CONFIG_HPP
#define CONFIG_HPP

#include "ConfigInfo.hpp"
#include "ConfigLexer.hpp"
#include "ConfigParser.hpp"

class Config {
  private:
    ConfigInfo info_;
  public:
    void Parse(std::string filename);
    const ConfigInfo &GetConf() const;
};

#endif /* CONFIG_HPP */
