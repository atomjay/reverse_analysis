// This script encountered a TypeError: cannot read property 'onMatch' of undefined.
// This indicates a potential issue with Memory.scan callbacks in this specific Frida environment/version.
// It is kept here for reference but is currently not functional.

// find_secret.js
// This script will scan the process memory for the secret password string.

const secretPassword = "偵探小幫手";

// Manually convert string to UTF-8 byte pattern for Frida's Memory.scan
function stringToUtf8Bytes(str) {
    let bytes = [];
    for (let i = 0; i < str.length; i++) {
        let charcode = str.charCodeAt(i);
        if (charcode < 0x80) bytes.push(charcode);
        else if (charcode < 0x800) {
            bytes.push(0xc0 | (charcode >> 6),
                       0x80 | (charcode & 0x3f));
        } else if (charcode < 0xd800 || charcode >= 0xe000) {
            bytes.push(0xe0 | (charcode >> 12),
                       0x80 | ((charcode>>6) & 0x3f),
                       0x80 | (charcode & 0x3f));
        } else {
            // surrogate pair
            i++;
            charcode = 0x10000 + (((charcode & 0x3ff)<<10)
                               | (str.charCodeAt(i) & 0x3ff));
            bytes.push(0xf0 | (charcode >> 18),
                       0x80 | ((charcode>>12) & 0x3f),
                       0x80 | ((charcode>>6) & 0x3f),
                       0x80 | (charcode & 0x3f));
        }
    }
    return bytes;
}

const secretBytes = stringToUtf8Bytes(secretPassword);

console.log("[Frida] Starting memory scan for secret password: '" + secretPassword + "'");

// Convert Uint8Array to a Frida-compatible byte pattern string
let pattern = '';
for (let i = 0; i < secretBytes.length; i++) {
    pattern += (secretBytes[i] < 16 ? '0' : '') + secretBytes[i].toString(16);
    if (i < secretBytes.length - 1) {
        pattern += ' '; // Add space for pattern format
    }
}

console.log("[Frida] Searching for byte pattern: " + pattern);

let found = false;

// Debugging: Log the types of arguments before calling Memory.scan
const ranges = Process.enumerateRanges('r--');
console.log("[Frida Debug] Type of ranges: " + typeof ranges + ", isArray: " + Array.isArray(ranges) + ", length: " + ranges.length);
console.log("[Frida Debug] Type of pattern: " + typeof pattern);

// Scan all readable memory ranges for the byte pattern
// Directly pass the object literal with onMatch and onComplete
// Using arrow functions for callbacks to ensure 'this' context is preserved if that's the issue
Memory.scan(ranges, pattern, {
    onMatch: (address, size) => { // Changed to arrow function
        console.log("[Frida] Found secret password at address: " + address);
        // Optionally read and print context around the found string
        const context = Memory.readUtf8String(address.sub(20), secretPassword.length + 40); // Read 20 bytes before and 20 bytes after
        console.log("[Frida] Context: " + context);
        found = true;
        // Do not return 'stop' here, let onComplete handle the exit
    },
    onComplete: () => { // Changed to arrow function
        if (!found) {
            console.log("[Frida] Secret password not found in readable memory ranges.");
        }
        console.log("[Frida] Memory scan finished.");
        // If you want to exit Frida after scan, you can use `send()` to notify client and then `recv()` to wait for client to detach.
        // Or simply let the script finish.
    }
});