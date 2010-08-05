var SP = require('./build/default/node-stringprep');

function run() {
    var p = new SP.StringPrep('nameprep');
    if (p.prepare('FooBar') !== 'foobar')
	throw p.prepare('FooBar');

    nextTick(run);
}

try {
    run();
} catch (e) {
    console.log(e);
}

