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
    document.getElementById("latitude").innerText = status.latitude + "°";
    document.getElementById("longitude").innerText = status.longitude + "°";
    document.getElementById("heading").innerText = status.heading + "°";
    document.getElementById("depth").innerText = status.depth + " m";
    document.getElementById("altitude").innerText = status.altitude + " m";
    document.getElementById("temperature").innerText = status.temperature + "°C";
    document.getElementById("mode").innerText = status.mode;
    document.getElementById("satelliteCount").innerText = status.satelliteCount;
    document.getElementById("poseCertainty").innerText = status.poseCertainty;
    document.getElementById("validVelocity").innerText = status.validVelocity;
    document.getElementById("fom").innerText = status.fom;

    setTimeout(requestUpdate, 1000); 
}