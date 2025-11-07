#include <drogon/drogon.h>
#include <iostream>

int main() {
  drogon::app().loadConfigFile("config.json").run();
}
