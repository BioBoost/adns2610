// Just a simple demo app that takes the data from the serial port
// and produces an image
// Requires serialport and jimp packages

const SerialPort = require('serialport');
const Readline = require('@serialport/parser-readline');
const port = new SerialPort('/dev/ttyACM0', {
  baudRate: 115200
});
const Jimp = require('jimp');

const parser = port.pipe(new Readline());
parser.on('data', function(line) {
  if(line.indexOf("Frame:") > -1) {
    line = line.split(":").pop();
    // Split string in elements, expands the value to RGBA and make sure
    // empty trailing and proceeding elements are filtered out
    let data = line.split('|').filter(function (val) {
      return val.length != 0;
    }).map(Number);
    let rgbaValues  = [].concat.apply([], data.map(val => {
        return [val, val, val, 255];
      })
    );
    let buffer = Buffer.from(rgbaValues);

    new Jimp({ data: buffer, width: 18, height: 18 }, (err, image) => {
      if (!err) {
        image.write('./image.png', function(err){
          if (err) {
            console.log("Failed to write image");
          }
        });
      } else {
        console.log("Failed to create image");
      }
    });
  }
});