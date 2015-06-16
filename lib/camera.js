var config = require('config'),
    fs = require('fs');

var NativeCamera = require('../build/Release/camera.node');

var Camera = function () {};

Camera.prototype.init = function () {
    this.properties = {
        width: config.get('camera.width'),
        height: config.get('camera.height'),
        fps: config.get('camera.fps'),
        imagePath: config.get('camera.imagePath'),
        imagePathTemp: config.get('camera.imagePathTemp')
    };

    NativeCamera.init(this.properties);
};

Camera.prototype.getImage = function (callback) {
    fs.readFile(this.properties.imagePath, callback);
};

module.exports = Camera;
