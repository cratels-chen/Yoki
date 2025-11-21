#ifndef DD43D779_114E_4540_9E76_5E38ED4204A3
#define DD43D779_114E_4540_9E76_5E38ED4204A3
#include "CheckerBase.h"
#include <clang/Basic/SourceLocation.h>
#include <string>

class Defect {
public:
  Defect() = default;
  Defect(const std::string &defectId, CheckerBase &belongingChecker,
         const std::string &defectMessage, const std::string &filePath,
         int lineNumber)
      : defectId(defectId), belongingChecker(belongingChecker),
        defectMessage(defectMessage), filePath(filePath),
        lineNumber(lineNumber) {}

  // setters
  void setDefectId(const std::string &id) { defectId = id; }
  void setBelongingChecker(CheckerBase &checker) { belongingChecker = checker; }
  void setDefectMessage(const std::string &message) { defectMessage = message; }
  void setLineNumber(int line) { lineNumber = line; }
  void setFilePath(const std::string &path) { filePath = path; }

  // getters
  std::string getDefectId() const { return defectId; }
  CheckerBase &getBelongingChecker() const { return belongingChecker; }
  std::string getDefectMessage() const { return defectMessage; }
  int getLineNumber() const { return lineNumber; }
  std::string getFilePath() const { return filePath; }

private:
  std::string defectId;
  std::string defectMessage;
  unsigned int lineNumber = 0;
  std::string filePath;
  CheckerBase &belongingChecker;
};

#endif /* DD43D779_114E_4540_9E76_5E38ED4204A3 */
