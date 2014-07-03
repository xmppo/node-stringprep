'use strict';

require('should')

it('Should convert to Unicode', function(done) {

    var SP = require('../index')

    var result = SP.toUnicode('xn--iu-t0x')
    result.should.equal('i\u2665u')
    done()
})

it('Should convert unassigned code point', function(done) {

	var SP = require('../index')

	var result = SP.toUnicode('xn--h28h', { allowUnassigned : true })
	result.should.equal('\ud83d\ude03')
	done()
})
