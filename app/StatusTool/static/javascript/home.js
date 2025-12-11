document.addEventListener("DOMContentLoaded", function() {
  var status = GetStatus(document);
  UpdateSubmitButton(document, status);

  updateTracks();

  document.getElementById("saveTrackButton").addEventListener("click", function(event) {
    event.preventDefault();

    var track = document.getElementById("track").value;
    if (!track) {
      alert("Track name cannot be empty!");
      return;
    }

    var xhr = new XMLHttpRequest();
    xhr.open("POST", "/saveTrack", true);
    xhr.onload = function() {
      if (xhr.status != 200) {
        alert("Failed to save track name.");
      } else {
        document.getElementById("currentTrackName").textContent = "Current Track: " + track;
      }
    };

    var formData = new FormData();
    formData.append("track", track);
    xhr.send(formData);
  });

  document.getElementById("startStopButton").addEventListener("click", function(event) {
    event.preventDefault();

    var status = GetStatus(document);
    status = status ? false : true;

    var xhr = new XMLHttpRequest();
    xhr.open("POST", "/startStop", true);
    xhr.onload = function() {
      if (xhr.status != 200) {
        alert("Failed to start/stop recording.");
        return;
      }
      UpdateSubmitButton(document, status);
      ToggleRecordingIndicator(status);
    };

    var formData = new FormData();
    formData.append("status", status ? "STARTED" : "STOPPED");
    xhr.send(formData);
    updateTracks();
  });

  function GetStatus(document) {
    var submitButton = document.getElementById("startStopButton");
    return submitButton.innerText.toLowerCase().includes("record") ? false : true;
  }

  function UpdateSubmitButton(document, status) {
    var submitButton = document.getElementById("startStopButton");
    var submitIcon = document.getElementById("submitIcon");
    var submitText = document.getElementById("submitText");

    if (status) {
      submitButton.classList.add("btn-danger");
      submitButton.classList.remove("btn-success");
      submitText.textContent = "Stop";
      submitIcon.classList.add("fa-stop");
      submitIcon.classList.remove("fa-play");
    } else {
      submitButton.classList.add("btn-success");
      submitButton.classList.remove("btn-danger");
      submitText.textContent = "Record";
      submitIcon.classList.add("fa-play");
      submitIcon.classList.remove("fa-stop");
    }
  }

  function ToggleRecordingIndicator(status) {
    var recordingIndicator = document.getElementById("recordingIndicator");
    if (status) {
      recordingIndicator.style.display = "inline-block";
    } else {
      recordingIndicator.style.display = "none";
    }
  }

  function checkAndUpdateTracks() {
    var status = GetStatus(document);
    if (status) {
      updateTracks();
    }
  }

  function updateTracks() {
    var xhr = new XMLHttpRequest();
    xhr.open("GET", "/tracks", true);
    xhr.onload = function() {
      if (xhr.status === 200) {
        document.querySelector("#tracksTable tbody").innerHTML = xhr.responseText;
      }
    };
    xhr.send();
  }
});