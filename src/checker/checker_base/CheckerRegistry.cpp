#include "CheckerRegistry.h"

#include <algorithm>

CheckerRegistry &CheckerRegistry::getInstance() {
  static CheckerRegistry instance;
  return instance;
}

void CheckerRegistry::registerChecker(Entry entry) {
  std::lock_guard<std::mutex> lock(mtx);

  const auto it =
      std::find_if(entries.begin(), entries.end(), [&](const Entry &existing) {
        return existing.checkerName == entry.checkerName;
      });
  if (it != entries.end()) {
    return;
  }

  entries.emplace_back(std::move(entry));
}

std::vector<CheckerRegistry::Entry> CheckerRegistry::getAll() const {
  std::lock_guard<std::mutex> lock(mtx);
  auto copy = entries;
  std::sort(copy.begin(), copy.end(), [](const Entry &a, const Entry &b) {
    return a.checkerName < b.checkerName;
  });
  return copy;
}
