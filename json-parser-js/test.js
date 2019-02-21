const MyJSON = require('./MyJSON');

const dataJSON = [
    {name: 'An'},
    {name: 'An', gender: 'boy'},
    {name: 'An', gender: 'boy', age: 21},
    {name: 'An\t(Beo)', gender: 'boy', age: -11, hasGF: false, girlfriend: null},
    {name: 'An\n(Beo)', gender: 'boy', age: -11, hasGF: true, girlfriend: {name: 'Someone', age: 21}},
    {name: 'An\n(Beo)', gender: 'boy', age: -11, hasGF: true, girlfriend: {name: 'Someone', age: 21, beautiful: true, bf: {name: 'An'}}, married: false},
];

const dataStr = dataJSON.map((value)=>JSON.stringify(value, null));

dataStr.forEach((value, index)=>{
    try {
        const obj = MyJSON.parse(value);
        console.log("\x1b[32m%s\x1b[0m", "PASS " + index + ": ");
        console.log(JSON.stringify(obj, null, 4));
    } catch (e) {
        console.log("\x1b[31m%s\x1b[0m", "ERROR " + index + ": " + e);
        console.log("\t" + value);
        process.exit(1);
    }
});
