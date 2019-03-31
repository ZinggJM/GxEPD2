#ifndef _CONTROLLER_H_
#define _CONTROLLER_H_

class Controller
{
  public:
    virtual void spiBeginTransaction() = 0;
    virtual void spiEndTransaction() = 0;
};


class NullController : public Controller
{
  public:
    void spiBeginTransaction() {}
    void spiEndTransaction() {}
};

#endif