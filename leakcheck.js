var SP = require('./build/Release/node_stringprep.node');
var useSetImmediate = (parseFloat(process.version.replace('v', '')) >= 0.10)
function run() {
    var p = new SP.StringPrep('nameprep');
    var r = p.prepare('A\u0308ffin');
    if (r !== 'äffin')
	throw r;

    if (true == useSetImmediate)
        setImmediate(run)
    else
        process.nextTick(run)
}

try {
    run();
} catch (e) {
    console.log(JSON.stringify(e));
}

