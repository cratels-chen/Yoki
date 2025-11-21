#include "Utils.h"
#include "YokiConfig.h"
#include <cstdlib>
#include <fstream>
#include <nlohmann/json.hpp>
#include <regex>
#include <spdlog/spdlog.h>

std::vector<std::string> getFileVec(const std::string &compileCommandDir) {
  auto compileCommandFile = compileCommandDir + "/compile_commands.json";

  // 从YokiConfig单例获取排除路径
  auto &config = YokiConfig::getInstance();
  const auto &excludePaths = config.getExcludePaths();

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

  std::vector<std::string> fileVec;
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
  return fileVec;
}
