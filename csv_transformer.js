const fs = require('fs');
const path = require('path');
const csv = require('csv-parser');

const csvFilePath = path.join(__dirname, 'node_coordinates.csv');
const jsonFilePath = path.join(__dirname, 'locations_transformed.json');

const data = {};

// Read the CSV file
fs.createReadStream(csvFilePath)
  .pipe(csv())
  .on('data', (row) => {
    const address = `0x${parseInt(row['address'], 10)}`;
    const nodeId = parseInt(row['id'], 10);
    const coordinates = [parseFloat(row['x-coordinate']), parseFloat(row['y-coordinate'])];

    if (!data[address]) {
      data[address] = [];
    }

    data[address].push({
      id: nodeId,
      coordinates: coordinates
    });
  })
  .on('end', () => {
    // Transform the data into the desired JSON structure
    const jsonData = Object.keys(data).map(address => ({
      address: address,
      locations: data[address]
    }));

    // Write the JSON data to a file
    fs.writeFileSync(jsonFilePath, JSON.stringify(jsonData, null, 2));
    console.log(`Data successfully transformed and saved to ${jsonFilePath}`);
  });
