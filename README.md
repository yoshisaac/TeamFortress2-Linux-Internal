# What is this?
This is a program that is compiled into a Shared Object, and is used to give Extra Sensory Perception, automatic aiming, and automate player movement, in the game "Team Fortress 2". In a nutshell, this is a hack/cheat.  
  
Also, this project is primarily an unfaithful port of [this internal hack](https://github.com/faluthe/tf_c) to C++.  

# How does this work?
It internally (as a program that is injected inside tf2's process) reads and writes memory. A notable feature of it's functionality is using exposed symbols of other Shared Objects used by the game to run the game's own code against itself. The hack accomplishes this via interfaces of the game's own classes.

# How to compile
### Cloning Repository
Clone the repo using `git`.  

```bash
$ git clone https://github.com/yoshisaac/TeamFortress2-Linux-Internal.git
$ cd TeamFortress2-Linux-Internal/
```

### Install Dependencies
Install the necessary packages.  
  
Debian Advanced Package Tool (APT):  
```bash
$ sudo apt install #TODO
```
  
Arch Package Manager (pacman):  
```bash
$ sudo pacman -S #TODO
```

### Compiling
Build the program from source.  
```bash
$ make
...
```

### Running
Injecting the program into Team Fortress 2.  
```bash
$ sudo ./inject
[sudo] password for user:
...
Library loaded successfully at 0x12345678900. Use Ctrl+C to unload.
```

# Features
* Head aimbot
* Player ESP
  - Box
  - Name
  - Health
  - Flags
* Visuals
  - Remove scope & zoom
  - Override FOV
* Bhop

![ESP](https://r2.e-z.host/bb3dfc85-7f7f-4dcb-8b0b-3a4af0aa57e4/cvhlcwdqwgul2v1ia3.png)
![pickups](https://r2.e-z.host/bb3dfc85-7f7f-4dcb-8b0b-3a4af0aa57e4/6jhjaiseb021kvef5b.png)

# TODO
* Make better menu with SDL+IMGUI
* Add packages
