var SP = require('./build/Release/node_stringprep.node');

function run() {
    var p = new SP.StringPrep('nameprep');
    var r = p.prepare('A\u0308ffin');
    if (r !== 'äffin')
	throw r;

    process.nextTick(run);
}

try {
    run();
} catch (e) {
    console.log(JSON.stringify(e));
}

