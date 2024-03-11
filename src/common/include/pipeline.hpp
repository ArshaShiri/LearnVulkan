#ifndef SRC_COMMON_INCLUDE_PIPELINE
#define SRC_COMMON_INCLUDE_PIPELINE

#include <string>
#include <vector>

class Pipeline
{
  public:
    Pipeline(const std::string &vertFilepath, const std::string &fragFilepath);

  private:
    void createGraphicsPipeline(const std::string &vertFilepath, const std::string &fragFilepath);
};

#endif /* SRC_COMMON_INCLUDE_PIPELINE */
