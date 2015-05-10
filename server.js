var express = require('express');
var path = require('path');
var fs = require('fs');

// setting environment type
if (!process.env.hasOwnProperty('NODE_ENV') || ['production', 'development'].indexOf(process.env['NODE_ENV']) === -1) {
    process.env['NODE_ENV'] = 'production';
}

global.ENV = process.env['NODE_ENV'];

// loading camera native module
var camera = require(path.resolve('build', ENV === 'development' ? 'Debug' : 'Release', 'camera.node'));

camera.init({
    width: 640,
    height: 480,
    fps: 15.0
});

var app = express();

app.get('/', function (req, res) {
    res.writeHead(200, {
        'content-type': 'image/jpeg',
        'cache-control': 'private, no-cache, no-store, must-revalidate',
        'expires': '0',
        'pragma': 'no-cache'
    });

    var stream = fs.createReadStream(path.resolve('/dev/shm/image.jpg'));
    stream.pipe(res);
});

app.listen(1337);
