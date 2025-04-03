
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
        sensors.push(device);
    }


    for (const sensor of sensors) {
    	sensor.setNoiseThreshold(0.04);
    	sensor.setPrescaler(2);
    	sensor.updateBaseline();
        sensor.printDetails();
    }

    function readAndPrintChannels() {
        let touchedLocations = [];
        // Read from each sensor
        sensors.forEach((device, index) => {
            device.readI2C();
            const rawDataArray = device.getRawData();
            const hasTouch = (currentValue) => currentValue > 0.01;
            
            if (rawDataArray.length > 0 && rawDataArray.some(hasTouch)) {
                const address = sensorAddresses[index].toString(16);
                // console.log(`Sensor ${index} (0x${sensorAddresses[index].toString(16)}) values:`, {...rawDataArray});
                const touchedIndexes = rawDataArray.reduce((acc, value, index) => acc.concat(value > 0.01 && value !== 8 && value !== 0.03125 ? [address,index,value] : []), []);
                // console.log(touchedIndexes);
                touchedLocations.push(touchedIndexes);
            }
        });
        
        if (touchedLocations.length > 0) {
        console.log(touchedLocations.flat(Infinity).join(" "));
            client.send(touchedLocations.flat(Infinity).join(" "), 3002, 'localhost');
        }
    }

    // Read sensors every 50ms
    setInterval(readAndPrintChannels, 100);

} catch (err) {
    console.error('Failed to initialize Trill devices:', err.message);
    process.exit(1);
}
