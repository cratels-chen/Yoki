#include "DefectManager.h"
#include "YokiConfig.h"
#include "spdlog/spdlog.h"
#include <chrono>
#include <cstdlib>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <regex>
#include <set>
#include <sstream>
#include <unistd.h>

void DefectManager::addDefect(const Defect &defect) {
  // 锁定互斥量，确保线程安全
  std::lock_guard<std::mutex> lock(defectMutex);
  // 插入缺陷到管理器中
  defects.push_back(defect);
}

void DefectManager::dumpAsJson() {}

void DefectManager::dumpAsHtml() {
  // 生成带时间戳的文件名
  auto now = std::chrono::system_clock::now();
  auto time_t = std::chrono::system_clock::to_time_t(now);
  std::ostringstream timestamp;
  timestamp << std::put_time(std::localtime(&time_t), "%Y%m%d_%H%M%S");

  // 创建报告文件夹
  std::string reportDirName = "Yoki_Report_" + timestamp.str();
  std::string reportDir = reportDirName;

  // 创建目录
  std::string mkdirCmd = "mkdir -p " + reportDir;
  if (system(mkdirCmd.c_str()) != 0) {
    spdlog::error("Failed to create report directory: {}", reportDir);
    return;
  }

  std::string htmlFilename = reportDir + "/report.html";

  // 读取模板文件
  std::ifstream templateFile("resources/report_template/report_template.html");
  if (!templateFile.is_open()) {
    spdlog::error(
        "Failed to open report_template/report_template.html for reading.");
    return;
  }

  std::string htmlTemplate((std::istreambuf_iterator<char>(templateFile)),
                           std::istreambuf_iterator<char>());
  templateFile.close();

  // 生成当前时间字符串
  std::ostringstream timeStr;
  timeStr << std::put_time(std::localtime(&time_t), "%Y-%m-%d %H:%M:%S");

  // 生成缺陷数据行
  std::ostringstream defectRows;
  for (const auto &defect : defects) {
    defectRows << "                        <tr>\n";
    defectRows << "                            <td>\n";
    defectRows << "                                <span class=\"defect-id\">"
               << escapeHtml(defect.getDefectId()) << "</span>\n";
    defectRows << "                            </td>\n";
    defectRows << "                            <td>\n";
    defectRows
        << "                                <span class=\"checker-name\">"
        << escapeHtml(defect.getBelongingChecker().getName()) << "</span>\n";
    defectRows << "                            </td>\n";
    defectRows << "                            <td>\n";
    defectRows
        << "                                <div class=\"defect-message\">"
        << escapeHtml(defect.getDefectMessage()) << "</div>\n";
    defectRows << "                            </td>\n";
    defectRows << "                            <td>\n";
    defectRows << "                                <span class=\"file-path\">"
               << escapeHtml(defect.getFilePath()) << "</span>\n";
    defectRows << "                            </td>\n";
    defectRows << "                            <td>\n";
    defectRows << "                                <span class=\"line-number\">"
               << defect.getLineNumber() << "</span>\n";
    defectRows << "                            </td>\n";
    defectRows << "                        </tr>\n";
  }

  // 替换模板中的占位符
  htmlTemplate = std::regex_replace(
      htmlTemplate, std::regex("\\{\\{SCAN_TIME\\}\\}"), timeStr.str());
  htmlTemplate =
      std::regex_replace(htmlTemplate, std::regex("\\{\\{DEFECT_COUNT\\}\\}"),
                         std::to_string(defects.size()));
  htmlTemplate = std::regex_replace(
      htmlTemplate, std::regex("\\{\\{FILE_COUNT\\}\\}"), getUniqueFileCount());
  htmlTemplate =
      std::regex_replace(htmlTemplate, std::regex("\\{\\{PROJECT_NAME\\}\\}"),
                         YokiConfig::getInstance().getProgramName());
  htmlTemplate = std::regex_replace(
      htmlTemplate, std::regex("\\{\\{DEFECT_ROWS\\}\\}"), defectRows.str());
  htmlTemplate = std::regex_replace(
      htmlTemplate, std::regex("\\{\\{GENERATION_TIME\\}\\}"), timeStr.str());

  // 将 logo 文件拷贝到当前目录
  std::string copyLogoCmd = "cp resources/yoki_logo.png " + reportDir + "/";
  if (system(copyLogoCmd.c_str()) != 0) {
    spdlog::warn("Failed to copy logo file to report directory");
  }

  // 修改logo路径为当前目录
  htmlTemplate = std::regex_replace(
      htmlTemplate, std::regex("\\.\\.\/yoki_logo\\.png"), "yoki_logo.png");

  // 写入最终报告文件
  std::ofstream htmlFile(htmlFilename);
  if (!htmlFile.is_open()) {
    spdlog::error("Failed to open {} for writing.", htmlFilename);
    return;
  }

  htmlFile << htmlTemplate;
  htmlFile.close();

  // 创建压缩包
  std::string zipFilename = reportDirName + ".zip";
  std::string zipCmd =
      "cd " + reportDir + " && zip -r ../" + zipFilename + " . && cd ..";
  if (system(zipCmd.c_str()) == 0) {
    spdlog::info("Report package created successfully: {}", zipFilename);
    // 删除临时目录
    std::string removeCmd = "rm -rf " + reportDir;
    system(removeCmd.c_str());
  } else {
    spdlog::error("Failed to create report package");
    spdlog::info("Report files available in directory: {}", reportDir);
  }
}

// 辅助方法实现
std::string DefectManager::escapeHtml(const std::string &text) const {
  std::string result = text;
  std::regex_replace(result, std::regex("&"), "&amp;");
  result = std::regex_replace(result, std::regex("<"), "&lt;");
  result = std::regex_replace(result, std::regex(">"), "&gt;");
  result = std::regex_replace(result, std::regex("\""), "&quot;");
  result = std::regex_replace(result, std::regex("'"), "&#39;");
  return result;
}

std::string DefectManager::getUniqueFileCount() const {
  std::set<std::string> uniqueFiles;
  for (const auto &defect : defects) {
    uniqueFiles.insert(defect.getFilePath());
  }
  return std::to_string(uniqueFiles.size());
}

std::string DefectManager::getCurrentWorkingDirectory() const {
  char *cwd = getcwd(nullptr, 0);
  if (cwd != nullptr) {
    std::string result(cwd);
    free(cwd);
    return result;
  }
  return "Unknown";
}
