#pragma once
#include "Common.hpp"
#include "wpl_core.hpp"

namespace cidr::data {


struct UserLaunchPaths {
  using DataT = std::tuple<std::string, std::array<std::string, 3>>;
  DataT data{};
  std::string& AppDataPath() { return std::get<1>(data)[0]; }
  std::string& AppResourcesPath() { return std::get<1>(data)[1]; }
  std::string& UserName() { return std::get<0>(data); }
  std::string& UserDataPath() { return std::get<1>(data)[2]; }
  static nlohmann::json ToJson(const UserLaunchPaths& obj) {
    nlohmann::json j;
    j["kUserName"] = std::get<0>(obj.data);
    j["kAppDataPath"] = std::get<1>(obj.data)[0];
    j["kAppResourcesPath"] = std::get<1>(obj.data)[1];
    j["kUserDataPath"] = std::get<1>(obj.data)[2];
    return j;
  }
  static UserLaunchPaths FromJson(const nlohmann::json& obj) {
    UserLaunchPaths ret;
    ret.UserName() = obj.at("kUserName").get<std::string>();
    ret.AppDataPath() = obj.at("kAppDataPath").get<std::string>();
    ret.AppResourcesPath() = obj.at("kAppResourcesPath").get<std::string>();
    ret.UserDataPath() = obj.at("kUserDataPath").get<std::string>();
    return ret;
  }
};


struct LaunchPaths {
  using DataT = std::map<std::string,UserLaunchPaths>;
  DataT data;

  UserLaunchPaths& GetUserLaunchPaths(std::string_view user_id)  { return data.at(std::string(user_id.data(),user_id.size())); }
  static nlohmann::json ToJson(const LaunchPaths& obj) { 
    nlohmann::json j;
    for (const auto& [user_id, user_cache] : obj.data) {
      j[user_id] = UserLaunchPaths::ToJson(user_cache);
    }
    return j;
  };
  static LaunchPaths FromJson(const nlohmann::json& obj) { 
    LaunchPaths ret;
    for (const auto& [user_id_str, user_json] : obj.items()) {  
      UserLaunchPaths user_cache = UserLaunchPaths::FromJson(user_json);
      ret.data[user_id_str] = user_cache;
    }
    return ret;
  };
};

}  // namespace cidr::data::cache
