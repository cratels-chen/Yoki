#ifndef A65E212B_8C6D_4C64_9253_C4DC4ADBE290
#define A65E212B_8C6D_4C64_9253_C4DC4ADBE290
#include <string>
#include <vector>


class UTGenerator {
public:
  static UTGenerator &getInstance() {
    static UTGenerator instance;
    return instance;
  }

  void generate();
  void generate(const std::string &filePath);
  void generate(const std::vector<std::string> &filePaths);   
};

#endif /* A65E212B_8C6D_4C64_9253_C4DC4ADBE290 */
