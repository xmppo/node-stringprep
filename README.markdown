# node-stringprep

[![Build Status](https://travis-ci.org/astro/node-stringprep.png)](https://travis-ci.org/astro/node-stringprep)

[Flattr this!](https://flattr.com/thing/44598/node-stringprep)


Exposes predefined Unicode normalization functions that are required by many protocols. This is just a binding to [ICU](http://icu-project.org/), which is [said to be fast.](http://ayena.de/node/74).

If ICU is not available then we make use of JavaScript fallbacks.

## Usage

```javascript
    var StringPrep = require('node-stringprep').StringPrep;
    var prep = new StringPrep('nameprep');
    prep.prepare('Äffchen')  // => 'äffchen'
```

For a list of supported profiles, see [node-stringprep.cc](http://github.com/astro/node-stringprep/blob/master/node-stringprep.cc#L160)

Javascript fallbacks can be disabled/enabled using the following methods on the `StringPrep` object:

```javascript
var prep = new StringPrep('resourceprep')
prep.disableJsFallbacks()
prep.enableJsFallbacks()
```

Javascript fallbacks are enabled by default. You can also check to see if native `icu` bindings can/will be used by calling the `isNative()` method:

```javascript
var prep = new StringPrep('resourceprep')
prep.isNative()  // true or false
```

We also implement the ToASCII and ToUnicode operations as defined in the [IDNA RFC 3490](http://www.ietf.org/rfc/rfc3490.txt). These routines convert Unicode to ASCII with [NamePrep](http://www.ietf.org/rfc/rfc3491.txt) and then with [Punycode](http://www.ietf.org/rfc/rfc3492.txt), and vice versa.

```javascript
    var nodeStringPrep = require('node-stringprep');
    nodeStringPrep.toASCII('i♥u') // 'xn--iu-t0x'
    nodeStringPrep.toUnicode('xn--iu-t0x') // 'i♥u'
```

The operations can be finessed with an optional second argument, a set of boolean flags:

```javascript
    nodeStringPrep.toASCII('i♥u', {
        allowUnassigned: true, // allow unassigned code points to be converted
        throwIfError: true, // throw exception if error, don't return string unchanged
        useSTD3Rules: true // use the STD3 ASCII rules for host names
    })
    nodeStringPrep.toUnicode('xn--iu-t0x', {
        allowUnassigned: true // allow unassigned code points to be converted
    })
```



## Installation

```
    npm i node-stringprep
```

If `libicu` isn't available installation will gracefully fail and javascript fallbacks will be used.

If experiencing issues with __node-gyp__ please see https://github.com/TooTallNate/node-gyp/issues/363 which may be able to help.

### Debian

```
    apt-get install libicu-dev
```

### RedHat & Centos

```
    yum install libicu-devel
```

### Gentoo ###

```
    emerge icu
```

### OSX
#### MacPorts

```
    port install icu +devel
```

#### Boxen

```
    sudo ln -s /opt/boxen/homebrew/Cellar/icu4c/52.1/bin/icu-config /usr/local/bin/icu-config
    sudo ln -s /opt/boxen/homebrew/Cellar/icu4c/52.1/include/* /usr/local/include
```

#### Homebrew
    brew install icu4c
    ln -s /usr/local/Cellar/icu4c/<VERSION>/bin/icu-config /usr/local/bin/icu-config
    ln -s /usr/local/Cellar/icu4c/<VERSION>/include/* /usr/local/include

If experiencing issues with 'homebrew' installing version 50.1 of icu4c, try the following:

```
    brew search icu4c
    brew tap homebrew/versions
    brew versions icu4c
    cd $(brew --prefix) && git pull --rebase
    git checkout c25fd2f $(brew --prefix)/Library/Formula/icu4c.rb
    brew install icu4c
```

## Running Tests

```
npm test
```
