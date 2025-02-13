const trill = require('./build/Release/trill');

try {
    // Create array of sensor addresses
    const sensorAddresses = [0x30];
    const sensors = [];

    // Initialize each sensor
    for (const address of sensorAddresses) {
        console.log(`Connecting to Trill CRAFT device at address 0x${address.toString(16)}...`);
        const device = new trill.Trill(address);
        device.printDetails();
        sensors.push(device);
    }
    
    function readAndPrintChannels() {
        // Read from each sensor
        sensors.forEach((device, index) => {
            device.readI2C();
            const rawData = device.getRawData();
            if (rawData.length > 0) {
                console.log(`Sensor ${index} (0x${sensorAddresses[index].toString(16)}) values:`, rawData);
            }
        });
    }

    // Read sensors every 50ms
    setInterval(readAndPrintChannels, 50);

} catch (err) {
    console.error('Failed to initialize Trill devices:', err.message);
    process.exit(1);
}
