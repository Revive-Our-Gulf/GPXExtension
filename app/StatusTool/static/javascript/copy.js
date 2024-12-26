function copyToClipboard(elementId) {
    var copyText = document.getElementById(elementId).innerText;
    var textArea = document.createElement("textarea");
    textArea.value = copyText;
    document.body.appendChild(textArea);
    textArea.select();
    document.execCommand("copy");
    document.body.removeChild(textArea);

    snackbar.classList.remove("show");
    void snackbar.offsetWidth;
    snackbar.classList.add("show");

    if (snackbarTimeout) {
        clearTimeout(snackbarTimeout);
    }

    snackbarTimeout = setTimeout(function() {
        snackbar.classList.remove("show");
    }, 1500);
}

document.getElementById("snackbar").addEventListener("animationend", function() {
    this.classList.remove("show");
});
