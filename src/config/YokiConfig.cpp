#include "YokiConfig.h"
#include <cstdlib>
#include <fstream>
#include <nlohmann/json.hpp>
#include <regex>
#include <spdlog/spdlog.h>

using json = nlohmann::json;

// 静态成员变量定义
YokiConfig &YokiConfig::getInstance() {
  static YokiConfig instance;
  return instance;
}

std::string YokiConfig::getProgramName() { return programName; }

std::string YokiConfig::getProgramPath() { return programPath; }

std::vector<std::string> &YokiConfig::getRulesVec() { return rulesVec; }

std::vector<std::string> &YokiConfig::getExcludePaths() { return excludePaths; }

std::string YokiConfig::getMode() { return mode; }

std::shared_ptr<clang::tooling::CompilationDatabase>
YokiConfig::getCompilationDB() {
  return compilationDBPtr;
}

void YokiConfig::setCompilationDB(
    std::shared_ptr<clang::tooling::CompilationDatabase> db) {
  compilationDBPtr = db;
}

std::vector<std::string> &YokiConfig::getFileVec() { return fileVec; }

void YokiConfig::initializeFileVec() {
  // 构造compile_commands.json文件路径
  std::string compileCommandDir = programPath + "/build";
  std::string compileCommandFile = compileCommandDir + "/compile_commands.json";

  // 将字符串模式转换为正则表达式对象
  std::vector<std::regex> excludePatterns;
  for (const auto &pattern : excludePaths) {
    excludePatterns.push_back(std::regex(pattern));
  }

  std::ifstream file(compileCommandFile);
  if (!file.is_open()) {
    spdlog::error("Failed to open compile_commands.json: {}",
                  compileCommandFile);
    std::exit(EXIT_FAILURE);
  }

  fileVec.clear(); // 清空之前的内容
  nlohmann::json jsonData;
  file >> jsonData;
  for (const auto &entry : jsonData) {
    std::string filePath = entry["file"];
    bool exclude = false;
    for (const auto &excludePtn : excludePatterns) {
      if (std::regex_search(filePath, excludePtn)) {
        exclude = true;
        break;
      }
    }
    // 如果不在排除列表中，则添加到待检查文件列表中
    if (!exclude) {
      fileVec.push_back(filePath);
    }
  }
  // 关闭文件
  file.close();
}

bool YokiConfig::isStaticAnalysis() { return mode == "code_analysis"; }

bool YokiConfig::isTUGeneration() { return mode == "tu_generation"; }

bool YokiConfig::loadConfigFromFile(const std::string &filePath) {
  std::ifstream file(filePath);
  if (!file.is_open()) {
    spdlog::error("Failed to open config file: {}", filePath);
    return false;
  }

  json config;
  file >> config;

  // 文件格式
  // {
  //   "program_name" : "Program Name",
  //   "program_path" : "path/to/program",
  //   "rules_enabled": [ "rule1", "rule2", "rule3" ],
  //   "exclude_paths" : [ "path1", "path2", "path3" ],
  //   "mode" : "code_analysis|tu_generation"
  // }

  // 读取 JSON 数据
  programName = config["program_name"];
  programPath = config["program_path"];
  rulesVec = config["rules_enabled"].get<std::vector<std::string>>();
  excludePaths = config["exclude_paths"].get<std::vector<std::string>>();
  mode = config["mode"].get<std::string>();

  spdlog::info("Successfully load config from {}", filePath);

  // 关闭文件
  file.close();
  return true;
}
