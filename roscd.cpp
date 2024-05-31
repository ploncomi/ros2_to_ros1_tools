#include <sys/ioctl.h>

#include <cstdlib>
#include <iostream>
#include <string>
#include <filesystem>

namespace fs = std::filesystem;

void inject_shell(const char* cmd)
{
  int i = 0;
  while (cmd[i] != '\0')
  {
    ioctl(0, TIOCSTI, &cmd[i++]);
  }
}

void inject_shell(std::string cmd)
{
  for (char& c : cmd)
    ioctl(0, TIOCSTI, &c);
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
  std::cout << std::filesystem::current_path() << std::endl;

  for(auto& p: fs::recursive_directory_iterator(".", fs::directory_options::skip_permission_denied))
  {
    if (p.path().string().find(argv[1]) != std::string::npos)
    {
      inject_shell("cd " + src_path + "/" + p.path().string() + "\r");
      break;
    }
  }

  return 0;
}
