const trill = require('./build/Release/trill');
const dgram = require('node:dgram');
const client = dgram.createSocket('udp4');

try {
    // Create array of sensor addresses
    const sensorAddresses = [0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37];
    const sensors = [];

    // Initialize each sensor
    for (const address of sensorAddresses) {
        console.log(`Connecting to Trill CRAFT device at address 0x${address.toString(16)}...`);
        const device = new trill.Trill(address);
        device.setNoiseThreshold(0.05)
        device.printDetails();
        sensors.push(device);
    }

    function readAndPrintChannels() {
        // Read from each sensor
        sensors.forEach((device, index) => {
            device.readI2C();
            const rawDataArray = device.getRawData();
            const hasTouch = (currentValue) => currentValue > 0;
            if (rawDataArray.length > 0 && rawDataArray.some(hasTouch)) {
                console.log(`Sensor ${index} (0x${sensorAddresses[index].toString(16)}) values:`, {...rawDataArray});
            }
        });
	const message = [0.5,0.5,0.9,0.9];
	client.send(message.join(" "), 3002, 'localhost');
    }

    // Read sensors every 50ms
    setInterval(readAndPrintChannels, 2000);

} catch (err) {
    console.error('Failed to initialize Trill devices:', err.message);
    process.exit(1);
}
