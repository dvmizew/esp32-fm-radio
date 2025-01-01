function toggleRadio() {
    fetch('/toggleRadio')
        .then(response => response.text())
        .then(data => {
            console.log(data);
        });
}

function toggleBluetooth() {
    fetch('/toggleBluetooth')
        .then(response => response.text())
        .then(data => {
            console.log(data);
        });
}

function changeStation(increase) {
    const endpoint = increase ? '/increaseFrequency' : '/decreaseFrequency';
    fetch(endpoint)
        .then(response => response.text())
        .then(data => {
            console.log(data);
            fetchCurrentFrequency();
        });
}

function adjustVolume(increase) {
    const endpoint = increase ? '/volumeUp' : '/volumeDown';
    fetch(endpoint)
        .then(response => response.text())
        .then(data => {
            console.log(data);
            fetchCurrentVolume();
        });
}

function fetchCurrentFrequency() {
    fetch('/getFrequency')
        .then(response => response.text())
        .then(data => {
            document.getElementById('frequency').innerText = data;
        });
}

function fetchCurrentVolume() {
    fetch('/getVolume')
        .then(response => response.text())
        .then(data => {
            document.getElementById('volume').innerText = data;
        });
}

function setFrequency() {
    const frequency = document.getElementById('frequency-input').value;
    fetch(`/setFrequency?freq=${frequency}`)
        .then(response => response.text())
        .then(data => {
            console.log(data);
            fetchCurrentFrequency();
        });
}

function setStationFrequency(frequency) {
    fetch(`/setFrequency?freq=${frequency}`)
        .then(response => response.text())
        .then(data => {
            console.log(data);
            fetchCurrentFrequency();
        });
}

function playNextTrack() {
    fetch('/playNextTrack')
        .then(response => response.text())
        .then(data => {
            console.log(data);
        });
}

function playPreviousTrack() {
    fetch('/playPreviousTrack')
        .then(response => response.text())
        .then(data => {
            console.log(data);
        });
}

function togglePlayback() {
    fetch('/togglePlayback')
        .then(response => response.text())
        .then(data => {
            console.log(data);
        });
}

function volumeUp() {
    fetch('/volumeUp')
        .then(response => response.text())
        .then(data => {
            console.log(data);
            fetchCurrentVolume();
        });
}

function volumeDown() {
    fetch('/volumeDown')
        .then(response => response.text())
        .then(data => {
            console.log(data);
            fetchCurrentVolume();
        });
}

function showSection(sectionId) {
    const sections = document.querySelectorAll('.section');
    sections.forEach(section => {
        section.style.display = 'none';
    });
    document.getElementById(sectionId).style.display = 'block';
}

function toggleDarkMode() {
    document.body.classList.toggle('dark-mode');
}

function restartESP() {
    fetch('/restart')
        .then(response => {
            if (response.ok) {
                alert('ESP32 is restarting...');
            } else {
                alert('Failed to restart ESP32');
            }
        })
        .catch(error => {
            console.error('Error:', error);
            alert('Failed to restart ESP32');
        });
}

document.addEventListener('DOMContentLoaded', (event) => {
    fetchCurrentFrequency();
    fetchCurrentVolume();
});