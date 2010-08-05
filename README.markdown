# node-stringprep #

(Flattr this!)[https://flattr.com/thing/44598/node-stringprep]

## Purpose ##

Exposes predefined Unicode normalization functions that are required by many protocols. This is just a binding to [ICU](http://icu-project.org/), which is [said to be fast.](http://ayena.de/node/74)

## Installation ##

    npm install node-stringprep

## Usage ##

    var StringPrep = require('node-stringprep').StringPrep;
    var prep = new StringPrep('nameprep');
    prep.prepare('Äffchen')  // => 'äffchen'
