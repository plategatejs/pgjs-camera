var express = require('express'),
    config = require('config');

// setting camera module up
var Camera = require('./lib/camera');
var camera = new Camera();

camera.init();

// setting Express up
var port = config.get('server.port');
var app = express();

app.get('/', function (req, res) {
    res.writeHead(200, {
        'content-type': 'image/jpeg',
        'cache-control': 'private, no-cache, no-store, must-revalidate',
        'expires': '0',
        'pragma': 'no-cache'
    });

    camera.getImage().pipe(res);
});

app.listen(port);
