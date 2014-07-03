'use strict';

require('should')

it('Should convert to ASCII', function(done) {

    var SP = require('../index')

    var result = SP.toASCII('i\u2665u')
    result.should.equal('xn--iu-t0x')
    done()
})

it('Should throw on error', function(done) {

	var SP = require('../index')
	SP.toASCII.bind(SP, '\ud83d\ude03', { throwIfError: true }).should.throw()
	done()
})

it('Should convert unassigned code point', function(done) {

	var SP = require('../index')

	var result = SP.toASCII('\ud83d\ude03', { allowUnassigned : true })
	result.should.equal('xn--h28h')
	done()
})

it('Should error on non-STD3 char', function(done) {
	var SP = require('../index')
	SP.toASCII.bind(SP, 'abc#def', { throwIfError: true, useSTD3Rules: true }).should.throw()
	done()

})