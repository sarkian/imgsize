imgsize - Simple tool that prints the image(s) size using [LibGD](http://libgd.bitbucket.org/).
===============================================================================================

Usage
-----
```
$ imgsize [OPTION] ~/path/to/image.png
    or
$ imgsize [OPTION] img1.png img2.jpg # etc.
    or
$ imgsize [OPTION] /path/to/directory/with/images/*
```

Options
-------
* **-o** - Print only size, without filename
* **-n** - Not print newlines
* **-s** - Not display errors
* **-b** - Space before
* **-a** - Space after
* **-h** - Display help and exit

Installing
----------
```
$ git clone https://github.com/sarkian/imgsize
$ cd imgsize
$ make
$ sudo make install
```

Dependensies
------------
* LibGD (On Ubuntu just install ```libgd-dev```)

Uninstalling
------------
```
$ sudo make uninstall
```

License
-------
[MIT License](http://opensource.org/licenses/mit-license.php)

