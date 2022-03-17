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

## Compiling
** You can skip this step by downloading the [realease package]() **
To compile from source, follow those commands:
```bash
git clone https://github.com/AloisRautureau/reed-chessboard-controller.git
cd reed-chessboard-controller
cmake -S . -B build
```
You'll then find the controller binary in `build/controller`.

## Flashing the controller code
You'll first need to flash the controller code (reed_chessboard_controller.uf2) onto your RP2040 based board. The specifics depend on the manufacturer, so please refer to the documentation of your specific controller for this step.

The pinout is as follows:
- Ranks (from A to H) are GPIO pins from 8 to 15
- Columns (from 1 to 8) are GPIO pins from 0 to 7
- The move validation button should be linked to GPIO 16

(I'll try to add a clear schematic for this, I swear)

# Usage
[Back to top](#table-of-contents)

(This is a mock, but should look something like this)
- Connect the controller via USB
- Run the driver software
- Play, and the moves you make should appear in the terminal! Cool stuff right there

# License
[Back to top](#table-of-contents)

The project is licensed under the MIT License. Do what you want with it, basically. Be cool if you credit the author if you use it tho!

# Can I contribute?
[Back to top](#table-of-contents)

Sure... *I guess?*

The project is most likely in a finished state as of the time this repo went public, and isn't particularily scalable.

Contributions are therefore welcome when it comes to bug fixes, etc, but there won't be any real fixed way of contributing. Just send an email and we'll figure something out!