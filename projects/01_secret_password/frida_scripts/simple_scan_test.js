
// simple_scan_test.js
// A simplified script to test Memory.scan functionality.

console.log("[Frida] Simple scan test started.");

// Scan for a simple, known byte pattern (e.g., 'AAAA' in ASCII)
// '41 41 41 41' is the hexadecimal representation of 'AAAA'
Memory.scan(Process.enumerateRanges('r--'), '41 41 41 41', {
    onMatch: function(address, size) {
        console.log("[Frida] Found AAAA at: " + address);
        // Optionally read and print context
        const context = Memory.readUtf8String(address, 4);
        console.log("[Frida] Context: " + context);
        return 'stop'; // Stop after the first match
    },
    onComplete: function() {
        console.log("[Frida] Simple scan test finished.");
    }
});

console.log("[Frida] Simple scan test script end.");
