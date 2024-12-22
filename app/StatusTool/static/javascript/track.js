function deleteTrack(track) {
    if (confirm("Are you sure you want to delete " + track + "?")) {
        var request = new XMLHttpRequest();
        request.open("GET", "/delete_track?track=" + track, true);
        request.onload = function() {
            if (request.status == 200) {
                location.reload();
            } else {
                alert("Failed to delete track.");
            }
        };
        request.send();
    }
  }