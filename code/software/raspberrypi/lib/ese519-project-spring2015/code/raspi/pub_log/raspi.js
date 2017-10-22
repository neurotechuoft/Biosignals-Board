// Assumptions
// 1. node.js is installed and so is socket.io
// 2. socket.io is located at the path below

var io = require('/usr/lib/node_modules/socket.io').listen(8080);

function rnd() {
    var num=Math.floor(Math.random()*1000);
    return num;
}

io.sockets.on('connection', function (socket) {
    
    // At an interval of 1000 ms keep pushing out random numbers
    setInterval( function() {
        var n=rnd();
        socket.broadcast.emit('stream', {n:n.toString()});
    }, 1000);

});
