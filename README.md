# reed-chessboard-controller
A simple controller and UCI compatible driver to create Reed switch-based smart chessboards using a Raspberry Pi Pico.

# Table of Contents
- [Table of Contents](#table-of-contents)
- [Description](#description)
- [Installation](#installation)
- [Usage](#usage)
- [License](#license)
- [Can I contribute?](#can-i-contribute)

# Description
[Back to top](#table-of-contents)

This was created mainly as a subproject of BEAUB, a robot playing OTB chess. I figured people might want to create smart reed switch-based smart chessboards themselves, since they're cheap and not too difficult, which is why this repo exists!

You can think of the project as two parts:
- The controller code, meant to run on the RP2040 micro-controller.
- The driver, making this act as a normal USB Serial Device that can be used by other programs.

# Installation
[Back to top](#table-of-contents)

# Usage
[Back to top](#table-of-contents)

(This is a mock, since everything is under developement, but it should look something like this by release time)
- Flash the "controller" code on your RP2040  based micro-controller.
- Follow the pinout to connect everything to your reed matrix.
- Launch the driver, then connect the board to your PC via USB.

# License
[Back to top](#table-of-contents)

The project is licensed under the MIT License. Do what you want with it, basically. Be cool if you credit the author if you use it tho!

# Can I contribute?
[Back to top](#table-of-contents)

Sure... *I guess?*

The project is most likely in a finished state as of the time this repo went public, and the subject isn't really particularily scalable.

Contributions are therefore welcome when it comes to bug fixes, etc, but there won't be any real fixed way of contributing.