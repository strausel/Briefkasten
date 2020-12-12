var lastOeffnungEvent = {
    deviceId: "",
    timestamp: 0
}
var lastKlappeEvent = {
    deviceId: "",
    timestamp: 0
}
// remember how many times the buttons were pressed
var Oeffnungscounter = 0;
var Klappencounter = 0;


// react on the Events
function handleOeffnung (event) {
    // read variables from the event
    let ev = JSON.parse(event.data);
    let evData = ev.data; // the data from the argon event
    let evDeviceId = ev.coreid; // the device id
    let evTimestamp = Date.parse(ev.published_at); // the timestamp of the event

    Oeffnungscounter++;


    lastOeffnungEvent.timestamp = evTimestamp;
    lastOeffnungEvent.deviceId = evDeviceId;


    // the data we want to send to the clients
    let data = {
        message: evData, // just forward "bumped"
        counter: Oeffnungscounter,
    }

    // send data to all connected clients
    sendData("oeffnung", data, evDeviceId, evTimestamp );
}

function handleKlappe (event) {
    // read variables from the event
    let ev = JSON.parse(event.data);
    let evData = ev.data; // the data from the argon event
    let evDeviceId = ev.coreid; // the device id
    let evTimestamp = Date.parse(ev.published_at); // the timestamp of the event

    Klappencounter++;

    lastKlappeEvent.timestamp = evTimestamp;
    lastKlappeEvent.deviceId = evDeviceId;


    // the data we want to send to the clients
    let data = {
        message: evData, // just forward "bumped"
        counter: Klappencounter,
    }

    // send data to all connected clients
    sendData("klappe", data, evDeviceId, evTimestamp );
}


// send data to the clients.
// You don't have to change this function
function sendData(evName, evData, evDeviceId, evTimestamp ) {
    
    // map device id to device nr
    let nr = exports.deviceIds.indexOf(evDeviceId)

    // the message that we send to the client
    let data = {
        eventName: evName,
        eventData: evData,
        deviceNumber: nr,
        timestamp: evTimestamp,
    };

    // send the data to all connected clients
    exports.sse.send(data)
}

exports.deviceIds = [];
exports.sse = null;

// export your own functions here as well
exports.handleOeffnung = handleOeffnung;
exports.handleKlappe = handleKlappe;