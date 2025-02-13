const TrillAddon = require('bindings')('trill-addon');

class Trill {
    constructor() {
        this.device = new TrillAddon.TrillWrapper();
    }

    setup(bus, deviceName, address) {
        return this.device.setup(bus, deviceName, address);
    }

    read() {
        this.device.readI2C();
        const numTouches = this.device.getNumTouches();
        const touches = [];

        for (let i = 0; i < numTouches; i++) {
            const touch = {
                location: this.device.getTouchLocation(i)
            };
            if (this.device.is2D()) {
                touch.horizontalLocation = this.device.getTouchHorizontalLocation(i);
            }
            touches.push(touch);
        }

        return {
            numTouches,
            touches,
            rawData: this.device.getRawData()
        };
    }
}

module.exports = Trill;
