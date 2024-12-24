document.addEventListener("DOMContentLoaded", function() {
  var status = GetStatus(document);
  UpdateSubmitButton(document, status);

  document.getElementById("trackForm").addEventListener("submit", function(event) {
    event.preventDefault();

    // Check and flip state 
    var status = GetStatus(document);
    status = status ? false : true;

    var xhr = new XMLHttpRequest();
    xhr.open("POST", "/", true);
    xhr.onload = function() {
      if (xhr.status != 200) return;
    };

    var formData = new FormData(this);

    formData.append("status", status ? "STARTED" : "STOPPED");

    var track = formData.get("track");
    if (!track) {
      alert("Track name cannot be empty!");
      return;
    }

    UpdateSubmitButton(document, status);

    xhr.send(formData);
  });

  function GetStatus(document) {
    var submitButton = document.getElementById("submit");
    return submitButton.innerText.toLowerCase().includes("start") ? false : true;
  }
  
  function UpdateSubmitButton(document, status) {
    var submitButton = document.getElementById("submit");
    var submitIcon = document.getElementById("submitIcon");
    var submitText = document.getElementById("submitText");

    if (status) {
      submitButton.classList.add("btn-danger");
      submitButton.classList.remove("btn-success");
      submitText.textContent = "Stop";
      submitIcon.classList.add("fa-stop");
      submitIcon.classList.add("fa-play");
    } else {
      submitButton.classList.add("btn-success");
      submitButton.classList.remove("btn-danger");
      submitText.textContent = "Start";
      submitIcon.classList.add("fa-play");
      submitIcon.classList.add("fa-stop");
    }
  }
});