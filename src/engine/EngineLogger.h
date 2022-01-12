#ifndef ENGINELOGGER_H
#define ENGINELOGGER_H

#include <string>

using namespace std;

class EngineLogger {
 private:
  enum class Level { INFO = 1, ERROR = 2 } level = Level::INFO;
  static EngineLogger* instance;

  EngineLogger() = default;
 public:
  static EngineLogger* getInstance();
  void info(const string&);
  void error(const string&);
  void setLevel(Level lv);
};

#endif
