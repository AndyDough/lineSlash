#pragma once
class BaseScreen {
public:
  virtual void update() = 0;
  virtual void draw() = 0;
};
