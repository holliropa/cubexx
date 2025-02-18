#pragma once

#include <map>
#include <string>
#include <vector>
#include <memory>
#include "mesh.hpp"

class MeshManager {
public:
    static std::map<std::string, std::shared_ptr<Mesh>> Meshes;

    static std::shared_ptr<Mesh> Get(const std::string &name);

    static void Clear();

private:
    MeshManager() {}
};