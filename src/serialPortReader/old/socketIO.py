import socketio
import serial
import time
ser = serial.Serial('COM4', 9800, timeout=1)
f = open("data.js", "w+") 




sid = ""
sio = socketio.AsyncServer(async_mode='asgi')
app = socketio.ASGIApp(sio, static_files={
    '/': './src/public/'
})


async def sendMessage(message):
    await sio.emit('message', message, to=sid)
    print('this ran')

    
@sio.event
async def sum(sid, data):
    result = data['numbers'][0] + data['numbers'][1]
    return {'result': result}

@sio.event
async def connect(sid, environ):
    print(sid, 'connected')
    sid = sid


@sio.event
async def disconnect(sid):
    print(sid, 'disconnected')



for i in range(0, 10000):
    # Reading all bytes available bytes till EOL
    line = ser.readline() 
    if line:
        # Converting Byte Strings into unicode strings
        try:
            string = str(line.decode("utf-8"))
        except UnicodeDecodeError:
            string = str(line.decode("utf-8"))
         #Converting Unicode String into integer
         
        sendMessage(string)
        f.write("""thing.innerHTML = "%s" """ % string)
        print(string)
 
ser.close()
f.close()