#pragma once
#include <string>
#include <vector>
#include "World.h"

class Files 
{
private:
  std::string worldFolder;
public:
  Files();
  //Creates folder in WorldFolder directory
  void addWorldFolder(World currentWorld);
  //Removes 
  void removeWorldFolder(World currentWorld);

  void loadWorldFolder(std::vector<World> worldList);

};