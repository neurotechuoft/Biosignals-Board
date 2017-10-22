// connect via socket
var socket = io.connect('http://127.0.0.1:8080');

// connect to the socket stream and log in data
socket.on('stream', function (data) {
	console.log(data.n);
	document.getElementById('number').innerHTML=data.n;
});
 
