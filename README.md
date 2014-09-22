VFD.Clock
=========

Digital clock based on [Vacuum Fluorescent Display](http://en.wikipedia.org/wiki/Vacuum_fluorescent_display).

License
-------
This program is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.


How to get this code?
---------------------

You can get it code in 2 ways:

- Download it by clicking at `ZIP` button at the top of Github page.
- Using git clone instruction.

In a second case You nedd to install on Your computer git software.

If You are Linux OS user please install few package listed above:

- git-core
- gitk
- gettext

After that, in a folder where You want to save sources, please type:

`git clone https://github.com/divano/VFD.Clock.git`

How to use this code?
----------------------

How You want to. This is standard procedure to compile program for AVR microcontrolors.

- Type `make all` to compile the sources.
- Type `make program` to flash the hex file into AVR.
- Type `make clean` to clean all temporary generated files.

Fuse bits
----------

In CONFIG file You can find fuse bits values.
To set the fuse bits in MCU, You have to run in example:

    avrdude -c usbasp -p atmega8515 -U lfuse:w:0x1f:m
    avrdude -c usbasp -p atmega8515 -U hfuse:w:0xd9:m

Known problems
--------------

No one. Please report to me if found anyone.
 
Version history
---------------

- Version 2.0:

    - Current release.

- Version 1.0 (oscilate about 2005 year):

    - Old release. You can find this source code at: 

[http://elportal.pl/index.php?module=ContentExpress&func=display&ceid=241](http://elportal.pl/index.php?module=ContentExpress&func=display&ceid=241)

Thanks to
----------

From beginning of this project, there are two persons without which this project will never see the sun light. In first release of this programm, the main programm file contained followed comments:

    Program is dedicated for two persons: AP & NB Without who, my world would be gray and boring... Thank You Very Much !!!

And once again I would like to say big Thank You to You both. It is true that without you both this program would never created.
