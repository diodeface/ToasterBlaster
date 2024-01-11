#pragma once

class ToasterComponent {
  protected:
    bool enabled;

  public:
    bool isEnabled() {
        return enabled;
    }

    virtual void start() {};

    virtual void stop() {};

    virtual void pause() {}

    virtual void reset() {};
    
    virtual void update() = 0;
};