#ifndef KEYLISTENER_H
#define KEYLISTENER_H

#include <set>

using namespace std;


class KeyListener {
 private:
  int key;
  bool wasPressed{};

 public:
  explicit KeyListener(int key): key(key) { }
  bool isDownEvent(const set<int>& keys) {
    if (keys.count(key)) {
      if (!wasPressed) {
        wasPressed = true;
        return true;
      }
    } else {
      wasPressed = false;
    }
    return false;
  }
};

#endif
