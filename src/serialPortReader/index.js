//most of the basic code for websocket connection through python and js is copied at its base
// from Miguel Grinberg

const sio = io();
text = document.getElementById('text');

sio.on('connect', () => {
  console.log('connected');
  sio.emit('sum', {numbers: [1, 2]}, (result) => {
    console.log(result);
  });
});

sio.on('disconnect', () => {
  console.log('disconnected');
});

sio.on('message', (message) => {
  console.log('here is the message: ' + message);
  text.innerHTML = message;
  
});