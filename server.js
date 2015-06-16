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
    camera.getImage(function (err, data) {
        if (!err) {
            res.writeHead(200, {
                'content-type': 'image/jpeg',
                'content-length': data.length,
                'cache-control': 'private, no-cache, no-store, must-revalidate',
                'expires': '0',
                'pragma': 'no-cache'
            });

            res.end(data);
        }
    });
});

app.listen(port);
