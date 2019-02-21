class MyJSON {
    static get QUOTE() { return '\"' };
    static isBlank(c) {
        return [' ', '\t', '\n', '\r'].includes(c);
    }

    static parse(str) {
        const result = {};
        const buf = str.split('');
        const len = buf.length;
        let index = 0;
        let isStartObj = false;
        let isWaitForComma = false;
        let isWaitForColon = false;

        while (index < len) {
            if (!this.isBlank(buf[index])) {
                if (!isStartObj) {
                    if (buf[index] !== '{') throw new Error('{ expected at ' + index);
                    isStartObj = true;
                } else {
                    if (isWaitForComma && (buf[index] === ',' || buf[index] === '}')) {
                        isWaitForComma = false;
                    } else {
                        if (isWaitForComma) throw new Error(', or } expected at ' + index);
                    }

                    if (buf[index] === '}') break;
                    const key = this.matchStr(buf, index, len);
                    if (key.err) throw new Error(key.msg);
                    index = key.next;

                    let next = this.findColon(buf, index, len);
                    if (index < 0) throw new Error(': expected at ' + index);
                    index = next;

                    let value = this.matchStr(buf, index, len);
                    if (value.err) {
                        value = this.matchNumber(buf, index, len);
                        if (value.err) {
                            value = this.matchPrimitave(buf, index, len);
                            if (value.err) {
                                value.value = this.parse(str.slice(index));
                            }
                        }
                    }

                    result[key.value] = value.value;
                    index = value.next; // ++ (next - 1) = next
                }
            }
            ++index;
        }

        return result;
    }

    /**
     * 
     * @param {string[]} buf 
     * @param {number} index 
     * @param {number} len
     * @returns {{value: string, next: number, err: boolean, msg: string}} string & next index 
     */
    static matchStr(buf, index, len) {
        while (this.isBlank(buf[index])) ++index;

        if (buf[index] !== this.QUOTE) {
            return {
                err: true,
                msg: "Expect \" at " + index,
            };
        }
        ++index;
        let str = '';
        let startEscape = false;

        while (index < len) {
            if (buf[index] === this.QUOTE) {
                return {
                    value: str,
                    next: index + 1,
                }
            }
            if (!this.isBlank(buf[index])) {
                if (buf[index] === '\\') {
                    if (!startEscape) {
                        startEscape = true;
                    } else {
                        str += '\\';
                    }
                } else {
                    if (startEscape) {
                        switch (buf[index]) {
                            case 'r': str += '\r'; break;
                            case 'n': str += '\n'; break;
                            case 't': str += '\t'; break;
                            default:
                                return {
                                    err: true,
                                    msg: "Cannot read escape string at " + index,
                                };
                        }
                        startEscape = false;
                    } else {
                        str += buf[index];
                    }
                }
            }
            ++index;
        }

        // when go to there, string haven't read
        return {
            err: true,
            msg: "Cannot read string at " + index,
        };
    }

    static matchNumber(buf, index, len) {
        while (this.isBlank(buf[index]))++index;

        let number = 0;
        let isRead = false;
        let isNegative = false;

        while (index < len) {
            if (buf[index] === '-') {
                if (isNegative) return {
                    err: true,
                    msg: '- not expected at ' + index,
                }
                isNegative = true;
            } else {
                if (!this.isBlank(buf[index])) {
                    if (buf[index].match(/[0-9]/)) {
                        number = number * 10 + (+buf[index]);
                        isRead = true;
                    } else {
                        break;
                    }
                }
            }
            ++index;
        }

        if (!isRead) return {
            err: true,
            msg: "Cannot read number at " + index,
        };

        if (isNegative) number *= -1;
        return {
            value: number,
            next: index,
        }
    }

    static matchPrimitave(buf, index, len) {
        const prims = {
            'false': false,
            'true': true,
            'null': null,
            'undefined': undefined,
        };

        while (this.isBlank(buf[index])) ++index;
        let val = '';
        while (index < len) {
            if (!this.isBlank(buf[index]) && ![',', '}'].includes(buf[index])) {
                val += buf[index];
            } else {
                break;
            }
            ++index;
        }

        if (prims.hasOwnProperty(val)) {
            return {
                value: prims[val],
                next: index,
            }
        } else {
            return {
                err: true,
            }
        }
    }

    static findColon(buf, index, len) {
        let isFinded = false;
        while (index < len) {
            if (!this.isBlank(buf[index])) {
                if (buf[index] === ':') {
                    isFinded = true;
                }
                break;
            }
            ++index;
        }

        if (!isFinded) return -1;

        return index + 1;
    }
}

module.exports = MyJSON;
