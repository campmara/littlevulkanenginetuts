#ifndef PIPELINE_H
#define PIPELINE_H

#include <string>
#include <vector>

namespace XIV {
    class Pipeline {
    public:
        Pipeline(const std::string &vertPath, const std::string &fragPath);

    private:
        std::vector<char> ReadFile(const std::string &fileName);

        void Create(const std::string &vertPath, const std::string &fragPath);
    };
} // namespace XIV

#endif