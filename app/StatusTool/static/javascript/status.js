function load() { 
    requestUpdate();
}

function requestUpdate() { 
    var request = new XMLHttpRequest(); 
    request.onreadystatechange = function() { 
        if (request.readyState == 4 && request.status == 200) {
            updateStatus(request.responseText);
        }
    }; 
    request.open("GET", "current", true); 
    request.send(null); 
}

function updateStatus(responseText) { 
    var status = JSON.parse(responseText);

    document.getElementById("created").innerText = status.created;
    document.getElementById("latitude").innerText = parseFloat(status.latitude).toFixed(7);
    document.getElementById("longitude").innerText = parseFloat(status.longitude).toFixed(7);
    document.getElementById("heading").innerText = parseFloat(status.heading).toFixed(2);
    document.getElementById("depth").innerText = parseFloat(status.depth).toFixed(2);
    document.getElementById("altitude").innerText = parseFloat(status.altitude).toFixed(2);
    document.getElementById("temperature").innerText = parseFloat(status.temperature).toFixed(1);
    document.getElementById("mode").innerText = status.mode;
    document.getElementById("satelliteCount").innerText = status.satelliteCount;
    document.getElementById("hdop").innerText = parseFloat(status.hdop).toFixed(3);
    document.getElementById("haccuracy").innerText = parseFloat(status.haccuracy).toFixed(3);
    document.getElementById("validVelocity").innerText = status.validVelocity;

    setTimeout(requestUpdate, 1000); 
}
let snackbarTimeout;
