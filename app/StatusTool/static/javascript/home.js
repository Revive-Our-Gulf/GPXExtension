document.addEventListener("DOMContentLoaded", function() {
    var submitButton = document.getElementById("submit");
    updateButtonColor(submitButton);

    document.getElementById("trackForm").addEventListener("submit", function(event) {
      event.preventDefault();
      var formData = new FormData(this);
      var xhr = new XMLHttpRequest();
      xhr.open("POST", "/", true);
      xhr.onload = function() {
        if (xhr.status === 200) {
          // Handle successful response
          console.log("Form submitted successfully");
          // Update button value and color
          submitButton.value = submitButton.value.toLowerCase() === "start" ? "Stop" : "Start";
          updateButtonColor(submitButton);
          location.reload();
        } else {
          // Handle error response
          console.error("Failed to submit form");
        }
      };
      xhr.send(formData);
    });

    function updateButtonColor(button) {
      if (button.value.toLowerCase() === "start") {
        button.classList.remove("btn-danger");
        button.classList.add("btn-success");
      } else if (button.value.toLowerCase() === "stop") {
        button.classList.remove("btn-success");
        button.classList.add("btn-danger");
      }
    }
  });