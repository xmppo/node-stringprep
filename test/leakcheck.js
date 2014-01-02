var SP = require('../index')
var next = global.setImmediate
  ? function () { setImmediate(run) }
  : function () { process.nextTick(run) }

function run() {
    var p = new SP.StringPrep('nameprep')
    var r = p.prepare('A\u0308ffin')
    if (r !== 'äffin')
        throw r
    next()
}

try {
    run()
    console.log('Success')
    process.exit(0)
} catch (e) {
    console.log(e.stack)
    process.exit(1)
}