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
  if (currentWorld.name == "") {
      return;
  }
  //here I want to make a file with actual display name and seed. (folder name currently used for display name, but prevents duplicate names)
}

std::vector<std::string> get_directories(const std::string& s)
{
    std::vector<std::string> r;
    for (auto& p : std::filesystem::recursive_directory_iterator(s))
        if (p.is_directory())
            r.push_back(p.path().string().substr(p.path().string().find_last_of("/\\")+1));
   
    return r;
}

void Files::loadWorldFolder(std::vector<World> &worldList)
{
    std::vector<std::string> s = get_directories("./Worlds/");
    worldList.resize(s.size());
    for (int i = 0; i < worldList.size(); i++) 
    {
        worldList[i].name = s[i];
        worldList[i].seed = 0;
    }
}

