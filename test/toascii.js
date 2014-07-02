'use strict';

require('should')

it('Should convert to ASCII', function(done) {

    var SP = require('../index')

    var result = SP.toASCII('I\u2665U')
    result.should.equal('xn--iu-t0x')
    done()
})
