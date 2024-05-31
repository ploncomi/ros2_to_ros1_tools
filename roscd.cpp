#include <sys/ioctl.h>

#include <cstdlib>
#include <iostream>
#include <fstream>
#include <string>
#include <filesystem>

namespace fs = std::filesystem;

void inject_shell(std::string cmd)
{
  for (char& c : cmd)
    ioctl(0, TIOCSTI, &c);
}

bool hasEnding (std::string const &fullString, std::string const &ending)
{
    if (fullString.length() >= ending.length())
    {
        return (0 == fullString.compare (fullString.length() - ending.length(), ending.length(), ending));
    }
    else
    {
        return false;
    }
}

std::string getEnv( const std::string & var )
{
  const char * val = std::getenv( var.c_str() );
  if ( val == nullptr )
  {
     return "";
  }
  else
  {
     return val;
  }
}


int main(int argc, char *argv[])
{
  if (argc == 1)
  {
    std::string ros_path = getEnv("ROS_PACKAGE_PATH");
    inject_shell("cd " + ros_path + "/../../../build\r");
    return 0;
  }

  std::string src_path = getEnv("ROS_PACKAGE_PATH") + "/../../../src";
  std::filesystem::current_path({src_path});

  bool any_ok = false;

  for(auto& p: fs::recursive_directory_iterator(".", fs::directory_options::skip_permission_denied))
  {
    if (!hasEnding(p.path().string(), argv[1]))
      continue;

    std::ifstream f(p.path().string() + "/package.xml");

    if (!f.good())
      continue;

    inject_shell("cd " + src_path + "/" + p.path().string() + "\r");
    any_ok = true;
    break;
  }

  if (!any_ok)
    std::cout << argv[1] << " is not a ros package name" << std::endl;

  return 0;
}
