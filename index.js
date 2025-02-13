const trill = require('./build/Release/trill');

try {
    console.log("Connecting to Trill CRAFT device...");
    const device = new trill.Trill();
    
    // Print device details after connection
    device.printDetails();
    
    function readAndPrintChannels() {
        // Read new data from the device
        device.readI2C();
        
        // Get raw data from all channels
        const rawData = device.getRawData();
        if (rawData.length > 0) {
            console.log('Channel values:', rawData);
        }
    }

    // Read sensor every 50ms
    setInterval(readAndPrintChannels, 50);

} catch (err) {
    console.error('Failed to initialize Trill:', err.message);
    process.exit(1);
}
