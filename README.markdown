# node-stringprep #

[Flattr this!](https://flattr.com/thing/44598/node-stringprep)

## Purpose ##

Exposes predefined Unicode normalization functions that are required by many protocols. This is just a binding to [ICU](http://icu-project.org/), which is [said to be fast.](http://ayena.de/node/74)

## Installation ##

    # Debian
    apt-get install libicu-dev

    # Gentoo
    emerge icu

    # OSX using MacPorts
    port install icu +devel

    # OSX using Homebrew
    brew install icu4c
    ln -s /usr/local/Cellar/icu4c/<VERSION>/bin/icu-config /usr/local/bin/icu-config

    npm install node-stringprep

## Usage ##

    var StringPrep = require('node-stringprep').StringPrep;
    var prep = new StringPrep('nameprep');
    prep.prepare('Äffchen')  // => 'äffchen'

For a list of supported profiles, see [node-stringprep.cc](http://github.com/astro/node-stringprep/blob/master/node-stringprep.cc#L160)
