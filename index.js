var namePrep, toUnicode, resourcePrep, nodePrep, StringPrep

try {
    var bindings = require('bindings')('node_stringprep.node')
} catch (ex) {
    console.warn(
        'Cannot load StringPrep-' +
        require('./package.json').version +
        ' bindings (using fallback). You may need to ' +
        '`npm install node-stringprep`'
    )
}

var toUnicode = function(value) {
    try {
        return bindings.toUnicode(value)
    } catch (e) {
        return value
    }
}

var StringPrep = function(operation) {
    this.operation = operation
    try {
        this.stringPrep = new bindings.StringPrep(this.operation)
    } catch (e) {
        this.stringPrep = null
    }
}

StringPrep.prototype.UNKNOWN_PROFILE_TYPE = 'Unknown profile type'
StringPrep.prototype.UNHANDLED_FALLBACK = 'Unhandled JS fallback'

StringPrep.prototype.prepare = function(value) {
    this.value = value
    try {
        if (this.stringPrep) {
            return this.stringPrep(this.value)
        }
    } catch (e) {}
    return this.jsFallback()
}

StringPrep.prototype.jsFallback = function() {
    switch (this.operation) {
        case 'nameprep':
        case 'nodeprep':
            return this.value.toLowerCase()
        case 'resourceprep':
            return this.value
        case 'nfs4_cs_prep':
        case 'nfs4_cis_prep':
        case 'nfs4_mixed_prep prefix':
        case 'nfs4_mixed_prep suffix':
        case 'iscsi':
        case 'mib':
        case 'saslprep':
        case 'trace':
        case 'ldap':
        case 'ldapci':
            throw new Error(this.UNHANDLED_FALLBACK)
        default:
            throw new Error(this.UNKNOWN_PROFILE_TYPE)
    }
}

module.exports = {
    toUnicode: toUnicode,
    StringPrep: StringPrep
}