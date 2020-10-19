#pragma once

class Renderable
{
public:
  virtual ~Renderable() = default;
  virtual void render() = 0;
  virtual bool init() = 0;

  const int id;

protected:
  explicit Renderable(int id);
};


