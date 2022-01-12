#include "EngineLogger.h"

EngineLogger* EngineLogger::instance = nullptr;

EngineLogger* EngineLogger::getInstance() {
  if (instance == nullptr) {
    instance = new EngineLogger();
  }

  return instance;
}
void EngineLogger::info(const string& msg) {
  if (level <= Level::INFO) {
    printf("[INFO] %s", msg.c_str());
  }
}
void EngineLogger::error(const string& msg) {
  printf("[ERROR] %s", msg.c_str());
}
void EngineLogger::setLevel(EngineLogger::Level lv) {
  level = lv;
}
