# StripMaker
EuroScope plugin that creates and prints out customizable Flight Progress Strips

# Features
- Realistic generation of flight strips
- Integrated into EuroScope via a TAG Item
- Customizable strip formats
- Works with ESC/POS-capable printers (Not yet implemented)

# Status
Currently, the plugin is able to generate strips of 4 different types, in user-defined formats. Printer integration is in development; if you wish to print strips generated with the program, you can use the python-escpos library as a temporary work-around.

# Installation
Install by dragging the contents of the StripMaker.zip file into the folder of your choice. Then, open EuroScope and load 'StripMaker.dll' through the Other SET>Plug-in menu. Now, you can add the Print Status and Print Strip TAG Items and Functions to whichever list you wish.

# Support
If you encounter an error, create an Issue in this repo. Keep in mind the plugin is not stable for now.

# Contributing
This project is open for any contribution! If you see anything that I should've done differently, or want to add a new feature, submit a pull request via this repo.

# License
This plugin is released under the [GPL v3 (or later) license](https://www.gnu.org/licenses/gpl.html).
