var rootUrl = window.location.origin; // get the root URL, e.g. https://example.herokuapp.com
//var moment = require('moment');

var app = new Vue({
    el: "#app",
    data: {
        a1_0: "unknown", // Geschwindigkeitswerte für Öffnung
        b1_0: "unknown",
        c1_0: "unknown",
        to_0: "unknown",

        v1_0: "unknown", // Geschwindigkeitswerte für Klappe
        w1_0: "unknown",
        x1_0: "unknown",
        tk_0: "unknown",

        weight_0: "unknown",
        Postmenge_0: "unknown",


        TotalWeight: 0,
        Oeffnungscounter: 0,    // Öffnungszähler
        Klappencounter: 0,       // Klappenzähler
        Wochentag: "unknown",
        Datum: "unknown",

    },
    // This function is executed once when the page is loaded.
    mounted: function () {
        this.initSse();
    },
    methods: {
        // Initialise the Event Stream (Server Sent Events)
        // You don't have to change this function
        initSse: function () {
            if (typeof (EventSource) !== "undefined") {
                var url = rootUrl + "/api/events";
                var source = new EventSource(url);
                source.onmessage = (event) => {
                    this.updateVariables(JSON.parse(event.data));
                };
            } else {
                this.message = "Your browser does not support server-sent events.";
            }
        },
        
        // Remote Funktionen aufrufen
        remoteKlappe: function (nr) {
            var duration = 2000; // blinking duration in milliseconds
            if(nr == 1) {
                axios.post(rootUrl + "/api/device/0/function/remoteKlappe", { arg: duration })
                .then(response => {
                    // Handle the response from the server
                    console.log(response.data); // we could to something meaningful with the return value here ... 
                })
                .catch(error => {
                    alert("Could not call the function 'remoteKlappe' of device number 0.\n\n" + error)
                })
            }else{
            axios.post(rootUrl + "/api/device/" + nr + "/function/remoteKlappe", { arg: duration })
                .then(response => {
                    // Handle the response from the server
                    console.log(response.data); // we could to something meaningful with the return value here ... 
                })
                .catch(error => {
                    alert("Could not call the function 'remoteKlappe' of device number " + nr + ".\n\n" + error)
                })
            }
            
        },
        remoteOeffnung: function (nr) {
            var duration = 2000; // blinking duration in milliseconds
            if(nr == 1) {
                axios.post(rootUrl + "/api/device/0/function/remoteOeffnung", { arg: duration })
                .then(response => {
                    // Handle the response from the server
                    console.log(response.data); // we could to something meaningful with the return value here ... 
                })
                .catch(error => {
                    alert("Could not call the function 'remoteOeffnung' of device number 0.\n\n" + error)
                })
            }else{
            axios.post(rootUrl + "/api/device/" + nr + "/function/remoteOeffnung", { arg: duration })
                .then(response => {
                    // Handle the response from the server
                    console.log(response.data); // we could to something meaningful with the return value here ... 
                })
                .catch(error => {
                    alert("Could not call the function 'remoteOeffnung' of device number " + nr + ".\n\n" + error)
                   })
            }
            
        },
        // get the value of the variables on the device with number "nr" from your backend
        getVariables: function (nr) {
            axios.get(rootUrl + "/api/device/" + nr + "/variable/a1")
                .then(response => {
                    // Handle the response from the server
                    var a1 = response.data.result;
                    if (nr === 0) {
                        this.a1_0 = a1;
                    }
                    else {
                        console.log("unknown device number: " + nr);
                    }
                })
                .catch(error => {
                    alert("Could not read variable a1 of device number " + nr + ".\n\n" + error)
                })
                axios.get(rootUrl + "/api/device/" + nr + "/variable/b1")
                .then(response => {
                    // Handle the response from the server
                    var b1 = response.data.result;
                    if (nr === 0) {
                        this.b1_0 = b1;
                    }
                    else {
                        console.log("unknown device number: " + nr);
                    }
                })
                .catch(error => {
                    alert("Could not read variable b1 of device number " + nr + ".\n\n" + error)
                })
                axios.get(rootUrl + "/api/device/" + nr + "/variable/c1")
                .then(response => {
                    // Handle the response from the server
                    var c1 = response.data.result;
                    if (nr === 0) {
                        this.c1_0 = c1;
                    }
                    else {
                        console.log("unknown device number: " + nr);
                    }
                })
                .catch(error => {
                    alert("Could not read variable c1 of device number " + nr + ".\n\n" + error)
                })
                axios.get(rootUrl + "/api/device/" + nr + "/variable/to")
                .then(response => {
                    // Handle the response from the server
                    var to = response.data.result;
                    if (nr === 0) {
                        this.to_0 = to;
                    }
                    else {
                        console.log("unknown device number: " + nr);
                    }
                })
                .catch(error => {
                    alert("Could not read variable to of device number " + nr + ".\n\n" + error)
                })
        },

            getVariables1: function (nr) {
                axios.get(rootUrl + "/api/device/" + nr + "/variable/v1")
                .then(response => {
                    // Handle the response from the server
                    var v1 = response.data.result;
                    if (nr === 0) {
                        this.v1_0 = v1;
                    }
                    else {
                        console.log("unknown device number: " + nr);
                    }
                })
                .catch(error => {
                    alert("Could not read variable v1 of device number " + nr + ".\n\n" + error)
                })
                axios.get(rootUrl + "/api/device/" + nr + "/variable/w1")
                .then(response => {
                    // Handle the response from the server
                    var w1 = response.data.result;
                    if (nr === 0) {
                        this.w1_0 = w1;
                    }
                    else {
                        console.log("unknown device number: " + nr);
                    }
                })
                .catch(error => {
                    alert("Could not read variable w1 of device number " + nr + ".\n\n" + error)
                })
                axios.get(rootUrl + "/api/device/" + nr + "/variable/x1")
                .then(response => {
                    // Handle the response from the server
                    var x1 = response.data.result;
                    if (nr === 0) {
                        this.x1_0 = x1;
                    }
                    else {
                        console.log("unknown device number: " + nr);
                    }
                })
                .catch(error => {
                    alert("Could not read variable x1 of device number " + nr + ".\n\n" + error)
                })
                axios.get(rootUrl + "/api/device/" + nr + "/variable/tk")
                .then(response => {
                    // Handle the response from the server
                    var tk = response.data.result;
                    if (nr === 0) {
                        this.tk_0 = tk;
                    }
                    else {
                        console.log("unknown device number: " + nr);
                    }
                })
                .catch(error => {
                    alert("Could not read variable tk of device number " + nr + ".\n\n" + error)
                })
        },

            getVariables2: function (nr) {
                axios.get(rootUrl + "/api/device/" + nr + "/variable/weight")
                .then(response => {
                    // Handle the response from the server
                    var weight = response.data.result;
                    if (nr === 0) {
                        this.weight_0 = weight;
                    }
                    else {
                        console.log("unknown device number: " + nr);
                    }
                })
                .catch(error => {
                    alert("Could not read variable weight of device number " + nr + ".\n\n" + error)
                })
                axios.get(rootUrl + "/api/device/" + nr + "/variable/Postmenge")
                .then(response => {
                    // Handle the response from the server
                    var Postmenge = response.data.result;
                    if (nr === 0) {
                        this.Postmenge_0 = Postmenge;
                    }
                    else {
                        console.log("unknown device number: " + nr);
                    }
                })
                .catch(error => {
                    alert("Could not read variable Postmenge of device number " + nr + ".\n\n" + error)
                })
                
            },
            // react on events: update the variables to be displayed
        updateVariables(ev) {
            // Event "bump"
            if (ev.eventName === "oeffnung") {
                this.Oeffnungscounter = ev.eventData.counter;
                //this.oeffnungSync = ev.eventData.oeffnungSync;
            }
            if (ev.eventName === "klappe") {
                this.Klappencounter = ev.eventData.counter;
                //this.klappeSync = ev.eventData.klappeSync;
            }

        },
        
    }
})
