// Create a client instance
client = new Paho.MQTT.Client('192.168.0.3', Number(1884), "clientId");

// set callback handlers
client.onConnectionLost = onConnectionLost;
client.onMessageArrived = onMessageArrived;

// connect the client
client.connect({onSuccess:onConnect});


// called when the client connects
function onConnect() {
  // Once a connection has been made, make a subscription and send a message.
  console.log("onConnect");
  client.subscribe("ruokkija");
  message = new Paho.MQTT.Message("Hello");
  message.destinationName = "World";
  message.qos=1;
  client.send(message);
  console.log(message);
}

// called when the client loses its connection
function onConnectionLost(responseObject) {
  if (responseObject.errorCode !== 0) {
    console.log("onConnectionLost:"+responseObject.errorMessage);
  }
}

// called when a message arrives
function onMessageArrived(message) {
  //console.log("onMessageArrived:"+message.payloadString);
  $(".mqtt").text(message.payloadString);
 
  
}