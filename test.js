var SP = require('./build/Release/node_stringprep.node');
var p = new SP.StringPrep('nameprep');
var r = p.prepare('A\u0308ffin');
if (r !== 'äffin') console.log("Error preparing 'A\u0308ffin'. The result was " + r);
else console.log("The modules seems to work");