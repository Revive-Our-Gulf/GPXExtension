# GPX Extension #

## What is it? ##

This is an extension of BlueOS designed for the operation of the BlueROV. The extension has the following functionality:

* It logs position and status information to a database.
* It takes video sequences as queries, and outputs corresponding [GPX](https://wiki.openstreetmap.org/wiki/GPX#:~:text=GPX%2C%20or%20GPS%20exchange%20format,and%20convert%20to%20other%20forms). files according to data that has been logged.

## Technical Details ##

* This project was spawned from the recommended BlueOS extension [template](https://github.com/BlueOS-community/blueos-extension-template).
* It is expected to run in a docker image, as is standard practice with BlueOS extensions. 
* Data is recorded to the [MariaDB](https://mariadb.org/) database, which is supposed to be lightweight (it runs on a Raspberry Pi 4).
The NGINX web server was replaced using [CrowCpp](https://crowcpp.org/master/) for web support (similar to Flask in Python). The main design choices here were speed and lightweight code.

## Project Setup ##

The project is made up of two main files:

* _Logger_: Reads information from the BlueROV and logs it to a database.
* _StatusTool: Provides the Extension GUI, contains the GPX generation logic and handles the queries.