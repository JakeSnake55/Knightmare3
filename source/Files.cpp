#include "Files.h"
#include <filesystem>

namespace fs = std::filesystem;

Files::Files() 
{
  worldFolder = "./Worlds/";
  World sketch;
  addWorldFolder(sketch);
}

void Files::addWorldFolder(World currentWorld)
{
  fs::create_directory((worldFolder + currentWorld.name));
}
