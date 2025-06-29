#ifndef CONVAR_HPP
#define CONVAR_HPP

class Convar {
public:
  void set_int(int value) {
    *(int*)(this + 0x58) = value;
  }

  int get_int() {
    return *(int*)(this + 0x58);
  }

};

#endif
