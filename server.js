var express = require('express'),
    config = require('config'),
    fs = require('fs');

// setting camera native module up
var properties = {
    width: config.get('camera.width'),
    height: config.get('camera.height'),
    fps: config.get('camera.fps')
};

var camera = require('./build/Release/camera.node');
camera.init(properties);

// setting Express up
var IMAGE_FILE = '/dev/shm/image.jpg';
var port = config.get('server.port');

var app = express();

app.get('/', function (req, res) {
    res.writeHead(200, {
        'content-type': 'image/jpeg',
        'cache-control': 'private, no-cache, no-store, must-revalidate',
        'expires': '0',
        'pragma': 'no-cache'
    });

    var stream = fs.createReadStream(IMAGE_FILE);
    stream.pipe(res);
});

app.listen(port);
