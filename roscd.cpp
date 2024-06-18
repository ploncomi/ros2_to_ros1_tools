#include <sys/ioctl.h>

#include <cstdlib>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>
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

std::vector<std::string> split(std::string s, std::string delimiter)
{
  size_t pos_start = 0, pos_end, delim_len = delimiter.length();
  std::string token;
  std::vector<std::string> res;

  while ((pos_end = s.find(delimiter, pos_start)) != std::string::npos)
  {
    token = s.substr (pos_start, pos_end - pos_start);
    pos_start = pos_end + delim_len;
    res.push_back (token);
  }

  res.push_back (s.substr (pos_start));
  return res;
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

  bool go_to_share_dir = false;

  if (argc >= 3 && std::string(argv[2]) == "--share")
    go_to_share_dir = true;


  std::vector<std::string> all_packages;

  std::string src_path;

  if (go_to_share_dir)
    src_path = getEnv("ROS_PACKAGE_PATH") + "/../..";
  else
    src_path = getEnv("ROS_PACKAGE_PATH") + "/../../../src";
  std::filesystem::current_path({src_path});

  bool any_ok = false;

  for(auto& p: fs::recursive_directory_iterator(".", fs::directory_options::skip_permission_denied))
  {
    std::ifstream f(p.path().string() + "/package.xml");

    if (!f.good())
      continue;

    std::vector<std::string> splitted = split(p.path().string(), "/");
    all_packages.push_back(splitted.back());

    if (!hasEnding(p.path().string(), argv[1]))
      continue;

    inject_shell("cd " + src_path + "/" + p.path().string() + "\r");
    any_ok = true;
    break;
  }

  if (!any_ok)
  {
    if (std::string(argv[1]) == "--list")
    {
      for (std::string s : all_packages)
        std::cout << s << " ";
    }
    else
    {
      std::cout << argv[1] << " is not a ros package name" << std::endl;
      std::cout << "available packages:" << std::endl;
      std::sort(all_packages.begin(), all_packages.end());
      for (std::string s : all_packages)
        std::cout << "  " << s << std::endl;
    }
  }

  return 0;
}
