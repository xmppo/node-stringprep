# node-stringprep #

[Flattr this!](https://flattr.com/thing/44598/node-stringprep)

## Purpose ##

Exposes predefined Unicode normalization functions that are required by many protocols. This is just a binding to [ICU](http://icu-project.org/), which is [said to be fast.](http://ayena.de/node/74)

## Installation ##

    npm i node-stringprep
    
### Debian ###

    apt-get install libicu-dev

### Gentoo ###

emerge icu

### OSX ###
#### MacPorts ####
    port install icu +devel

#### Homebrew ####
    brew install icu4c
    ln -s /usr/local/Cellar/icu4c/<VERSION>/bin/icu-config /usr/local/bin/icu-config
    ln -s /usr/local/Cellar/icu4c/<VERSION>/include/unicode /usr/local/include

If experiencing issues with 'homebrew' installing version 50.1 of icu4c, try the following:

    brew search icu4c
    brew tap homebrew/versions
    brew versions icu4c
    cd $(brew --prefix) && git pull --rebase
    git checkout c25fd2f $(brew --prefix)/Library/Formula/icu4c.rb
    brew install icu4c
    
## Usage ##

    var StringPrep = require('node-stringprep').StringPrep;
    var prep = new StringPrep('nameprep');
    prep.prepare('Äffchen')  // => 'äffchen'

For a list of supported profiles, see [node-stringprep.cc](http://github.com/astro/node-stringprep/blob/master/node-stringprep.cc#L160)
