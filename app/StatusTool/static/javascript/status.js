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

    if (document.getElementById("created")) document.getElementById("created").innerText = status.created;
    if (document.getElementById("latitude")) document.getElementById("latitude").innerText = parseFloat(status.latitude).toFixed(7);
    if (document.getElementById("longitude")) document.getElementById("longitude").innerText = parseFloat(status.longitude).toFixed(7);
    if (document.getElementById("heading")) document.getElementById("heading").innerText = parseFloat(status.heading).toFixed(2);
    if (document.getElementById("depth")) document.getElementById("depth").innerText = parseFloat(status.depth).toFixed(2);
    if (document.getElementById("temperature")) document.getElementById("temperature").innerText = parseFloat(status.temperature).toFixed(1);
    if (document.getElementById("driveMode")) document.getElementById("driveMode").innerText = status.driveMode;
    if (document.getElementById("satellites")) document.getElementById("satellites").innerText = status.satellites;
    if (document.getElementById("hdop")) document.getElementById("hdop").innerText = parseFloat(status.hdop).toFixed(3);
    if (document.getElementById("haccuracy")) document.getElementById("haccuracy").innerText = parseFloat(status.haccuracy).toFixed(3);
    if (document.getElementById("distance")) document.getElementById("distance").innerText = parseFloat(status.distance).toFixed(3);
    if (document.getElementById("fom")) document.getElementById("fom").innerText = parseFloat(status.fom).toFixed(3);
    if (document.getElementById("validVelocity")) document.getElementById("validVelocity").innerText = status.validVelocity;

    setTimeout(requestUpdate, 1000); 
}
let snackbarTimeout;
