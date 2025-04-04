const trill = require('./build/Release/trill');
const dgram = require('node:dgram');
const client = dgram.createSocket('udp4');
const jsonData = require('./locations_transformed.json');

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
        let locations = [];
        let touchedLocations = [];

	// Read from each sensor
        sensors.forEach((device, index) => {
            device.readI2C();
            const rawDataArray = device.getRawData();
            const hasTouch = (currentValue) => currentValue > 0.01;

            // FOR DEBUGGING PURPOSES ONLY!
            if (rawDataArray.length > 0 && rawDataArray.some(hasTouch)) {
                const address = sensorAddresses[index].toString(16);
                const touchedIndexes = rawDataArray.reduce((acc, value, index) => acc.concat(value > 0.01 && value !== 8 && value !== 0.03125 ? [address,index,value] : []), []);
                touchedLocations.push(touchedIndexes);
            }

            const address = `0x${sensorAddresses[index].toString(16)}`;

            // correlate the rawDataArray with the json data
            const correlatedData = jsonData.find(node => node.address === address).locations.filter((location, index) => location.id === index).flatMap(location => [location.coordinates, rawDataArray[location.id]]);
            locations = locations.concat(correlatedData);
        });


	// FOR DEBUGGING PURPOSES ONLY!
        if (touchedLocations.length > 0) {
          console.log(touchedLocations.flat(Infinity).join(" "));
          //console.log(locations.join(" "));
        }

        client.send(locations.join(" "), 3002, 'localhost');
    }

    // Read sensors every 50ms
    setInterval(readAndPrintChannels, 100);

} catch (err) {
    console.error('Failed to initialize Trill devices:', err.message);
    process.exit(1);
}
